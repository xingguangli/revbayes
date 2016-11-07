#ifndef AbstractPhyloCTMCSiteHomogeneous_H
#define AbstractPhyloCTMCSiteHomogeneous_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "ConstantNode.h"
#include "DiscreteTaxonData.h"
#include "DnaState.h"
#include "MemberObject.h"
#include "RbMathLogic.h"
#include "RbSettings.h"
#include "RbVector.h"
#include "RateGenerator.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

#include <memory.h>

namespace RevBayesCore {
const size_t numActiveLikelihoods = 2; // compile-time constant, but should be runtime...

    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     * This file contains the distribution class for a character state evolving along a tree.
     * This abstract base class can be derived for any character evolution model with homogeneous mixture sites. A
     * homogeneous mixture model over sites is a model where all sites are drawn from the same distribution and the
     * specific instance of the per site parameter is integrated over. The per site parameter could be a rate scaler (e.g. the + gamma models)
     * or different rate matrices or anything else.
     *
     * The pruning algorithm is implemented in this base class and calles some few pure virtual methods.
     * The important functions you have to override are:
     * - computeRootLikelihood(size_t root, size_t l, size_t r, size_t m)
     * - computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r)
     * - computeTipLikelihood(const TopologyNode &node, size_t nIdx)
     * - getRootFrequencies()
     * - updateTransitionProbabilities()
     *
     *
     * The data are stored for convenience in this class in a matrix (std::vector<std::vector< unsigned > >) and can
     * be compressed.
     *
     * The partial likelihoods are stored in a c-style array called partialLikelihoods. The dimension are
     * partialLikelihoods[active][node_index][siteRateIndex][siteIndex][charIndex], however, since this is a one-dimensional c-style array,
     * you have to access the partialLikelihoods via
     * partialLikelihoods[active*num_nodes*num_site_rates*pattern_block_size*num_chars +
     *                    node_index*num_site_rates*pattern_block_size*num_chars +
     *                    siteRateIndex*pattern_block_size*num_chars +
     *                    siteIndex*num_chars +
     *                    charIndex]
     * Since this is a bit complex, we have some offset variables for convenience:
     * activeLikelihoodOffset      =  num_nodes*num_site_rates*pattern_block_size*num_chars;
     * nodeOffset                  =  num_site_rates*pattern_block_size*num_chars;
     * mixtureOffset               =  pattern_block_size*num_chars;
     * siteOffset                  =  num_chars;
     * This gives the more convenient access via
     * partialLikelihoods[active*activeLikelihoodOffset + node_index*nodeOffset + siteRateIndex*mixtureOffset + siteIndex*siteOffset + charIndex]
     *
     * Our implementation of the partial likelihoods mean that we can store the partial likelihood of a node, but not for site rates.
     * We also use twice as much memory because we store the partial likelihood along each branch and not only for each internal node.
     * This gives us a speed improvement during MCMC proposal in the order of a factor 2.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-06-17, version 1.0
     */
    template<class charType>
    class AbstractPhyloCTMCSiteHomogeneous : public TypedDistribution< AbstractHomologousDiscreteCharacterData >, public MemberObject< RbVector<double> >, public TreeChangeEventListener {

    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<Tree> *t, size_t nChars, size_t nMix, bool c, size_t nSites, bool amb );
        AbstractPhyloCTMCSiteHomogeneous(const AbstractPhyloCTMCSiteHomogeneous &n);                                                                                          //!< Copy constructor
        virtual                                                            ~AbstractPhyloCTMCSiteHomogeneous(void);                                                              //!< Virtual destructor

        // public member functions
        // pure virtual
        virtual AbstractPhyloCTMCSiteHomogeneous*                           clone(void) const = 0;                                                                      //!< Create an independent clone

        // non-virtual
        void                                                                bootstrap(void);
        virtual double                                                      computeLnProbability(void);
		virtual std::vector<charType>										drawAncestralStatesForNode(const TopologyNode &n);
        virtual void                                                        drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates);
        void                                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;     //!< Map the member methods to internal function calls
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                                 //!< The tree has changed and we want to know which part.
        virtual void                                                        recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates);
        virtual void                                                        redrawValue(void);
        void                                                                reInitialized(void);
        void                                                                setMcmcMode(bool tf);                                                                       //!< Change the likelihood computation to or from MCMC mode.
        void                                                                setValue(AbstractHomologousDiscreteCharacterData *v, bool f=false);                                   //!< Set the current value, e.g. attach an observation (clamp)
        virtual void                                                        tipDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates);
        void																updateMarginalNodeLikelihoods(void);
        
        void                                                                setClockRate(const TypedDagNode< double > *r);
        void                                                                setClockRate(const TypedDagNode< RbVector< double > > *r);
        void                                                                setPInv(const TypedDagNode< double > *);
        void                                                                setRateMatrix(const TypedDagNode< RateGenerator > *rm);
        void                                                                setRateMatrix(const TypedDagNode< RbVector< RateGenerator > > *rm);
        void                                                                setRootFrequencies(const TypedDagNode< RbVector< double > > *f);
        void                                                                setSiteRates(const TypedDagNode< RbVector< double > > *r);
        void                                                                setUseMarginalLikelihoods(bool tf);
        
    protected:
        
        // helper method for this and derived classes
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        virtual void                                                        resizeLikelihoodVectors(void);
        virtual void                                                        setActivePIDSpecialized(size_t i, size_t n);                                                          //!< Set the number of processes for this distribution.

        virtual void                                                        updateTransitionProbabilities(size_t node_idx, double brlen);
        virtual std::vector<double>                                         getRootFrequencies(void) const;


        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter


        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);

        // pure virtual methods
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r) = 0;
        virtual void                                                        computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m) = 0;
        virtual void                                                        computeTipLikelihood(const TopologyNode &node, size_t nIdx) = 0;
        virtual void                                                        computeRootLikelihood( size_t root, size_t left, size_t right) = 0;
        virtual void                                                        computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle) = 0;

        // virtual methods that you may want to overwrite
        virtual void                                                        compress(void);
        virtual void                                                        computeMarginalNodeLikelihood(size_t node_idx, size_t parentIdx);
        virtual void                                                        computeMarginalRootLikelihood();
        virtual std::vector< std::vector< double > >*                       sumMarginalLikelihoods(size_t node_index);
        virtual void                                                        computeRootLikelihoods( std::vector< double > &rv ) const;
        virtual double                                                      sumRootLikelihood( void );
        virtual std::vector<size_t>                                         getIncludedSiteIndices();


        // members
        double                                                              lnProb;
        double                                                              storedLnProb;
        size_t                                                              num_nodes;
        size_t                                                              num_sites;
        const size_t                                                        num_chars;
        size_t                                                              num_site_rates;
        const TypedDagNode<Tree>*                                           tau;
        std::vector<TransitionProbabilityMatrix>                            transition_prob_matrices;

        // the likelihoods
        double*                                                             partialLikelihoods;
        std::vector<size_t>                                                 activeLikelihood;
		double*																marginalLikelihoods;
        std::vector<double>                                                 scalingFactors;

        std::vector< std::vector< std::vector<double> > >                   perNodeSiteLogScalingFactors;
        bool                                                                use_scaling;

        // the data
        std::vector<std::vector<RbBitSet> >                                 ambiguous_char_matrix;
        std::vector<std::vector<unsigned long> >                            char_matrix;
        std::vector<std::vector<bool> >                                     gap_matrix;
        std::vector<size_t>                                                 pattern_counts;
        std::vector<bool>                                                   site_invariant;
        std::vector<size_t>                                                 invariant_site_index;
        size_t                                                              num_patterns;
        bool                                                                compressed;
		std::vector<size_t>                                                 site_pattern;    // an array that keeps track of which pattern is used for each site

        // flags for likelihood recomputation
        bool                                                                touched;
        std::vector<bool>                                                   changed_nodes;
        std::vector<bool>                                                   dirty_nodes;

        // offsets for nodes
        size_t                                                              activeLikelihoodOffset;
        size_t                                                              nodeOffset;
        size_t                                                              mixtureOffset;
        size_t                                                              siteOffset;
//        size_t                                                              nodeOffsetMarginal;
//        size_t                                                              siteOffsetMarginal;
        size_t                                                              nodeIndex;


        // flags
        bool                                                                using_ambiguous_characters;
        bool                                                                treatUnknownAsGap;
        bool                                                                treatAmbiguousAsGaps;

        bool                                                                useMarginalLikelihoods;
        bool                                                                inMcmcMode;

        // members
        const TypedDagNode< double >*                                       homogeneous_clock_rate;
        const TypedDagNode< RbVector< double > >*                           heterogeneous_clock_rates;
        const TypedDagNode< RateGenerator >*                                homogeneous_rate_matrix;
        const TypedDagNode< RbVector< RateGenerator > >*                    heterogeneous_rate_matrices;
        const TypedDagNode< RbVector< double > >*                           root_frequencies;
        const TypedDagNode< RbVector< double > >*                           site_rates;
        const TypedDagNode< RbVector< double > >*                           site_rates_probs;
        const TypedDagNode< double >*                                       p_inv;


        // flags specifying which model variants we use
        bool                                                                branch_heterogeneous_clock_rates;
        bool                                                                branch_heterogeneous_substitution_matrices;
        bool                                                                rate_variation_across_sites;

        // MPI variables
        size_t                                                              pattern_block_start;
        size_t                                                              pattern_block_end;
        size_t                                                              pattern_block_size;

    private:

        // private methods
        void                                                                fillLikelihoodVector(const TopologyNode &n, size_t nIdx);
        void                                                                recursiveMarginalLikelihoodComputation(size_t nIdx);
        void                                                                scale(size_t i);
        void                                                                scale(size_t i, size_t l, size_t r);
        void                                                                scale(size_t i, size_t l, size_t r, size_t m);
        virtual void                                                        simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<bool> &inv, const std::vector<size_t> &perSiteRates);

    };

}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "HomologousDiscreteCharacterData.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix_JC.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>

#ifdef RB_MPI
#include <mpi.h>
#endif


template<class charType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::AbstractPhyloCTMCSiteHomogeneous(const TypedDagNode<Tree> *t, size_t nChars, size_t nMix, bool c, size_t nSites,  bool amb) :
    TypedDistribution< AbstractHomologousDiscreteCharacterData >(  NULL ),
    lnProb( 0.0 ),
    storedLnProb( 0.0 ),
    num_nodes( t->getValue().getNumberOfNodes() ),
    num_sites( nSites ),
    num_chars( nChars ),
    num_site_rates( nMix ),
    tau( t ),
    transition_prob_matrices( std::vector<TransitionProbabilityMatrix>(num_site_rates, TransitionProbabilityMatrix(num_chars) ) ),
//    partialLikelihoods( new double[2*num_nodes*num_site_rates*num_sites*num_chars] ),
    partialLikelihoods( NULL ),
    activeLikelihood( std::vector<size_t>(num_nodes, 0) ),
//    marginalLikelihoods( new double[num_nodes*num_site_rates*num_sites*num_chars] ),
    marginalLikelihoods( NULL ),
    perNodeSiteLogScalingFactors( std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(num_nodes, std::vector<double>(num_sites, 0.0) ) ) ),
    use_scaling( true ),
    ambiguous_char_matrix(),
    char_matrix(),
    gap_matrix(),
    pattern_counts(),
    site_invariant( num_sites, false ),
    invariant_site_index( num_sites, 0 ),
    num_patterns( num_sites ),
    compressed( c ),
    nodeIndex( 0 ),
    site_pattern( std::vector<size_t>(num_sites, 0) ),
    touched( false ),
    changed_nodes( std::vector<bool>(num_nodes, false) ),
    dirty_nodes( std::vector<bool>(num_nodes, true) ),
    using_ambiguous_characters( amb ),
    treatUnknownAsGap( true ),
    treatAmbiguousAsGaps( false ),
    useMarginalLikelihoods( false ),
    inMcmcMode( false ),
    pattern_block_start( 0 ),
    pattern_block_end( num_patterns ),
    pattern_block_size( num_patterns )
{
    const std::vector<double> siteSizedVec(num_sites, 0.0);
    const std::vector<std::vector<double> > perNode(num_nodes*2, siteSizedVec);
    perNodeSiteLogScalingFactors.resize(2);
    perNodeSiteLogScalingFactors[0] = perNode;
    perNodeSiteLogScalingFactors[1] = perNode;


    // initialize with default parameters
    homogeneous_clock_rate        = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneous_clock_rates     = NULL;
    homogeneous_rate_matrix       = new ConstantNode<RateGenerator>("rateMatrix", new RateMatrix_JC( num_chars ) );
    heterogeneous_rate_matrices   = NULL;
    root_frequencies             = NULL;
    site_rates                   = NULL;
    site_rates_probs              = NULL;
    p_inv                        = new ConstantNode<double>("p_inv", new double(0.0) );

    // flags specifying which model variants we use
    branch_heterogeneous_clock_rates               = false;
    branch_heterogeneous_substitution_matrices     = false;
    rate_variation_across_sites                    = false;


    tau->getValue().getTreeChangeEventHandler().addListener( this );

    activeLikelihoodOffset      =  num_nodes*num_site_rates*pattern_block_size*num_chars;
    nodeOffset                  =  num_site_rates*pattern_block_size*num_chars;
    mixtureOffset               =  pattern_block_size*num_chars;
    siteOffset                  =  num_chars;


    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( tau );
    this->addParameter( homogeneous_clock_rate );
    this->addParameter( heterogeneous_clock_rates );
    this->addParameter( homogeneous_rate_matrix );
    this->addParameter( heterogeneous_rate_matrices );
    this->addParameter( root_frequencies );
    this->addParameter( site_rates );
    this->addParameter( site_rates_probs );
    this->addParameter( p_inv );

    // initially we use only a single processor until someone else tells us otherwise
    this->setActivePID( this->pid, 1 );

}


template<class charType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::AbstractPhyloCTMCSiteHomogeneous(const AbstractPhyloCTMCSiteHomogeneous &n) :
    TypedDistribution< AbstractHomologousDiscreteCharacterData >( n ),
    lnProb( n.lnProb ),
    storedLnProb( n.storedLnProb ),
    num_nodes( n.num_nodes ),
    num_sites( n.num_sites ),
    num_chars( n.num_chars ),
    num_site_rates( n.num_site_rates ),
    tau( n.tau ),
    transition_prob_matrices( n.transition_prob_matrices ),
//    partialLikelihoods( new double[2*num_nodes*num_site_rates*num_sites*num_chars] ),
    partialLikelihoods( NULL ),
    activeLikelihood( n.activeLikelihood ),
//    marginalLikelihoods( new double[num_nodes*num_site_rates*num_sites*num_chars] ),
    marginalLikelihoods( NULL ),
    perNodeSiteLogScalingFactors( n.perNodeSiteLogScalingFactors ),
    use_scaling( n.use_scaling ),
    ambiguous_char_matrix( n.ambiguous_char_matrix ),
    char_matrix( n.char_matrix ),
    gap_matrix( n.gap_matrix ),
    pattern_counts( n.pattern_counts ),
    site_invariant( n.site_invariant ),
    invariant_site_index( n.invariant_site_index ),
    num_patterns( n.num_patterns ),
    compressed( n.compressed ),
    site_pattern( n.site_pattern ),
    touched( false ),
    changed_nodes( n.changed_nodes ),
    dirty_nodes( n.dirty_nodes ),
    using_ambiguous_characters( n.using_ambiguous_characters ),
    treatUnknownAsGap( n.treatUnknownAsGap ),
    treatAmbiguousAsGaps( n.treatAmbiguousAsGaps ),
    useMarginalLikelihoods( n.useMarginalLikelihoods ),
    inMcmcMode( n.inMcmcMode ),
    pattern_block_start( n.pattern_block_start ),
    pattern_block_end( n.pattern_block_end ),
    pattern_block_size( n.pattern_block_size )
{

    // initialize with default parameters
    homogeneous_clock_rate        = n.homogeneous_clock_rate;
    heterogeneous_clock_rates     = n.heterogeneous_clock_rates;
    homogeneous_rate_matrix       = n.homogeneous_rate_matrix;
    heterogeneous_rate_matrices   = n.heterogeneous_rate_matrices;
    root_frequencies             = n.root_frequencies;
    site_rates                   = n.site_rates;
    site_rates_probs              = n.site_rates_probs;
    p_inv                        = n.p_inv;


    // flags specifying which model variants we use
    branch_heterogeneous_clock_rates               = n.branch_heterogeneous_clock_rates;
    branch_heterogeneous_substitution_matrices     = n.branch_heterogeneous_substitution_matrices;
    rate_variation_across_sites                    = n.rate_variation_across_sites;

    tau->getValue().getTreeChangeEventHandler().addListener( this );


    // copy the partial likelihoods if necessary
    if ( inMcmcMode == true )
    {
        partialLikelihoods = new double[2*num_nodes*num_site_rates*pattern_block_size*num_chars];
        memcpy(partialLikelihoods, n.partialLikelihoods, 2*num_nodes*num_site_rates*pattern_block_size*num_chars*sizeof(double));
    }

    // copy the marginal likelihoods if necessary
    if ( useMarginalLikelihoods == true )
    {
        marginalLikelihoods = new double[num_nodes*num_site_rates*pattern_block_size*num_chars];
        memcpy(marginalLikelihoods, n.marginalLikelihoods, num_nodes*num_site_rates*pattern_block_size*num_chars*sizeof(double));
    }
    activeLikelihoodOffset      =  num_nodes*num_site_rates*pattern_block_size*num_chars;
    nodeOffset                  =  num_site_rates*pattern_block_size*num_chars;
    mixtureOffset               =  pattern_block_size*num_chars;
    siteOffset                  =  num_chars;

}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class charType>
RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::~AbstractPhyloCTMCSiteHomogeneous( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!

    // remove myself from the tree listeners
    if ( tau != NULL )
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }

    // free the partial likelihoods
    delete [] partialLikelihoods;
    delete [] marginalLikelihoods;
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::bootstrap( void )
{
    
    // first we re-compress the data
    compress();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    std::vector<size_t> bootstrapped_pattern_counts = std::vector<size_t>(num_patterns,0);

    for (size_t i = 0; i<num_sites; ++i)
    {
        double u = rng->uniform01() * num_sites;
        size_t pattern_index = 0;
        while ( u > double(pattern_counts[pattern_index]) )
        {
            u -= double(pattern_counts[pattern_index]);
            ++pattern_index;
        }
        
        ++bootstrapped_pattern_counts[pattern_index];
        
    }
    
    pattern_counts = bootstrapped_pattern_counts;
    
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::compress( void )
{

    // only if the value has been set
    if ( this->value == NULL )
    {
        return;
    }

    ambiguous_char_matrix.clear();
    char_matrix.clear();
    gap_matrix.clear();
    pattern_counts.clear();
    num_patterns = 0;

    // resize the matrices
    size_t tips = tau->getValue().getNumberOfTips();
    ambiguous_char_matrix.resize(tips);
    char_matrix.resize(tips);
    gap_matrix.resize(tips);
    
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices = getIncludedSiteIndices();
    size_t siteIndex = siteIndices.back() + 1;
    
    // test if there were additional sites that we did not use
    while ( siteIndex < this->value->getNumberOfCharacters() )
    {
        if ( !this->value->isCharacterExcluded(siteIndex)  )
        {
            throw RbException( "The character matrix cannot set to this variable because it has too many included characters." );
        }
        siteIndex++;
    }
    
    // check whether there are ambiguous characters (besides gaps)
    bool ambiguousCharacters = false;
    // find the unique site patterns and compute their respective frequencies
    std::vector<TopologyNode*> nodes = tau->getValue().getNodes();
    for (size_t site = 0; site < num_sites; ++site)
    {

        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );
                DiscreteCharacterState &c = taxon.getCharacter(siteIndices[site]);

                // if we treat unknown characters as gaps and this is an unknown character then we change it
                // because we might then have a pattern more
                if ( treatAmbiguousAsGaps && (c.isAmbiguous() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( treatUnknownAsGap && (c.getNumberOfStates() == c.getNumberObservedStates() || c.isMissingState()) )
                {
                    c.setGapState( true );
                }
                else if ( !c.isGapState() && (c.isAmbiguous() || c.isMissingState()) )
                {
                    ambiguousCharacters = true;
                    break;
                }
            }
        }

        // break the loop if there was an ambiguous character
        if ( ambiguousCharacters )
        {
            break;
        }
    }

    // set the global variable if we use ambiguous characters
    using_ambiguous_characters = ambiguousCharacters;

    std::vector<bool> unique(num_sites, true);
    std::vector<size_t> indexOfSitePattern;

    // compress the character matrix if we're asked to
    if ( compressed == true )
    {
        // find the unique site patterns and compute their respective frequencies
        std::map<std::string,size_t> patterns;
        for (size_t site = 0; site < num_sites; ++site)
        {
            // create the site pattern
            std::string pattern = "";
            for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
            {
                if ( (*it)->isTip() )
                {
                    AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );
                    CharacterState &c = taxon.getCharacter(siteIndices[site]);
                    pattern += c.getStringValue();
                }
            }
            // check if we have already seen this site pattern
            std::map<std::string, size_t>::const_iterator index = patterns.find( pattern );
            if ( index != patterns.end() )
            {
                // we have already seen this pattern
                // increase the frequency counter
                pattern_counts[ index->second ]++;

                // obviously this site isn't unique nor the first encounter
                unique[site] = false;

				// remember which pattern this site uses
				site_pattern[site] = index->second;
            }
            else
            {
                // create a new pattern frequency counter for this pattern
                pattern_counts.push_back(1);

                // insert this pattern with the corresponding index in the map
                patterns.insert( std::pair<std::string,size_t>(pattern,num_patterns) );

				// remember which pattern this site uses
				site_pattern[site] = num_patterns;

                // increase the pattern counter
                num_patterns++;

                // add the index of the site to our pattern-index vector
                indexOfSitePattern.push_back( site );

                // flag that this site is unique (or the first occurence of this pattern)
                unique[site] = true;
            }
        }
    }
    else
    {
        // we do not compress
        num_patterns = num_sites;
        pattern_counts = std::vector<size_t>(num_sites,1);
        for(size_t i = 0; i < this->num_sites; i++)
		{
			indexOfSitePattern[i] = i;
		}
    }


    // compute which block of the data this process needs to compute
    pattern_block_start = size_t(floor( (double(pid-active_PID)   / num_processes ) * num_patterns) );
    pattern_block_end   = size_t(floor( (double(pid+1-active_PID) / num_processes ) * num_patterns) );
    pattern_block_size  = pattern_block_end - pattern_block_start;


    std::vector<size_t> process_pattern_counts = std::vector<size_t>(pattern_block_size,0);
    // allocate and fill the cells of the matrices
    for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if ( (*it)->isTip() )
        {
            size_t node_index = (*it)->getIndex();
            AbstractDiscreteTaxonData& taxon = value->getTaxonData( (*it)->getName() );

            // resize the column
            ambiguous_char_matrix[node_index].resize(pattern_block_size);
            char_matrix[node_index].resize(pattern_block_size);
            gap_matrix[node_index].resize(pattern_block_size);
            for (size_t patternIndex = 0; patternIndex < pattern_block_size; ++patternIndex)
            {
                // set the counts for this patter
                process_pattern_counts[patternIndex] = pattern_counts[patternIndex+pattern_block_start];
                
                charType &c = static_cast<charType &>( taxon.getCharacter(siteIndices[indexOfSitePattern[patternIndex+pattern_block_start]]) );
                gap_matrix[node_index][patternIndex] = c.isGapState();

                if ( using_ambiguous_characters == true )
                {
                    // we use the actual state
                    ambiguous_char_matrix[node_index][patternIndex] = c.getState();
                }
                else if ( c.isGapState() == false )
                {
                    // we use the index of the state
                    char_matrix[node_index][patternIndex] = c.getStateIndex();
                }
                else
                {
                    // just to be safe
                    char_matrix[node_index][patternIndex] = -1;
                }
                
            }
            
        }
        
    }
    
    // now copy back the pattern count vector
    pattern_counts = process_pattern_counts;

    // reset the vector if a site is invariant
    site_invariant.resize( pattern_block_size );
    invariant_site_index.resize( pattern_block_size );
    size_t length = char_matrix.size();
    for (size_t i=0; i<pattern_block_size; ++i)
    {
        bool inv = true;

        if ( using_ambiguous_characters == true )
        {
            const RbBitSet &val = ambiguous_char_matrix[0][i];

            invariant_site_index[i] = val.size();
            
            for (size_t j=1; j<length; ++j)
            {
                if ( val != ambiguous_char_matrix[j][i] || gap_matrix[j][i] == true )
                {
                    inv = false;
                    break;
                }
            }
        }
        else
        {
            unsigned long c = char_matrix[0][i];
            invariant_site_index[i] = c;
            
            for (size_t j=1; j<length; ++j)
            {
                if ( invariant_site_index[i] != char_matrix[j][i] || gap_matrix[j][i] == true )
                {
                    inv = false;
                    break;
                }
            }
        }

        site_invariant[i] = inv;
    }

    // finally we resize the partial likelihood vectors to the new pattern counts
    resizeLikelihoodVectors();

}


template<class charType>
double RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::computeLnProbability( void )
{

    // we need to check here if we still are listining to this tree for change events
    // the tree could have been replaced without telling us
    if ( tau->getValue().getTreeChangeEventHandler().isListening( this ) == false )
    {
        tau->getValue().getTreeChangeEventHandler().addListener( this );
        dirty_nodes = std::vector<bool>(num_nodes, true);
    }


    // if we are not in MCMC mode, then we need to (temporarily) allocate memory
    if ( inMcmcMode == false )
    {
        partialLikelihoods = new double[2*num_nodes*num_site_rates*pattern_block_size*num_chars];
    }

    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = tau->getValue().getRoot();

    // we start with the root and then traverse down the tree
    size_t root_index = root.getIndex();

    // only necessary if the root is actually dirty
    if ( dirty_nodes[root_index] == true )
    {

        // start by filling the likelihood vector for the children of the root
        if ( root.getNumberOfChildren() == 2 ) // rooted trees have two children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t left_index = left.getIndex();
            fillLikelihoodVector( left, left_index );
            const TopologyNode &right = root.getChild(1);
            size_t right_index = right.getIndex();
            fillLikelihoodVector( right, right_index );

            computeRootLikelihood( root_index, left_index, right_index );
            scale(root_index, left_index, right_index);

        }
        else if ( root.getNumberOfChildren() == 3 ) // unrooted trees have three children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t left_index = left.getIndex();
            fillLikelihoodVector( left, left_index );
            const TopologyNode &right = root.getChild(1);
            size_t right_index = right.getIndex();
            fillLikelihoodVector( right, right_index );
            const TopologyNode &middle = root.getChild(2);
            size_t middleIndex = middle.getIndex();
            fillLikelihoodVector( middle, middleIndex );

            computeRootLikelihood( root_index, left_index, right_index, middleIndex );
            scale(root_index, left_index, right_index, middleIndex);

        }
        else
        {
            throw RbException("The root node has an unexpected number of children. Only 2 (for rooted trees) or 3 (for unrooted trees) are allowed.");
        }

        // sum the partials up
        this->lnProb = sumRootLikelihood();

    }

    // if we are not in MCMC mode, then we need to (temporarily) free memory
    if ( inMcmcMode == false )
    {
        // free the partial likelihoods
        delete [] partialLikelihoods;
        partialLikelihoods = NULL;
    }

    return this->lnProb;
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::computeMarginalNodeLikelihood( size_t node_index, size_t parentnode_index )
{

    // compute the transition probability matrix
    this->updateTransitionProbabilities( node_index, this->tau->getValue().getNode(node_index).getBranchLength() );

    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node                  = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    double*         p_node_marginal         = this->marginalLikelihoods + node_index*this->nodeOffset;
    const double*   p_parent_node_marginal  = this->marginalLikelihoods + parentnode_index*this->nodeOffset;

    // get pointers the likelihood for both subtrees
    const double*   p_mixture                   = p_node;
    double*         p_mixture_marginal          = p_node_marginal;
    const double*   p_parent_mixture_marginal   = p_parent_node_marginal;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transition_prob_matrices[mixture].theMatrix;

        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture                  = p_mixture;
        double*         p_site_mixture_marginal         = p_mixture_marginal;
        const double*   p_parent_site_mixture_marginal  = p_parent_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j                    = p_site_mixture;
            double*         p_site_marginal_j           = p_site_mixture_marginal;
            // iterate over all end states
            for (size_t j=0; j<num_chars; ++j)
            {
                const double*   p_parent_site_marginal_k    = p_parent_site_mixture_marginal;
                *p_site_marginal_j = 0.0;

                // iterator over all start states
                for (size_t k=0; k<num_chars; ++k)
                {
                    // transition probability for k->j
                    const double tp_kj = *p_parent_site_marginal_k * tp_begin[ k * num_chars + j ];

                    // add the probability of starting from this state
                    *p_site_marginal_j += *p_site_j * tp_kj;

                    // next parent state
                    ++p_parent_site_marginal_k;
                }

                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }

            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_marginal+=this->siteOffset; p_parent_site_mixture_marginal+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_marginal+=this->mixtureOffset; p_parent_mixture_marginal+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::computeMarginalRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();

    // get the index of the root node
    size_t node_index = root.getIndex();

    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();

    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node           = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    double*         p_node_marginal  = this->marginalLikelihoods + node_index*this->nodeOffset;

    // get pointers the likelihood for both subtrees
    const double*   p_mixture           = p_node;
    double*         p_mixture_marginal  = p_node_marginal;

    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {

        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture          = p_mixture;
        double*         p_site_mixture_marginal = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j            = p_site_mixture;
            double*         p_site_marginal_j   = p_site_mixture_marginal;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                *p_site_marginal_j = *p_site_j * *f_j;

                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }

            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_marginal+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_marginal+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

}


/**
 * Draw a vector of ancestral states from the marginal distribution (non-conditional of the other ancestral states).
 * Here we assume that the marginal likelihoods have been updated.
 */
template<class charType>
std::vector<charType> RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::drawAncestralStatesForNode(const TopologyNode &node)
{

	size_t node_index = node.getIndex();

	// get the marginal likelihoods
    std::vector< std::vector<double> >* marginals = sumMarginalLikelihoods(node_index);

	RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector< charType > ancestralSeq = std::vector<charType>();

    for ( size_t i = 0; i < num_sites; ++i )
    {
		size_t pattern = i;
		// if the matrix is compressed use the pattern for this site
		if (compressed)
        {
			pattern = site_pattern[i];
		}

        // create the character
        charType c = charType( num_chars );
        c.setToFirstState();

		// sum the likelihoods for each character state
		const std::vector<double> siteMarginals = (*marginals)[pattern];
		double sumMarginals = 0.0;
		for (int j = 0; j < siteMarginals.size(); j++)
        {
			sumMarginals += siteMarginals[j];
		}

		double u = rng->uniform01();
		if (sumMarginals == 0.0)
        {

			// randomly draw state if all states have 0 probability
			c.setStateByIndex((size_t)(u*c.getNumberOfStates()));

		}
        else
        {

			// the marginals don't add up to 1, so rescale u
			u *= sumMarginals;

			// draw the character state
			size_t stateIndex = 0;
			while ( true )
            {

				u -= siteMarginals[stateIndex];

				if ( u > 0.0 )
                {

					c++;
					stateIndex++;

					if ( stateIndex == c.getNumberOfStates() )
                    {
						stateIndex = 0;
						c.setToFirstState();
					}

				}
                else
                {
					break;
				}
			}
		}

        // add the character to the sequence
        ancestralSeq.push_back( c );
    }

    // we need to free the vector
    delete marginals;

	return ancestralSeq;
}


/**
 * Draw a vector of ancestral states from the joint-conditional distribution of states.
 */
template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates)
{

    RandomNumberGenerator* rng = GLOBAL_RNG;

    // get working variables
    const std::vector<double> &f = this->getRootFrequencies();
    std::vector<double> siteProbVector(1,1.0);
    if (site_rates_probs != NULL)
        siteProbVector = site_rates_probs->getValue();


    const TopologyNode &root = tau->getValue().getRoot();
    size_t node_index = root.getIndex();
    size_t right = root.getChild(0).getIndex();
    size_t left = root.getChild(1).getIndex();

    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;

    // get pointers the likelihood for both subtrees
    const double*   p_site           = p_node;
    const double*   p_left_site      = p_left;
    const double*   p_right_site     = p_right;


    // sample root states
    std::vector<double> p( this->num_site_rates*this->num_chars, 0.0);
    std::vector<size_t> sampledSiteRates(this->num_sites,0);
    for (size_t i = 0; i < this->num_sites; i++)
    {

        // create the character
        charType c = charType( num_chars );
        c.setToFirstState();

        // sum to sample
        double sum = 0.0;

		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if ( compressed == true )
        {
			pattern = site_pattern[i];
		}

        // get ptr to first mixture cat for site
        p_site          = p_node  + pattern * this->siteOffset;
        p_left_site     = p_left  + pattern * this->siteOffset;
        p_right_site    = p_right + pattern * this->siteOffset;

        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
        {
            // get pointers to the likelihood for this mixture category
            const double* p_site_mixture_j       = p_site;
            const double* p_left_site_mixture_j  = p_left_site;
            const double* p_right_site_mixture_j = p_right_site;

            // iterate over all starting states
            for (size_t state = 0; state < this->num_chars; ++state)
            {
                size_t k = this->num_chars*mixture + state;
                p[k] = *p_site_mixture_j * *p_left_site_mixture_j * *p_right_site_mixture_j * f[state] * siteProbVector[mixture];
                sum += p[k];

                // increment the pointers to the next state for (site,rate)
                p_site_mixture_j++;
                p_left_site_mixture_j++;
                p_right_site_mixture_j++;
            }

            // increment the pointers to the next mixture category for given site
            p_site       += this->mixtureOffset;
            p_left_site  += this->mixtureOffset;
            p_right_site += this->mixtureOffset;

        } // end-for over all mixtures (=rate categories)

        // sample char from p
        bool stop = false;
        double u = rng->uniform01() * sum;
        for (size_t mixture = 0; mixture < this->num_site_rates; mixture++)
        {
            c.setToFirstState();
            for (size_t state = 0; state < this->num_chars; state++)
            {
                size_t k = this->num_chars * mixture + state;
                u -= p[k];
                if (u < 0.0)
                {
                    startStates[root.getIndex()][i] = c;
                    sampledSiteRates[i] = mixture;
                    stop = true;
                    break;
                }
                c++;
            }
            if (stop) break;
        }

        endStates[node_index][i] = startStates[node_index][i];
    }

    // recurse
    std::vector<TopologyNode*> children = root.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        // daughters identically inherit ancestral state
        startStates[ children[i]->getIndex() ] = endStates[ root.getIndex() ];

        // recurse towards tips
        if ( children[i]->isTip() == false )
        {
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        }
        else
        {
            tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        }
        
    }
}

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<double> &rv) const
{
    
    if ( n == "siteLikelihoods" )
    {
        
        // make sure the likelihoods are updated
        const_cast<AbstractPhyloCTMCSiteHomogeneous<charType> *>( this )->computeLnProbability();
        
        // get the per site likelihood
        std::vector<double> tmp = std::vector<double>(num_patterns, 0.0);
        computeRootLikelihoods( tmp );
        
        // now match it back to the actual sites
        rv = RbVector<double>(num_sites, 0.0);
        for (size_t i=0; i<num_sites; ++i)
        {
            size_t pattern_index = site_pattern[i];
            rv[i] = tmp[pattern_index] / pattern_counts[pattern_index];
        }
        
    }
    else
    {
        throw RbException("The PhyloCTMC process does not have a member method called '" + n + "'.");
    }
    
}




template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates)
{
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // get working variables
    size_t node_index = node.getIndex();
    size_t left = node.getChild(0).getIndex();
    size_t right = node.getChild(1).getIndex();
//    size_t parentIndex = node.getParent().getIndex();

    // get transition probabilities
    this->updateTransitionProbabilities( node_index, node.getBranchLength() );

    // get the pointers to the partial likelihoods and the marginal likelihoods
//    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;

    // get pointers the likelihood for both subtrees
//    const double*   p_site           = p_node;
//    const double*   p_left_site      = p_left;
//    const double*   p_right_site     = p_right;

    // sample characters conditioned on start states, going to end states
    std::vector<double> p(this->num_chars, 0.0);
    for (size_t i = 0; i < this->num_sites; i++)
    {
        size_t cat = sampledSiteRates[i];
        size_t k = startStates[node_index][i].getStateIndex();


        // sum to sample
        double sum = 0.0;

		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if ( compressed == true )
        {
			pattern = site_pattern[i];
		}

        // get ptr to first mixture cat for site
//        p_site          = p_node  + cat * this->mixtureOffset + pattern * this->siteOffset;
        const double* p_left_site_mixture_j     = p_left  + cat * this->mixtureOffset + pattern * this->siteOffset;
        const double* p_right_site_mixture_j    = p_right + cat * this->mixtureOffset + pattern * this->siteOffset;

        // iterate over possible end states for each site given start state
        for (size_t j = 0; j < this->num_chars; j++)
        {
            double tp_kj = this->transition_prob_matrices[cat][k][j];
            p[j] = tp_kj * *p_left_site_mixture_j * *p_right_site_mixture_j;
            sum += p[j];

//            p_site_mixture_j++;
            p_left_site_mixture_j++;
            p_right_site_mixture_j++;
        }

        // sample char from p
        charType c = charType( num_chars );
        c.setToFirstState();
        double u = rng->uniform01() * sum;
        for (size_t state = 0; state < this->num_chars; state++)
        {
            u -= p[state];
            if (u < 0.0)
            {
                endStates[node_index][i] = c;
                break;
            }
            c++;
        }
    }

    // recurse
    std::vector<TopologyNode*> children = node.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        // daughters identically inherit ancestral state
        startStates[ children[i]->getIndex() ] = endStates[ node.getIndex() ];

        // recurse towards tips
        if (!children[i]->isTip())
        {
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        }
        else
        {
            tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        }

    }

}

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::tipDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates)
{

    // get working variables
    size_t node_index = node.getIndex();
//    const std::vector<unsigned long> &char_node = this->char_matrix[node_index];

    // get transition probabilities
    this->updateTransitionProbabilities( node_index, node.getBranchLength() );

    const AbstractHomologousDiscreteCharacterData& d = this->getValue();
    const HomologousDiscreteCharacterData<charType>& dd = static_cast<const HomologousDiscreteCharacterData<charType>& >( d );
    const DiscreteTaxonData<charType>& td = dd.getTaxonData( node.getName() );

    // ideally sample ambiguous tip states given the underlying process and ancestral state
    // for now, always sample the clamped character
    std::vector<double> p(this->num_chars, 0.0);
    for (size_t i = 0; i < this->num_sites; i++)
    {
        charType c = td.getCharacter(i);
        endStates[node_index][i] = c;
    }

    // no further recursion

}

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::fillLikelihoodVector(const TopologyNode &node, size_t node_index)
{

    // check for recomputation
    if ( dirty_nodes[node_index] == true )
    {
        // mark as computed
        dirty_nodes[node_index] = false;

        if ( node.isTip() == true )
        {
            // this is a tip node
            // compute the likelihood for the tip and we are done
            computeTipLikelihood(node, node_index);
            
            // rescale likelihood vector
            scale(node_index);
        }
        else
        {
            // this is an internal node
            const TopologyNode     &left        = node.getChild(0);
            size_t                  left_index  = left.getIndex();
            fillLikelihoodVector( left, left_index );
            const TopologyNode     &right       = node.getChild(1);
            size_t                  right_index = right.getIndex();
            fillLikelihoodVector( right, right_index );

            // now compute the likelihoods of this internal node
            computeInternalNodeLikelihood(node,node_index,left_index,right_index);

            // rescale likelihood vector
        scale(node_index,left_index,right_index);
        }

    }

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n )
{

    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );

}


template<class charType>
std::vector<size_t> RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::getIncludedSiteIndices( void )
{
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices = std::vector<size_t>(num_sites,0);
    size_t siteIndex = 0;
    for (size_t i = 0; i < num_sites; ++i)
    {
        while ( this->value->isCharacterExcluded(siteIndex) )
        {
            siteIndex++;
            if ( siteIndex >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }
        
        siteIndices[i] = siteIndex;
        siteIndex++;
    }
    
    return siteIndices;
}



template<class charType>
std::vector<double> RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::getRootFrequencies( void ) const
{

    if ( branch_heterogeneous_substitution_matrices == true || root_frequencies != NULL )
    {
        return root_frequencies->getValue();
    }
    else
    {
        const RateMatrix *rm = dynamic_cast<const RateMatrix *>(&homogeneous_rate_matrix->getValue());
        if ( rm != NULL )
        {
            return rm->getStationaryFrequencies();
        }
        else
        {
            throw RbException("If you want to use RateGenerators that are not RateMatrices then you need to specify the root frequencies directly.");
        }

    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::keepSpecialization( DagNode* affecter )
{

    // reset flags for likelihood computation
    touched = false;

    // reset the ln probability
    this->storedLnProb = this->lnProb;

    // reset all flags
    for (std::vector<bool>::iterator it = this->dirty_nodes.begin(); it != this->dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }

    for (std::vector<bool>::iterator it = this->changed_nodes.begin(); it != this->changed_nodes.end(); ++it)
    {
        (*it) = false;
    }

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n ) {

    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();

    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( dirty_nodes[index] == false )
    {
        // the root doesn't have an ancestor
        if ( !n.isRoot() )
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }

        // set the flag
        dirty_nodes[index] = true;

        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( changed_nodes[index] == false )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            changed_nodes[index] = true;
        }

    }

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::recursiveMarginalLikelihoodComputation( size_t node_index )
{

    const TopologyNode &node = tau->getValue().getNode( node_index );

    for ( size_t i=0; i<node.getNumberOfChildren(); ++i )
    {
        const TopologyNode &child = node.getChild(i);

        if ( child.isTip() == false )
        {
            size_t childIndex = child.getIndex();
            computeMarginalNodeLikelihood( childIndex, node_index );
            recursiveMarginalLikelihoodComputation( childIndex );
        }

    }
}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::redrawValue( void )
{

    bool do_mask = this->dag_node != NULL && this->dag_node->isClamped();
    std::vector<std::vector<bool> > mask = std::vector<std::vector<bool> >(tau->getValue().getNumberOfTips(), std::vector<bool>());
    // we cannot use the stored gap matrix because it uses the pattern compression
    // therefore we create our own mask
    if ( do_mask == true )
    {
        // set the gap states as in the clamped data
        for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
        {
            // create a temporary variable for the taxon
            std::vector<bool> taxon_mask = std::vector<bool>(num_sites,false);
            
            const std::string &taxon_name = tau->getValue().getNode( i ).getName();
            AbstractDiscreteTaxonData& taxon = value->getTaxonData( taxon_name );
            
            for ( size_t site=0; site<num_sites; ++site)
            {
                taxon_mask[site] = taxon.getCharacter( site ).isGapState();
            }
            
            mask[i] = taxon_mask;
            
        }
        
    }
    
    // delete the old value first
    delete this->value;

    // create a new character data object
    this->value = new HomologousDiscreteCharacterData<charType>();

    // create a vector of taxon data
    std::vector< DiscreteTaxonData<charType> > taxa = std::vector< DiscreteTaxonData< charType > >( num_nodes, DiscreteTaxonData<charType>( Taxon("") ) );

    // first, simulate the per site rates
    RandomNumberGenerator* rng = GLOBAL_RNG;
    std::vector<size_t> perSiteRates = std::vector<size_t>(num_sites,0);
    std::vector<bool> inv = std::vector<bool>(num_sites,false);
    double prob_invariant = p_inv->getValue();
    for ( size_t i = 0; i < num_sites; ++i )
    {
        // draw if this site is invariant
        double u = rng->uniform01();
        if ( u < prob_invariant )
        {
            // this site is invariant
            inv[i] = true;

        }
        else if ( num_site_rates  > 1 )
        {
            // draw the rate for this site
            u = rng->uniform01();
            size_t rateIndex = size_t(u*num_site_rates);
            perSiteRates[i] = rateIndex;
            
        }
        else
        {
            // there is only a single site rate so this is 1.0
            perSiteRates[i] = 0;
            
        }
        
    }

    // simulate the root sequence
    const std::vector< double > &stationary_freqs = getRootFrequencies();
    DiscreteTaxonData< charType > &root = taxa[ tau->getValue().getRoot().getIndex() ];
    for ( size_t i = 0; i < num_sites; ++i )
    {
        // create the character
        charType c = charType( num_chars );
        c.setToFirstState();
        // draw the state
        double u = rng->uniform01();
        std::vector< double >::const_iterator freq = stationary_freqs.begin();
        while ( true )
        {
            u -= *freq;

            if ( u > 0.0 )
            {
                ++c;
                ++freq;
            }
            else
            {
                break;
            }

        }

        // add the character to the sequence
        root.addCharacter( c );
    }
    // recursively simulate the sequences
    root.setTaxon( Taxon("Root") );

    // recursively simulate the sequences
    simulate( tau->getValue().getRoot(), taxa, inv, perSiteRates );

    // add the taxon data to the character data
//    for (size_t i = 0; i < tau->getValue().getNumberOfNodes(); ++i)
    for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
    {
        this->value->addTaxonData( taxa[i] );
    }
    
    if ( do_mask == true )
    {
        // set the gap states as in the clamped data
        for (size_t i = 0; i < tau->getValue().getNumberOfTips(); ++i)
        {
            const std::string &taxon_name = tau->getValue().getNode( i ).getName();
            AbstractDiscreteTaxonData& taxon = value->getTaxonData( taxon_name );
            
            for ( size_t site=0; site<num_sites; ++site)
            {
                DiscreteCharacterState &c = taxon.getCharacter(site);
                if ( mask[i][site] == true )
                {
                    c.setGapState( true );
                }
            }
            
        }
        
    }

    // compress the data and initialize internal variables
    compress();

    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    {
        (*it) = true;
    }

    // flip the active likelihood pointers
    for (size_t index = 0; index < changed_nodes.size(); ++index)
    {
        if ( changed_nodes[index] == false )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            changed_nodes[index] = true;
        }
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::reInitialized( void )
{

    // we need to recompress because the tree may have changed
    compress();
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::resizeLikelihoodVectors( void )
{

    // only do this if we are in MCMC mode. This will safe memory
    if ( inMcmcMode == true )
    {

        // we resize the partial likelihood vectors to the new dimensions
        delete [] partialLikelihoods;

        partialLikelihoods = new double[2*num_nodes*num_site_rates*pattern_block_size*num_chars];

        // reinitialize likelihood vectors
        for (size_t i = 0; i < 2*num_nodes*num_site_rates*pattern_block_size*num_chars; i++)
        {
            partialLikelihoods[i] = 0.0;
        }

    }

    if ( useMarginalLikelihoods == true )
    {
        // we resize the partial likelihood vectors to the new dimensions
        delete [] marginalLikelihoods;

        marginalLikelihoods = new double[num_nodes*num_site_rates*pattern_block_size*num_chars];
        
        // reinitialize likelihood vectors
        for (size_t i = 0; i < num_nodes*num_site_rates*pattern_block_size*num_chars; i++)
        {
            marginalLikelihoods[i] = 0.0;
        }

    }
    
    perNodeSiteLogScalingFactors = std::vector<std::vector< std::vector<double> > >(2, std::vector<std::vector<double> >(num_nodes, std::vector<double>(pattern_block_size, 0.0) ) );

    transition_prob_matrices = std::vector<TransitionProbabilityMatrix>(num_site_rates, TransitionProbabilityMatrix(num_chars) );

    // set the offsets for easier iteration through the likelihood vector
    activeLikelihoodOffset      =  num_nodes*num_site_rates*pattern_block_size*num_chars;
    nodeOffset                  =  num_site_rates*pattern_block_size*num_chars;
    mixtureOffset               =  pattern_block_size*num_chars;
    siteOffset                  =  num_chars;

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::restoreSpecialization( DagNode* affecter )
{

    // reset flags for likelihood computation
    touched = false;

    // reset the ln probability
    this->lnProb = this->storedLnProb;

    // reset the flags
    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }

    // restore the active likelihoods vector
    for (size_t index = 0; index < changed_nodes.size(); ++index)
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changed_nodes[index] == true )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }

        // set all flags to false
        changed_nodes[index] = false;
    }

}

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::scale( size_t node_index)
{

    double* p_node = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;

    if ( use_scaling == true && node_index % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {

            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }

            }

            this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] = -log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {
                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( use_scaling == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] = 0;
        }

    }
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::scale( size_t node_index, size_t left, size_t right )
{

    size_t ali = this->activeLikelihood.at(nodeIndex);
    size_t leftali = this->activeLikelihood.at(left);
    size_t rightali = this->activeLikelihood.at(right);
    double* p_node = this->partialLikelihoods + ali*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    std::vector< std::vector<double> > & activePNSLSF = this->perNodeSiteLogScalingFactors.at(ali);
    std::vector<double> & nodesActivePNSLSF = activePNSLSF.at(nodeIndex);
    const std::vector< std::vector<double> > & leftActivePNSLSF = this->perNodeSiteLogScalingFactors.at(leftali);
    const std::vector<double> & leftNodesActivePNSLSF = leftActivePNSLSF.at(leftali);
    const std::vector< std::vector<double> > & rightActivePNSLSF = this->perNodeSiteLogScalingFactors.at(rightali);
    const std::vector<double> & rightNodesActivePNSLSF = rightActivePNSLSF.at(rightali);

    if ( use_scaling == true && nodeIndex % 4 == 0 )
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;

    if ( use_scaling == true && node_index % RbSettings::userSettings().getScalingDensity() == 0 )
    {

        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {


            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {


                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;


                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {

                    if ( p_site_mixture[i] > max )
                    {

                        max = p_site_mixture[i];
                    }
                }

            }


            this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] - log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {

                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {

                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( use_scaling == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            nodesActivePNSLSF.at(site) = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site];

        }

    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::scale( size_t node_index, size_t left, size_t right, size_t middle )
{

    double* p_node   = this->partialLikelihoods + this->activeLikelihood[node_index]*this->activeLikelihoodOffset + node_index*this->nodeOffset;

    if ( use_scaling == true && node_index % RbSettings::userSettings().getScalingDensity() == 0 )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {

            // the max probability
            double max = 0.0;

            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {
                    if ( p_site_mixture[i] > max )
                    {
                        max = p_site_mixture[i];
                    }
                }

            }

            this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site] - log(max);


            // compute the per site probabilities
            for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
            {
                // get the pointers to the likelihood for this mixture category
                size_t offset = mixture*this->mixtureOffset + site*this->siteOffset;

                double*          p_site_mixture          = p_node + offset;

                for ( size_t i=0; i<this->num_chars; ++i)
                {
                    p_site_mixture[i] /= max;
                }

            }

        }
    }
    else if ( use_scaling == true )
    {
        // iterate over all mixture categories
        for (size_t site = 0; site < this->pattern_block_size ; ++site)
        {
            this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] = this->perNodeSiteLogScalingFactors[this->activeLikelihood[left]][left][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[right]][right][site] + this->perNodeSiteLogScalingFactors[this->activeLikelihood[middle]][middle][site];
        }

    }
}


/**
 * Set the active PID of this specific distribution object.
 */
template <class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setActivePIDSpecialized(size_t a, size_t n)
{
    
    // we need to recompress the data
    this->compress();
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setValue(AbstractHomologousDiscreteCharacterData *v, bool force)
{

    // delegate to the parent class
    TypedDistribution< AbstractHomologousDiscreteCharacterData >::setValue(v, force);

    // reset the number of sites
    this->num_sites = v->getNumberOfIncludedCharacters();

	site_pattern.clear();
	site_pattern.resize(num_sites);

    // now compress the data and resize the likelihood vectors
    this->compress();
    
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::simulate( const TopologyNode &node, std::vector< DiscreteTaxonData< charType > > &taxa, const std::vector<bool> &invariant, const std::vector<size_t> &perSiteRates)
{

    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();

    // get the sequence of this node
    size_t node_index = node.getIndex();
    const DiscreteTaxonData< charType > &parent = taxa[ node_index ];

    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);

        // update the transition probability matrix
        updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );

        DiscreteTaxonData< charType > &taxon = taxa[ child.getIndex() ];
        for ( size_t i = 0; i < num_sites; ++i )
        {
            
            if ( invariant[i] == true )
            {
                
                // add the character to the sequence
                taxon.addCharacter( parent.getCharacter( i ) );
            }
            else
            {
                // get the ancestral character for this site
                unsigned long parentState = parent.getCharacter( i ).getStateIndex();
                
                double *freqs = transition_prob_matrices[ perSiteRates[i] ][ parentState ];

                // create the character
                charType c = charType( num_chars );
                c.setToFirstState();
                // draw the state
                double u = rng->uniform01();
                size_t stateIndex = 0;
                while ( true )
                {
                    u -= *freqs;
                    ++stateIndex;

                    if ( u > 0.0 && stateIndex < this->num_chars)
                    {
                        ++c;
                        ++freqs;
                    }
                    else
                    {
                        break;
                    }

                }

                // add the character to the sequence
                taxon.addCharacter( c );
            }

        }
        
        if ( child.isTip() )
        {
            taxon.setTaxon( child.getTaxon() );
        }
        else
        {
            // recursively simulate the sequences
            std::stringstream ss;
            ss << "Node" << child.getIndex();
            taxon.setTaxon( Taxon(ss.str()) );
            simulate( child, taxa, invariant, perSiteRates );
        }

    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setClockRate(const TypedDagNode< double > *r)
{

    // remove the old parameter first
    if ( homogeneous_clock_rate != NULL )
    {
        this->removeParameter( homogeneous_clock_rate );
        homogeneous_clock_rate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneous_clock_rates );
        heterogeneous_clock_rates = NULL;
    }

    // set the value
    branch_heterogeneous_clock_rates = false;
    homogeneous_clock_rate = r;

    // add the new parameter
    this->addParameter( homogeneous_clock_rate );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setClockRate(const TypedDagNode< RbVector< double > > *r)
{

    // remove the old parameter first
    if ( homogeneous_clock_rate != NULL )
    {
        this->removeParameter( homogeneous_clock_rate );
        homogeneous_clock_rate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneous_clock_rates );
        heterogeneous_clock_rates = NULL;
    }

    // set the value
    branch_heterogeneous_clock_rates = true;
    heterogeneous_clock_rates = r;

    // add the new parameter
    this->addParameter( heterogeneous_clock_rates );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}


/**
 * Change the likelihood computation to or from MCMC mode.
 */

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setMcmcMode(bool tf)
{

    // free old memory
    if ( inMcmcMode == true )
    {
        delete [] partialLikelihoods;
        partialLikelihoods = NULL;
    }

    // set our internal flag
    inMcmcMode = tf;

    if ( inMcmcMode == true )
    {
        resizeLikelihoodVectors();
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setPInv(const TypedDagNode< double > *r)
{

    // remove the old parameter first
    if ( p_inv != NULL )
    {
        this->removeParameter( p_inv );
        p_inv = NULL;
    }

    // set the value
    p_inv = r;

    // add the new parameter
    this->addParameter( p_inv );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setRateMatrix(const TypedDagNode< RateGenerator > *rm) {

    // remove the old parameter first
    if ( homogeneous_rate_matrix != NULL )
    {
        this->removeParameter( homogeneous_rate_matrix );
        homogeneous_rate_matrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneous_rate_matrices );
        heterogeneous_rate_matrices = NULL;
    }

    // set the value
    branch_heterogeneous_substitution_matrices = false;
    homogeneous_rate_matrix = rm;

    // add the new parameter
    this->addParameter( homogeneous_rate_matrix );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setRateMatrix(const TypedDagNode< RbVector< RateGenerator > > *rm) {

    // remove the old parameter first
    if ( homogeneous_rate_matrix != NULL )
    {
        this->removeParameter( homogeneous_rate_matrix );
        homogeneous_rate_matrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneous_rate_matrices );
        heterogeneous_rate_matrices = NULL;
    }

    // set the value
    branch_heterogeneous_substitution_matrices = true;
    heterogeneous_rate_matrices = rm;

    // add the new parameter
    this->addParameter( heterogeneous_rate_matrices );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setRootFrequencies(const TypedDagNode< RbVector< double > > *f)
{

    // remove the old parameter first
    if ( root_frequencies != NULL )
    {
        this->removeParameter( root_frequencies );
        root_frequencies = NULL;
    }

    if ( f != NULL )
    {
        // set the value
        root_frequencies = f;
    }
    else
    {
        branch_heterogeneous_substitution_matrices = false;
    }

    // add the new parameter
    this->addParameter( root_frequencies );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }

}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setSiteRates(const TypedDagNode< RbVector< double > > *r)
{

    // remove the old parameter first
    if ( site_rates != NULL )
    {
        this->removeParameter( site_rates );
        site_rates = NULL;
    }

    if ( r != NULL )
    {
        // set the value
        rate_variation_across_sites = true;
        site_rates = r;
        this->num_site_rates = r->getValue().size();
        this->resizeLikelihoodVectors();
    }
    else
    {
        // set the value
        rate_variation_across_sites = false;
        site_rates = NULL;
        this->num_site_rates = 1;
        this->resizeLikelihoodVectors();

    }

    // add the new parameter
    this->addParameter( site_rates );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::setUseMarginalLikelihoods(bool tf)
{

    this->useMarginalLikelihoods = tf;
    this->resizeLikelihoodVectors();

}


template<class charType>
std::vector< std::vector<double> >* RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::sumMarginalLikelihoods( size_t node_index )
{

    std::vector< std::vector<double> >* per_mixture_Likelihoods = new std::vector< std::vector<double> >(this->pattern_block_size, std::vector<double>(num_chars, 0.0) );

    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node_marginal         = this->marginalLikelihoods + node_index*this->nodeOffset;

    // get pointers the likelihood for both subtrees
    double*         p_mixture_marginal          = p_node_marginal;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {

        // get pointers to the likelihood for this mixture category
        double*         p_site_mixture_marginal         = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->pattern_block_size; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            double*         p_site_marginal_j           = p_site_mixture_marginal;
            // iterate over all starting states
            for (size_t j=0; j<num_chars; ++j)
            {
                // add the probability of being in this state
                (*per_mixture_Likelihoods)[site][j] += *p_site_marginal_j;

                // increment pointers
                ++p_site_marginal_j;
            }

            // increment the pointers to the next site
            p_site_mixture_marginal+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture_marginal+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    return per_mixture_Likelihoods;
}




template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::computeRootLikelihoods( std::vector<double> &rv ) const
{
    // get the root node
    const TopologyNode &root = tau->getValue().getRoot();

    // get the index of the root node
    size_t node_index = root.getIndex();

    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[node_index] * this->activeLikelihoodOffset  + node_index*this->nodeOffset;

    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(pattern_block_size,0.0);

    // get pointer the likelihood
    double*   p_mixture     = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->num_site_rates; ++mixture)
    {

        // get pointers to the likelihood for this mixture category
        double*   p_site_mixture     = p_mixture;
        // iterate over all sites

        for (size_t site = 0; site < pattern_block_size; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointers to the likelihoods for this site and mixture category
            double* p_site_j   = p_site_mixture;
            // iterate over all starting states
            for (size_t i=0; i<num_chars; ++i)
            {
                // add the probability of starting from this state
                tmp += *p_site_j;

                // increment pointers
                ++p_site_j;
            }
            // add the likelihood for this mixture category
            per_mixture_Likelihoods[site] += tmp;

            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset;

        } // end-for over all sites (=patterns)

        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset;

    } // end-for over all mixtures (=rate categories)

    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();

    double p_inv = this->p_inv->getValue();
    double oneMinusPInv = 1.0 - p_inv;
    std::vector< size_t >::const_iterator patterns = this->pattern_counts.begin();
    if ( p_inv > 0.0 )
    {
        for (size_t site = 0; site < pattern_block_size; ++site, ++patterns)
        {

            if ( use_scaling == true )
            {

                if ( this->site_invariant[site] == true )
                {
                    rv[site] = log( p_inv * f[ this->invariant_site_index[site] ] * exp(this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site]) + oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * *patterns;
                }
                else
                {
                    rv[site] = log( oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * *patterns;
                }
                rv[site] -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] * *patterns;

            }
            else // no scaling
            {

                if ( this->site_invariant[site] == true )
                {
                    rv[site] = log( p_inv * f[ this->invariant_site_index[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * *patterns;
                }
                else
                {
                    rv[site] = log( oneMinusPInv * per_mixture_Likelihoods[site] / this->num_site_rates ) * *patterns;
                }

            }
            
        }
        
    }
    else
    {

        for (size_t site = 0; site < pattern_block_size; ++site, ++patterns)
        {

            rv[site] = log( per_mixture_Likelihoods[site] / this->num_site_rates ) * *patterns;

            if ( use_scaling == true )
            {
                rv[site] -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[node_index]][node_index][site] * *patterns;
            }

        }

    }

}



template<class charType>
double RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::sumRootLikelihood( void )
{
        

    std::vector<double> site_likelihoods = std::vector<double>(pattern_block_size,0.0);
    computeRootLikelihoods( site_likelihoods );
    
    double sum_partial_probs = 0.0;
    
    for (size_t site = 0; site < pattern_block_size; ++site)
    {
        sum_partial_probs += site_likelihoods[site];
    }
    
#ifdef RB_MPI

    // we only need to send message if there is more than one process
    if ( num_processes > 1 )
    {
    
        // send the likelihood from the helpers to the master
        if ( process_active == false )
        {
            // send from the workers the log-likelihood to the master
            MPI::COMM_WORLD.Send(&sum_partial_probs, 1, MPI::DOUBLE, active_PID, 0);
        }

        // receive the likelihoods from the helpers
        if ( process_active == true )
        {
            for (size_t i=active_PID+1; i<active_PID+num_processes; ++i)
            {
                double tmp = 0;
                MPI::COMM_WORLD.Recv(&tmp, 1, MPI::DOUBLE, int(i), 0);
                sum_partial_probs += tmp;
            }
        }

        // now send back the combined likelihood to the helpers
        if ( process_active == true )
        {
            for (size_t i=active_PID+1; i<active_PID+num_processes; ++i)
            {
                MPI::COMM_WORLD.Send(&sum_partial_probs, 1, MPI::DOUBLE, int(i), 0);
            }
        }
        else
        {
            MPI::COMM_WORLD.Recv(&sum_partial_probs, 1, MPI::DOUBLE, active_PID, 0);
        }
    
    }

#endif

    return sum_partial_probs;
}



/** Swap a parameter of the distribution */
template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == homogeneous_clock_rate)
    {
        homogeneous_clock_rate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_clock_rates)
    {
        heterogeneous_clock_rates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == homogeneous_rate_matrix)
    {
        homogeneous_rate_matrix = static_cast<const TypedDagNode< RateGenerator >* >( newP );
    }
    else if (oldP == heterogeneous_rate_matrices)
    {
        heterogeneous_rate_matrices = static_cast<const TypedDagNode< RbVector< RateGenerator > >* >( newP );
    }
    else if (oldP == root_frequencies)
    {
        root_frequencies = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == site_rates)
    {
        site_rates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == site_rates_probs)
    {
        site_rates_probs = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == p_inv)
    {
        p_inv = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == tau)
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );

        tau = static_cast<const TypedDagNode<Tree>* >( newP );

        tau->getValue().getTreeChangeEventHandler().addListener( this );

        num_nodes = tau->getValue().getNumberOfNodes();
    }

}

template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::touchSpecialization( DagNode* affecter, bool touchAll )
{

    if ( touched == false )
    {
        touched = true;
        this->storedLnProb = this->lnProb;
    }


    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == heterogeneous_clock_rates )
    {
        const std::set<size_t> &indices = heterogeneous_clock_rates->getTouchedElementIndices();

        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 )
        {
            // just flag everyting for recomputation
            touchAll = true;
        }
        else
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it)
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            }
        }
    }
    else if ( affecter == heterogeneous_rate_matrices )
    {

        const std::set<size_t> &indices = heterogeneous_rate_matrices->getTouchedElementIndices();

        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 )
        {
            // just flag everyting for recomputation
            touchAll = true;
        }
        else
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it)
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            }
        }
    }
    else if ( affecter == root_frequencies )
    {

        const TopologyNode &root = this->tau->getValue().getRoot();
        this->recursivelyFlagNodeDirty( root );
    }
    else if ( affecter != tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    {
        touchAll = true;
    }

    if ( touchAll )
    {

        for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
        {
            (*it) = true;
        }

        // flip the active likelihood pointers
        for (size_t index = 0; index < changed_nodes.size(); ++index)
        {
            if ( changed_nodes[index] == false )
            {
                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                changed_nodes[index] = true;
            }
        }
    }

}



template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::updateMarginalNodeLikelihoods( void )
{
    
    // calculate the root marginal likelihood, then start the recursive call down the tree
    this->computeMarginalRootLikelihood();

    // update the marginal likelihoods by a recursive downpass
    this->recursiveMarginalLikelihoodComputation( tau->getValue().getRoot().getIndex() );


}




template<class charType>
void RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType>::updateTransitionProbabilities(size_t node_idx, double brlen)
{

    // first, get the rate matrix for this branch
    const RateGenerator *rm;
    if ( this->branch_heterogeneous_substitution_matrices == true )
    {
        rm = &this->heterogeneous_rate_matrices->getValue()[node_idx];
    }
    else
    {
        rm = &this->homogeneous_rate_matrix->getValue();
    }

    // second, get the clock rate for the branch
    double rate;
    if ( this->branch_heterogeneous_clock_rates == true )
    {
        rate = this->heterogeneous_clock_rates->getValue()[node_idx];
    }
    else
    {
        rate = this->homogeneous_clock_rate->getValue();
    }

    // and finally compute the per site rate transition probability matrix
    const TopologyNode* node = tau->getValue().getNodes()[node_idx];

    if (node->isRoot()) throw RbException("ERROR: dnPhyloCTMC called updateTransitionProbabilities for the root node\n");

    double end_age = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(end_age) == false )
    {
        // we assume by default that the end is at time 0
        end_age = 0.0;
    }
    double start_age = end_age + node->getBranchLength();

    if ( this->rate_variation_across_sites == true )
    {
        const std::vector<double> &r = this->site_rates->getValue();
        for (size_t i = 0; i < this->num_site_rates; ++i)
        {
            rm->calculateTransitionProbabilities( start_age, end_age,  rate * r[i], this->transition_prob_matrices[i] );
        }
    }
    else
    {
        rm->calculateTransitionProbabilities( start_age, end_age,  rate, this->transition_prob_matrices[0] );
    }
}






#endif
