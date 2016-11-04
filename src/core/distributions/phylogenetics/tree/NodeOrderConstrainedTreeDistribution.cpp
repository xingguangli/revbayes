#include "RelativeNodeAgeConstraints.h"
#include "NodeOrderConstrainedTreeDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    o         Origin or time of the process.
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
NodeOrderConstrainedTreeDistribution::NodeOrderConstrainedTreeDistribution(TypedDistribution<Tree> *base_dist, const RelativeNodeAgeConstraints &c) : TypedDistribution<Tree>( new Tree() ),
    base_distribution( base_dist ),
    constraints( c ),
    nodeAges(),
    constrainedNodes(),
    owns_tree( false )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    
    // add the parameters of the distribution
    const std::vector<const DagNode*>& pars = base_distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = pars.begin(); it != pars.end(); ++it)
    {
        this->addParameter( *it );
    }
    
    // if we own the tree, then we initialize our value with a true clone of the value of the base distribution
    if ( owns_tree == true )
    {
        value = base_distribution->getValue().clone();
    }
    else // otherwise we just set our pointer to the same pointer to the value of the base distribution
    {
        value = &base_distribution->getValue();
    }
    
    updateSetOfConstrainedNodes();
}


NodeOrderConstrainedTreeDistribution::NodeOrderConstrainedTreeDistribution(const NodeOrderConstrainedTreeDistribution &d) : TypedDistribution<Tree>( d ),
    base_distribution( d.base_distribution->clone() ),
    constraints( d.constraints ),
    nodeAges( d.nodeAges ),
    constrainedNodes( d.constrainedNodes )
{
    // the copy constructor of the TypedDistribution creates a new copy of the value
    // however, here we want to hold exactly the same value as the base-distribution
    // thus, we delete the newly created value
    delete value;
    
    
    // and then set it to the value of the base distribution
    if ( owns_tree == true )
    {
        // if we own the tree, then we set it to a true copy
        value = base_distribution->getValue().clone();
    }
    else
    {
        // otherwise we simply use the same pointer
        value = &base_distribution->getValue();
    }

}



NodeOrderConstrainedTreeDistribution::~NodeOrderConstrainedTreeDistribution()
{
    
    delete base_distribution;
    
    // DO NOT DELETE THE VALUE
    // the base distribution is the actual owner of the value!!!
    // we simply avoid the deletion of the value by setting its pointer to NULL
    // our base class, the TypedDistribution thinks that it owns the value and thus deletes it
    if ( owns_tree == false )
    {
        value = NULL;
    }
    
}


NodeOrderConstrainedTreeDistribution* NodeOrderConstrainedTreeDistribution::clone( void ) const
{
    
    return new NodeOrderConstrainedTreeDistribution( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double NodeOrderConstrainedTreeDistribution::computeLnProbability( void )
{
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() )
    {
        return RbConstants::Double::neginf;
    }
    
    // since we and the base distribution own the same value,
    // we do not need to set the value of the base distribution
    if ( owns_tree == true )
    {
        base_distribution->setValue( value->clone() );
    }

    double lnProb = base_distribution->computeLnProbability();
    
    return lnProb;
}


/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool NodeOrderConstrainedTreeDistribution::matchesConstraints( void )
{
    
    updateMapOfNodeAges();
    
    std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > > constra = constraints.getConstraints();
    
    for (size_t i = 0; i < constra.size() ; ++i) {
        constrainedNodes.insert(constra[i].first);
        constrainedNodes.insert(constra[i].second);
    }

    for (size_t i = 0; i < constra.size() ; ++i) {
        if ( nodeAges.at(constra[i].first) <  nodeAges.at(constra[i].second) ) {
            return false;
        }
    }
    return true;

}


void NodeOrderConstrainedTreeDistribution::updateSetOfConstrainedNodes()
{
    std::vector <std::pair < std::pair<std::string, std::string>, std::pair<std::string, std::string> > > constra = constraints.getConstraints();
    for (size_t i = 0; i < constra.size() ; ++i) {
        constrainedNodes.insert(constra[i].first);
        constrainedNodes.insert(constra[i].second);
    }
    return;
}


//Here we compute node ages from the current tree.
void NodeOrderConstrainedTreeDistribution::updateMapOfNodeAges()
{
    
    nodeAges.clear();
    for (std::set< std::pair < std::string, std::string > >::iterator elem=constrainedNodes.begin(); elem != constrainedNodes.end(); ++elem)
    {
        nodeAges[(*elem)] = TreeUtilities::getAgeOfMRCA(*value, elem->first, elem->second);
    }
    
    
    //There must be a smart and efficient way of doing that.
    //For the moment we do it dumb and slow.
    
    /*Attempt at a smart algorithm
     std::unordered_set<std::string> tipsToCareAbout;
     for (const auto& elem: constrainedNodes) {
     nodeAges[elem] = -1.0;
     tipsToCareAbout.insert(elem.first);
     tipsToCareAbout.insert(elem.second);
     }
     //First find all tip nodes of interest
     std::map < TopologyNode*, bool > nodesOfInterest;
     for (size_t i = 0; i < tree->getValue().getNumberOfTips()-1; ++i)
     {
     TopologyNode *node = n[i];
     if (tipsToCareAbout.contains( node->getName()) ) {
     nodesOfInterest[node] = true;
     }
     }
     //Now for internal nodes
     for (size_t i = tree->getValue().getNumberOfTips(); i < n.size(); ++i)
     {
     TopologyNode *node = n[i];
     std::vector< TopologyNode* > kids = node->getChildren();
     bool doneOne = false;
     for (size_t j = 0; j< kids.size() ++j) {
     if ( nodesOfInterest[kids[j]] )
     {
     nodesOfInterest[node] = true;
     if (doneOne)
     {
     //Two children of interest, it may be a node whose age we'd like to know
     //...
     }
     else {
     doneOne = true;
     }
     }
     }
     
     }
     */
    
    return;
    
}



/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void NodeOrderConstrainedTreeDistribution::redrawValue( void )
{
    
    base_distribution->redrawValue();
    // if we own the tree, then we need to free the memory before we create a new random variable
    if ( owns_tree == true )
    {
        delete value;
        value = base_distribution->getValue().clone();
    }
    else
    {
        // if we don't own the tree, then we just replace the current pointer with the pointer
        // to the new value of the base distribution
        value = &base_distribution->getValue();
    }
    
    
}


/**
 * Set the current value.
 */
void NodeOrderConstrainedTreeDistribution::setValue(Tree *v, bool f )
{
    
    if ( owns_tree == true )
    {
        TypedDistribution<Tree>::setValue(v, f);
        
        // if we own the tree then we simply initialize the base distribution with a clone
        base_distribution->setValue(v->clone(), f);
    }
    else
    {
        // otherwise we set our value to the same value as the base distribution
        // but first we need to make sure that our base class doesn't delete the value
        value = NULL;
        
        // and the we can set it for both ourselves and the base distribution
        TypedDistribution<Tree>::setValue(v, f);
        base_distribution->setValue(v, f);
    }
    
    updateSetOfConstrainedNodes();

    
    //    if ( rootAge != NULL )
    //    {
    //        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(rootAge);
    //        if ( stoch_root_age != NULL )
    //        {
    //            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
    //        }
    //        else
    //        {
    //            //            double factor = rootAge->getValue() / value->getRoot().getAge();
    //            //            TreeUtilities::rescaleTree( value, &value->getRoot(), factor);
    //
    //            value->getRoot().setAge( rootAge->getValue() );
    //        }
    //
    //    }
    
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void NodeOrderConstrainedTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    
    base_distribution->swapParameter(oldP,newP);
    
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void NodeOrderConstrainedTreeDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    base_distribution->touch(affecter, touchAll);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );
}

void NodeOrderConstrainedTreeDistribution::keepSpecialization(DagNode *affecter)
{
    base_distribution->keep(affecter);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );
}

void NodeOrderConstrainedTreeDistribution::restoreSpecialization(DagNode *restorer)
{
    base_distribution->restore(restorer);
    double a = base_distribution->getValue().getRoot().getAge();
    value->getRoot().setAge( a );
    
}