/* 
 * File:   InverseInverseWishartDistribution.h
 * Author: nl
 *
 * Created on 15 juillet 2014, 10:08
 */

#ifndef INVERSEWISHARTDISTRIBUTION_H
#define	INVERSEWISHARTDISTRIBUTION_H


#include <iostream>

#include "TypedDistribution.h"
#include "MatrixReal.h"
#include "RbVector.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    class InverseWishartDistribution : public TypedDistribution<MatrixReal>   {
        
    public:
        
        // inverse InverseWishart distribution of parameter sigma0 and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<MatrixReal> *insigma0, const TypedDagNode<int>* indf);

        // inverse InverseWishart distribution of parameter sigma0 = Diagonal(kappaVector) and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<RbVector<double> > *inkappaVector, const TypedDagNode<int>* indf);

        // inverse InverseWishart distribution of parameter sigma0 kappa * Identitymatrix and df degrees of freedom
        InverseWishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf);

        virtual                                            ~InverseWishartDistribution(void) {}
        
        // public member functions

        InverseWishartDistribution*                         clone(void) const;                                                          //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
        int                                                 getDF() const {return df->getValue();}
        
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter

    private:

        const TypedDagNode<MatrixReal>*                     sigma0;
        const TypedDagNode<RbVector<double> >*              kappaVector;
        const TypedDagNode<double>*                         kappa;
        const TypedDagNode<int>*                            df;
        const TypedDagNode<int>*                            dim;
                
    };
    
}


#endif	/* INVERSEWISHARTDISTRIBUTION_H */

