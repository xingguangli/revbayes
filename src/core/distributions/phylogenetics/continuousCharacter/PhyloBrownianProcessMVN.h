#ifndef PhyloBrownianProcessMVN_H
#define PhyloBrownianProcessMVN_H

#include "AbstractPhyloBrownianProcess.h"
#include "MatrixReal.h"

#include <vector>

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
    class PhyloBrownianProcessMVN : public AbstractPhyloBrownianProcess {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        PhyloBrownianProcessMVN(const TypedDagNode<Tree> *t, size_t nSites );
        virtual                                                            ~PhyloBrownianProcessMVN(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual PhyloBrownianProcessMVN*                                    clone(void) const;                                                                      //!< Create an independent clone
        
        // non-virtual
        double                                                              computeLnProbability(void);
        void                                                                setRootState(const TypedDagNode< double >* s);
        void                                                                setRootState(const TypedDagNode< RbVector< double > >* s);
        
    protected:
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        void                                                                resetValue( void );
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        std::vector<double>                                                 simulateRootCharacters(size_t n);
        double                                                              sumRootLikelihood(void);
        virtual void                                                        touchSpecialization(DagNode *toucher, bool touchAll);
       
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter
        
    private:
        double                                                              computeRootState(size_t siteIdx);
        std::set<size_t>                                                    recursiveComputeCovarianceMatrix( MatrixReal &m, const TopologyNode &node, size_t node_index );
        
        const TypedDagNode< double >*                                       homogeneous_root_state;
        const TypedDagNode< RbVector< double > >*                           heterogeneous_root_state;
        
        size_t                                                              numTips;
        std::vector<std::vector<double> >                                   obs;
        MatrixReal*                                                         phylogeneticCovarianceMatrix;
        MatrixReal*                                                         storedPhylogeneticCovarianceMatrix;
        MatrixReal                                                          inversePhylogeneticCovarianceMatrix;
        bool                                                                changedCovariance;
        bool                                                                needsCovarianceRecomputation;
        bool                                                                needsScaleRecomputation;
    };
    
}


#endif
