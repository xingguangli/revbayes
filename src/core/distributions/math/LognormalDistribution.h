#ifndef LognormalDistribution_H
#define LognormalDistribution_H

#include "ContinuousDistribution.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class LognormalDistribution : public ContinuousDistribution {
        
    public:
        LognormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s);
        virtual                                            ~LognormalDistribution(void);                                                  //!< Virtual destructor
        
        // public member functions
        double                                              cdf(void) const;                                                                  //!< Cummulative density function
        LognormalDistribution*                              clone(void) const;                                                          //!< Create an independent clone
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
        const TypedDagNode<double>*                         mean;
        const TypedDagNode<double>*                         sd;
        
    };
    
}

#endif
