#ifndef NodeOrderConstrainedTreeDistribution_H
#define NodeOrderConstrainedTreeDistribution_H

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "RelativeNodeAgeConstraints.h"

namespace RevBayesCore {
        
    /**
     * @file
     * This file contains the declaration of the random variable class for tree topology priors with node order constraints.
     *
     * @brief Declaration of the tree topology priors with node order constraints class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Bastien Boussau)
     * @since 2015-12-03, version 1.0
     *
     */
    class NodeOrderConstrainedTreeDistribution : public TypedDistribution<Tree> {
        
    public:
        NodeOrderConstrainedTreeDistribution(TypedDistribution<Tree> *base_dist, const RelativeNodeAgeConstraints &c);
        NodeOrderConstrainedTreeDistribution(const NodeOrderConstrainedTreeDistribution &d);
        
        virtual ~NodeOrderConstrainedTreeDistribution();
        // pure virtual member functions
        virtual NodeOrderConstrainedTreeDistribution*           clone(void) const;                                                                              //!< Create an independent clone
        
        
        // public member functions you may want to override
        double                                                  computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        virtual void                                            redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                            setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        
    protected:
        
        //        // virtual methods that may be overwritten, but then the derived class should call this methods
//        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                            swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        bool                                                    matchesConstraints(void);
        void                                                    updateMapOfNodeAges();
        void                                                    updateSetOfConstrainedNodes();

        // members
        TypedDistribution<Tree>*   base_distribution;                                                           //!< Tree distribution.
        RelativeNodeAgeConstraints                              constraints;                                        //!< Node age constraints.
        std::set< std::pair < std::string, std::string > >      constrainedNodes;
        std::map<std::pair<std::string, std::string>, double >  nodeAges;

        
        // just for testing
        bool                                                    owns_tree;

    };
    
}

#endif