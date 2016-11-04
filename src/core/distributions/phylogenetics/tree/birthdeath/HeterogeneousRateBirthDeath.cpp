#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "HeterogeneousRateBirthDeath.h"
#include "OdeHeterogeneousRateBirthDeath.h"

#include <algorithm>
#include <cmath>
#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;

HeterogeneousRateBirthDeath::HeterogeneousRateBirthDeath( const TypedDagNode<double> *a, const TypedDagNode<int> *rs, const TypedDagNode<RbVector<double> > *s, const TypedDagNode<RbVector<double> > *e, const TypedDagNode<double > *ev, const TypedDagNode< double > *r, const std::vector<Taxon> &n) : AbstractCharacterHistoryBirthDeathProcess(),
    root_age( a ),
    root_state( rs ),
    speciation( s ),
    extinction( e ),
    event_rate( ev ),
    rho( r ),
    branch_histories( NULL, 1, speciation->getValue().size() ),
    taxa( n ),
    activeLikelihood( std::vector<size_t>(2*n.size()-1, 0) ),
    changed_nodes( std::vector<bool>(2*n.size()-1, false) ),
    dirty_nodes( std::vector<bool>(2*n.size()-1, true) ),
    nodeStates( std::vector<std::vector<state_type> >(2*n.size()-1, std::vector<state_type>(2,std::vector<double>(1+speciation->getValue().size(),0))) ),
    scalingFactors( std::vector<std::vector<double> >(2*n.size()-1, std::vector<double>(2,0.0) ) ),
    totalScaling( 0.0 ),
    NUM_TIME_SLICES( 200.0 )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( root_age );
    addParameter( root_state );
    addParameter( speciation );
    addParameter( extinction );
    addParameter( event_rate );
    addParameter( rho );
    
    num_taxa = taxa.size();
    num_rate_categories = speciation->getValue().size();
    
    if ( num_rate_categories != extinction->getValue().size() )
    {
        throw RbException("The number of speciation rates and extinction rates is not equal.");
    }
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= num_taxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (num_taxa - 1) * RbConstants::LN2 - lnFact ;
    
    simulateTree();
    
}


HeterogeneousRateBirthDeath::~HeterogeneousRateBirthDeath()
{
    
}


/**
 * Recursive call to attach ordered interior node times to the time tree psi. Call it initially with the
 * root of the tree.
 */
void HeterogeneousRateBirthDeath::attachTimes(Tree* psi, std::vector<TopologyNode *> &nodes, size_t index, const std::vector<double> &interiorNodeTimes, double originTime )
{
    
    if (index < num_taxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of nodes
        size_t node_index = static_cast<size_t>( floor(rng->uniform01()*nodes.size()) );
        
        // Get the node from the list
        TopologyNode* parent = nodes.at(node_index);
        psi->getNode( parent->getIndex() ).setAge( originTime - interiorNodeTimes[index] );
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(node_index));
        
        // Add the left child if an interior node
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            nodes.push_back(leftChild);
        }
        
        // Add the right child if an interior node
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            nodes.push_back(rightChild);
        }
        
        // Recursive call to this function
        attachTimes(psi, nodes, index+1, interiorNodeTimes, originTime);
    }
    
}


/** Build random binary tree to size num_taxa. The result is a draw from the uniform distribution on histories. */
void HeterogeneousRateBirthDeath::buildRandomBinaryHistory(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < num_taxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // Get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // Remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(index));
        
        // Add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // Add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // Recursive call to this function
        buildRandomBinaryHistory(tips);
    }
}


/* Clone function */
HeterogeneousRateBirthDeath* HeterogeneousRateBirthDeath::clone( void ) const
{
    
    return new HeterogeneousRateBirthDeath( *this );
}


/* Compute probability */
double HeterogeneousRateBirthDeath::computeLnProbability( void )
{
    // for now
    totalScaling = 0;
    
    // Variable declarations and initialization
    double lnProb = 0.0;
    double age = root_age->getValue();
    
    // we need to check that the root age matches
    if ( age != value->getRoot().getAge() )
    {
        return RbConstants::Double::neginf;
    }
    
    
    // check that the ages are in correct chronological order
    // i.e., no child is older than its parent
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isRoot() == false )
        {
            
            if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( (the_node.getAge() - (*it)->getParent().getAge()) > 1E-6 && the_node.isSampledAncestor() == true )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // check that the sampled ancestor nodes have a zero branch length
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isSampledAncestor() == true )
        {
            
            if ( the_node.isFossil() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( the_node.getBranchLength() > 1E-6 )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // add the survival of a second species if we condition on the MRCA
    lnProb = computeRootLikelihood();
    
    return lnProb + logTreeTopologyProb;
}


void HeterogeneousRateBirthDeath::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t node_index)
{
    
    // check for recomputation
    if ( dirty_nodes[node_index] || true )
    {
        // mark as computed
        dirty_nodes[node_index] = false;
        
        
        const BranchHistory& bh = branch_histories[ node_index ];
        const std::multiset<CharacterEvent*,CharacterEventCompare>& hist = bh.getHistory();
        
//        const std::vector<CharacterEvent*> child_states = bh.getChildCharacters();
//        size_t start_index = child_states[0]->getState();
        size_t state_index_tipwards  = computeStartIndex( node_index );
        size_t state_index_rootwards = computeStartIndex( node.getParent().getIndex() );

        std::vector<double> initialState = std::vector<double>(1+num_rate_categories,0);
        if ( node.isTip() )
        {
            // this is a tip node
            
            double samplingProbability = rho->getValue();
            for (size_t i=0; i<num_rate_categories; ++i)
            {
                initialState[i] = 1.0 - samplingProbability;
            }
            initialState[num_rate_categories] = samplingProbability;
            
        }
        else
        {
            // this is an internal node
            const TopologyNode &left = node.getChild(0);
            size_t left_index = left.getIndex();
            computeNodeProbability( left, left_index );
            const TopologyNode &right = node.getChild(1);
            size_t right_index = right.getIndex();
            computeNodeProbability( right, right_index );
            
            // now compute the likelihoods of this internal node
            const std::vector<double> &leftStates = nodeStates[left_index][activeLikelihood[left_index]];
            const std::vector<double> &rightStates = nodeStates[right_index][activeLikelihood[right_index]];
            const RbVector<double> &birthRate = speciation->getValue();
            for (size_t i=0; i<num_rate_categories; ++i)
            {
                initialState[i] = leftStates[i];
            }
            
            initialState[num_rate_categories] = leftStates[num_rate_categories]*rightStates[num_rate_categories]*birthRate[ state_index_tipwards ];
            
        }
        
        const RbVector<double> &s = speciation->getValue();
        const RbVector<double> &e = extinction->getValue();
        double                  r = event_rate->getValue();
        double beginAge = node.getAge();
//        double endAge = node.getParent().getAge();
        
        double begin_time = 0.0;
        double branch_length = node.getBranchLength();
        for (std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it=hist.begin(); it!=hist.end(); ++it)
        {
            CharacterEvent* event = *it;
            double end_time = event->getTime();
            double time_interval = end_time - begin_time;
            
            // we need to set the current rate category
            size_t current_state = event->getState();
            
//            updateBranchProbabilitiesNumerically(initialState, beginAge, beginAge+time_interval, s, e, r, current_state);
            updateBranchProbabilitiesNumerically(initialState, beginAge, beginAge+time_interval, s, e, r, current_state);
            
            initialState[num_rate_categories] = initialState[num_rate_categories]*event_rate->getValue()* (1.0/num_rate_categories);
            
            
            begin_time = end_time;
            beginAge += time_interval;
        }
        
        double time_interval = branch_length - begin_time;
        updateBranchProbabilitiesNumerically(initialState, beginAge, beginAge+time_interval, s, e, r, state_index_rootwards);
        
        
        // rescale the states
        double max = initialState[num_rate_categories];
        initialState[num_rate_categories] = 1.0;
        
//        totalScaling -= scalingFactors[node_index][activeLikelihood[node_index]];
//        scalingFactors[node_index][activeLikelihood[node_index]] = log(max);
//        totalScaling += scalingFactors[node_index][activeLikelihood[node_index]] - scalingFactors[node_index][activeLikelihood[node_index]^1];
//        totalScaling += scalingFactors[node_index][activeLikelihood[node_index]];
        
        totalScaling += log(max);
        
        // store the states
        nodeStates[node_index][activeLikelihood[node_index]] = initialState;
    }
    
}


size_t HeterogeneousRateBirthDeath::computeStartIndex(size_t i) const
{
    
    size_t node_index = i;
    while ( value->getNode(node_index).isRoot() == false && branch_histories[node_index].getNumberEvents() == 0)
    {
        node_index = value->getNode(node_index).getParent().getIndex();
    }
    
    
    if ( value->getNode(node_index).isRoot() == false )
    {
        const BranchHistory &bh = branch_histories[ node_index ];
        const std::multiset<CharacterEvent*, CharacterEventCompare> &h = bh.getHistory();
        CharacterEvent *event = *(h.begin());
        return event->getState();
    }
    else
    {
        return root_state->getValue()-1;
    }
    
}


double HeterogeneousRateBirthDeath::computeRootLikelihood( void )
{
    
    const TopologyNode &root = value->getRoot();
//    size_t root_index = root.getIndex();
    
    // fill the like
    const TopologyNode &left = root.getChild(0);
    size_t left_index = left.getIndex();
    computeNodeProbability( left, left_index );
    const TopologyNode &right = root.getChild(1);
    size_t right_index = right.getIndex();
    computeNodeProbability( right, right_index );
    
    
    // now compute the likelihoods of this internal node
    std::vector< double > leftStates = nodeStates[left_index][activeLikelihood[left_index]];
    std::vector< double > rightStates = nodeStates[right_index][activeLikelihood[right_index]];
    
    double prob = leftStates[num_rate_categories]*rightStates[num_rate_categories];
    
    return log(prob) + totalScaling;
}


void HeterogeneousRateBirthDeath::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<int> &rv) const
{
    
    if ( n == "numberEvents" )
    {
        size_t num_branches = branch_histories.getNumberBranches();
        rv.clear();
        rv.resize( num_branches );
        
        for (size_t i = 0; i < num_branches; ++i)
        {
            rv[i] = int(branch_histories[i].getNumberEvents());
        }
        
    }
    else
    {
        throw RbException("The heterogeneous rate birth-death process does not have a member method called '" + n + "'.");
    }
    
}


void HeterogeneousRateBirthDeath::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<double> &rv) const
{
    
    if ( n == "averageSpeciationRate" )
    {
        size_t num_branches = branch_histories.getNumberBranches();
        const RbVector<double> &lambda = speciation->getValue();
        rv.clear();
        rv.resize( num_branches );
        
        for (size_t i = 0; i < num_branches; ++i)
        {
            const TopologyNode &node = this->value->getNode( i );
            const BranchHistory& bh = branch_histories[ i ];
            const std::multiset<CharacterEvent*,CharacterEventCompare>& hist = bh.getHistory();
            size_t state_index_rootwards = computeStartIndex( node.getParent().getIndex() );
            
            double rate = 0;
            double begin_time = 0.0;
            double branch_length = node.getBranchLength();
            for (std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it=hist.begin(); it!=hist.end(); ++it)
            {
                CharacterEvent* event = *it;
                double end_time = event->getTime();
                double time_interval = (end_time - begin_time) / branch_length;
                
                // we need to set the current rate caterogy
                size_t current_state = event->getState();

                rate += time_interval * lambda[current_state];
                
                begin_time = end_time;
            }
            rate += (branch_length-begin_time)/branch_length * lambda[state_index_rootwards];
            
            rv[i] = rate;
            
        }
        
    }
    else if ( n == "averageExtinctionRate" )
    {
        size_t num_branches = branch_histories.getNumberBranches();
        const RbVector<double> &mu = extinction->getValue();
        rv.clear();
        rv.resize( num_branches );
        
        for (size_t i = 0; i < num_branches; ++i)
        {
            const TopologyNode &node = this->value->getNode( i );
            const BranchHistory& bh = branch_histories[ i ];
            const std::multiset<CharacterEvent*,CharacterEventCompare>& hist = bh.getHistory();
            size_t state_index_rootwards = computeStartIndex( node.getParent().getIndex() );
            
            double rate = 0;
            double begin_time = 0.0;
            double branch_length = node.getBranchLength();
            for (std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it=hist.begin(); it!=hist.end(); ++it)
            {
                CharacterEvent* event = *it;
                double end_time = event->getTime();
                double time_interval = (end_time - begin_time) / branch_length;
                
                // we need to set the current rate caterogy
                size_t current_state = event->getState();
                
                rate += time_interval * mu[current_state];
                
                begin_time = end_time;
            }
            rate += (branch_length-begin_time)/branch_length * mu[state_index_rootwards];
            
            rv[i] = rate;
            
        }
        
    }
    else
    {
        throw RbException("The heterogeneous rate birth-death process does not have a member method called '" + n + "'.");
    }
    
}


/**
 * Get the character history object.
 */
CharacterHistory& HeterogeneousRateBirthDeath::getCharacterHistory( void )
{
    
    return branch_histories;
}

/**
 * Get the character history object.
 */
CharacterHistory HeterogeneousRateBirthDeath::getCharacterHistory( void ) const
{
    
    return branch_histories;
}


void HeterogeneousRateBirthDeath::redrawValue( void )
{
    simulateTree();
}


void HeterogeneousRateBirthDeath::setValue(Tree *v, bool force)
{
    
    // delegate to the parent class
    TypedDistribution< Tree >::setValue(v, force);
    
    branch_histories.setTree( value );
    
}


/** Simulate the tree conditioned on the time of origin */
void HeterogeneousRateBirthDeath::simulateTree( void )
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Create the time tree object (topology + times)
    Tree *psi = new Tree();
    
    // Root the topology by setting the appropriate flag
    psi->setRooted( true );
    
    // Create the root node and a vector of nodes
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    
    // Draw a random tree history
    buildRandomBinaryHistory(nodes);
    
    // Set the tip names
    for (size_t i=0; i<num_taxa; i++)
    {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // Get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(index) );
        
        // Set taxon
        node->setTaxon( taxa[i] );
    }
    
    // Initialize the topology by setting the root
    psi->setRoot(root);
    
    // Now simulate the speciation times counted from originTime
    std::vector<double> intNodeTimes;
    double              t_or = root_age->getValue();
    intNodeTimes.push_back( 0.0 );  // For time of mrca
    for ( size_t i=0; i<num_taxa-2; i++ )
    {
        double t = rng->uniform01() * t_or;
        intNodeTimes.push_back( t );
    }
    
    // Sort the speciation times from 0.0 (root node) to the largest value
    std::sort( intNodeTimes.begin(), intNodeTimes.end() );
    
    // Attach times
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, intNodeTimes, t_or);
    for (size_t i = 0; i < num_taxa; ++i)
    {
        TopologyNode& node = psi->getTipNode(i);
        psi->getNode( node.getIndex() ).setAge( 0.0 );
    }
    
    // Finally store the new value
    value = psi;
    
    branch_histories.setTree( psi );
    
}

void HeterogeneousRateBirthDeath::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age && this->dag_node != NULL )
    {
        dag_node->getAffectedNodes( affected );
    }
    
}

/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void HeterogeneousRateBirthDeath::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age && this->dag_node != NULL )
    {
        dag_node->keepAffected();
    }
    
}

/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void HeterogeneousRateBirthDeath::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age && this->dag_node != NULL )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dag_node->restoreAffected();
    }
    
}


void HeterogeneousRateBirthDeath::runMCMC( void )
{
    
    
    
}

/** Swap a parameter of the distribution */
void HeterogeneousRateBirthDeath::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == root_age)
    {
        root_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == root_state)
    {
        root_state = static_cast<const TypedDagNode<int>* >( newP );
    }
    else if (oldP == speciation)
    {
        speciation = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    else if (oldP == extinction)
    {
        extinction = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    else if (oldP == event_rate)
    {
        event_rate = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho)
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void HeterogeneousRateBirthDeath::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        
        if ( this->dag_node != NULL )
        {
            dag_node->touchAffected();
        }
        
    }
    
}


void HeterogeneousRateBirthDeath::updateBranchProbabilitiesNumerically(std::vector<double> &state, double begin, double end, const RbVector<double> &lambda, const RbVector<double> &mu, double delta, size_t current_rate_category)
{
    //    double dt = 0.1;
    double dt = root_age->getValue() / NUM_TIME_SLICES;
    
    OdeHeterogeneousRateBirthDeath ode = OdeHeterogeneousRateBirthDeath(lambda,mu,delta);
    ode.setCurrentRateCategory( current_rate_category );
    boost::numeric::odeint::runge_kutta4< state_type > stepper;
    boost::numeric::odeint::integrate_const( stepper, ode , state , begin , end, dt );
    
}

