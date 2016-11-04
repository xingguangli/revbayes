#ifndef UniformTopologyDistribution_H
#define UniformTopologyDistribution_H

#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class UniformTopologyDistribution : public TypedDistribution<Tree> {
        
    public:
        UniformTopologyDistribution(const std::vector<Taxon> &ta, const Clade &og, const std::vector<Clade> &c, bool rooted = false);
		virtual                                            ~UniformTopologyDistribution(void);                                                                    //!< Virtual destructor
        
        // public member functions
        UniformTopologyDistribution*                        clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                simulateTree(void);
        bool                                                matchesConstraints(void);
        void                                                simulateClade(std::vector<TopologyNode*> &n);                                           //!< Simulate n speciation events.

        // members
        size_t                                              num_taxa;
        std::vector<Taxon>                                  taxa;
        std::vector<Clade>                                  constraints;
        double                                              logTreeTopologyProb;                                                 //!< Topological constrains.
        Clade                                               outgroup;
        bool												rooted;
    };
    
}

#endif
