/**
 * @file
 * This file contains the declaration of the ChromosomesRateMatrixFunction class.
 * This class is derived from the function class and is used to
 * create the chromosomes rate matrix.
 *
 * @brief Declaration of the ChromosomesRateMatrixFunction.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */


#ifndef ChromosomesRateMatrixFunction_H
#define ChromosomesRateMatrixFunction_H

#include "RateMatrix_Chromosomes.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    class ChromosomesRateMatrixFunction : public TypedFunction<RateGenerator> {

    public:
        ChromosomesRateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l, const TypedDagNode<double> *d, const TypedDagNode<double> *r, const TypedDagNode<double> *m, const TypedDagNode<double> *l_l, const TypedDagNode<double> *d_l);
        
        virtual                                     ~ChromosomesRateMatrixFunction(void);                                                    //!< Virtual destructor
        
        // public member functions
        ChromosomesRateMatrixFunction*              clone(void) const;                                                              //!< Create an independent clone
        void                                        update(void);
        
    protected:
        void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Implementation of swaping parameters
        
    private:
        
        // members

        const TypedDagNode<int>*                    n;
        const TypedDagNode<double>*                 lambda;
        const TypedDagNode<double>*                 delta;
        const TypedDagNode<double>*                 rho;
        const TypedDagNode<double>*                 mu;
        const TypedDagNode<double>*                 lambda_l;
        const TypedDagNode<double>*                 delta_l;

    };
    
}

#endif
