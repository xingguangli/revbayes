#ifndef BetaDistribution_H
#define BetaDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    /**
     * @brief Beta distribution class.
     *
     * The Beta distribution represents a family of distributions
     * on the between 0 and 1. The Beta distribution has 2 parameters:
     *   alpha
     *   beta
     * Instances of this class can be associated to stochastic variables.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-12, version 1.0
     *
     */
    class BetaDistribution : public ContinuousDistribution {
        
    public:
        BetaDistribution(const TypedDagNode<double> *a, const TypedDagNode<double> *b);
        virtual                                            ~BetaDistribution(void);                                                  //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        BetaDistribution*                                   clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        double                                              getMax(void) const;
        double                                              getMin(void) const;
        double                                              quantile(double p) const;                                                       //!< Qu
        void                                                redrawValue(void);

    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // members
        const TypedDagNode<double>*                          alpha;
        const TypedDagNode<double>*                          beta;
        
    };
    
}

#endif
