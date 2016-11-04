#ifndef AbstractPhyloContinuousCharacterProcess_H
#define AbstractPhyloContinuousCharacterProcess_H

#include "ContinuousCharacterData.h"
#include "RbVector.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-23, version 1.0
     */
    class AbstractPhyloContinuousCharacterProcess : public TypedDistribution< ContinuousCharacterData > {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        AbstractPhyloContinuousCharacterProcess(const TypedDagNode<Tree> *t, size_t nSites );
        virtual                                                            ~AbstractPhyloContinuousCharacterProcess(void);                                                     //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual AbstractPhyloContinuousCharacterProcess*                    clone(void) const = 0;                                                                  //!< Create an independent clone
        virtual double                                                      computeLnProbability(void) = 0;
        
        // non-virtual
        void                                                                setBranchRate(const TypedDagNode< double >* r);
        void                                                                setBranchRate(const TypedDagNode< RbVector< double > >* r);
        void                                                                setSiteRate(const TypedDagNode< double >* s);
        void                                                                setSiteRate(const TypedDagNode< RbVector< double > >* s);
        void                                                                setValue(ContinuousCharacterData *v, bool f=false);                                     //!< Set the current value, e.g. attach an observation (clamp)
        void                                                                redrawValue(void);
        void                                                                reInitialized(void);
        
    protected:
        // pure virtual methods
        virtual void                                                        simulateRecursively(const TopologyNode& node, std::vector< ContinuousTaxonData > &t) = 0;
        virtual std::vector<double>                                         simulateRootCharacters(size_t n) = 0;

        
        // helper method for this and derived classes
        double                                                              computeBranchTime(size_t nodeIdx, double brlen);
        double                                                              computeSiteRate(size_t siteIdx);
        virtual void                                                        resetValue(void);
        
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter
        
        // members
        double                                                              lnProb;
        size_t                                                              num_nodes;
        size_t                                                              num_sites;
        const TypedDagNode<Tree>*                                           tau;
        
        // members
        const TypedDagNode< double >*                                       homogeneous_clock_rate;
        const TypedDagNode< RbVector< double > >*                           heterogeneous_clock_rates;
        const TypedDagNode< double >*                                       homogeneous_site_rate;
        const TypedDagNode< RbVector< double > >*                           heterogeneous_site_rates;
        
        
    };
    
}


#endif
