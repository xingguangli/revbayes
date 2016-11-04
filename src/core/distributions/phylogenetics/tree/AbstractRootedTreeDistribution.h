#ifndef AbstractRootedTreeDistribution_H
#define AbstractRootedTreeDistribution_H

#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    /**
     * @file
     * This file contains the declaration of the random variable class for constant rate birth-death process.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2016-01-26, version 1.0
     *
     */
    class AbstractRootedTreeDistribution : public TypedDistribution<Tree> {
        
    public:
        AbstractRootedTreeDistribution(const TypedDagNode<double> *ra, const std::vector<Taxon> &tn);
        
        
        virtual ~AbstractRootedTreeDistribution(void);
        // pure virtual member functions
        virtual AbstractRootedTreeDistribution*             clone(void) const = 0;                                                                              //!< Create an independent clone
        
        
        // public member functions you may want to override
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        virtual void                                        redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        size_t                                              getNumberOfTaxa(void) const;
        virtual double                                      getRootAge(void) const;
        double                                              getOriginTime(void) const;
        const std::vector<Taxon>&                           getTaxa(void) const;
        void                                                simulateClade(std::vector<TopologyNode *> &n, double age, double present);
        
    protected:
        // pure virtual helper functions
        virtual double                                      computeLnProbabilityDivergenceTimes(void) const = 0;                                                          //!< Compute the log-transformed probability of the current value.
        virtual double                                      simulateDivergenceTime(double origin, double present) const = 0;                                                  //!< Simulate n speciation events.
        virtual std::vector<double>*                        simulateDivergenceTimes(size_t n, double origin, double present) const = 0;                                                  //!< Simulate n speciation events.
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                  //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        void                                                recomputeDivergenceTimesSinceOrigin(void) const;                                                             //!< Extract the divergence times from the tree.
        int                                                 diversity(double t) const;                                                                          //!< Diversity at time t.
        std::vector<double>*                                getAgesOfInternalNodesFromMostRecentSample(void) const;                                             //!< Get the ages of all internal nodes since the time of the most recent tip age.
        std::vector<double>*                                getAgesOfTipsFromMostRecentSample(void) const;                                                      //!< Get the ages of all tip nodes since the time of the most recent tip age.
        double                                              simulateNextAge(size_t n, double start, double end, double present) const;
        void                                                simulateTree(void);
        
        // members                                                                                           //!< Time since the origin.
        const TypedDagNode<double>*                         root_age;                                                                                            //!< Time since the origin.
        size_t                                              num_taxa;                                                                                            //!< Number of taxa (needed for correct initialization).
        std::vector<Taxon>                                  taxa;                                                                                               //!< Taxon names that will be attached to new simulated trees.
        mutable std::vector<double>                         divergence_times;                                                                                               //!< Taxon names that will be attached to new simulated trees.
        double                                              log_tree_topology_prob;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        
    };
    
}

#endif
