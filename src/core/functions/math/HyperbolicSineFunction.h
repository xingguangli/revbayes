#ifndef HyperbolicSineFunction_h
#define HyperbolicSineFunction_h

#include <stdio.h>
#include "ContinuousFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Hyperbolic Sine of a real number.
     *
     * Compute the hyperbolic Sine of a real number x. (sinh(x) = (exp(x) - exp(-x))/2).
     *
     * \copyright (c) Copyright 2009-2015 (GPL version 3)
     * \author The RevBayes Development Core Team (Wade Dismukes)
     * \since Version 1.0, 2015-01-31
     *
     */
    class HyperbolicSineFunction : public ContinuousFunction {
        
    public:
        HyperbolicSineFunction(const TypedDagNode<double> *a);
        
        HyperbolicSineFunction*          clone(void) const;                                                  //!< Create a clone.
        void                                update(void);                                                       //!< Recompute the value
        
    protected:
        void                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<double>*         x;
    };
}


#endif /* HyperbolicSineFunction_hpp */
