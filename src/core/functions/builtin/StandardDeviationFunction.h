#ifndef StandardDeviationFunction_H
#define StandardDeviationFunction_H

#include "RbVector.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

#include <vector>

namespace RevBayesCore {
    
    
    /**
     * @brief StandardDeviation of a vector of real numbers.
     *
     * This function computes the StandardDeviation of a vector of real number:
     *   y = sum_{i=1}^{n}(x[i]-\bar{x})*(x[i]-\bar{x})/(n-1)
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-11-13
     *
     */
    class StandardDeviationFunction : public TypedFunction<double> {
        
    public:
        StandardDeviationFunction(const TypedDagNode< RbVector<double> > * v);
        virtual                                            ~StandardDeviationFunction(void);                                            //!< Virtual destructor
        
        // public member functions
        StandardDeviationFunction*                          clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode< RbVector<double> >*             vals;
        
    };
    
}


#endif
