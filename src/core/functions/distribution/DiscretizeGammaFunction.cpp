#include "DiscretizeGammaFunction.h"

#include "DistributionChisq.h"
#include "RbMathFunctions.h"



RevBayesCore::DiscretizeGammaFunction::DiscretizeGammaFunction(const TypedDagNode<double> *s, const TypedDagNode<double> *r, const TypedDagNode<int> *nc, bool med) : TypedFunction< RbVector<double> >( new RbVector<double>(nc->getValue(), 1.0) ),
    shape( s ),
    rate( r ),
    numCats(nc),
    median(med)
{
    
    addParameter( shape );
    addParameter( rate );
    addParameter( numCats );
        
}



RevBayesCore::DiscretizeGammaFunction* RevBayesCore::DiscretizeGammaFunction::clone( void ) const
{
    
    return new DiscretizeGammaFunction(*this);
}


void RevBayesCore::DiscretizeGammaFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == shape) 
    {
        shape = static_cast<const TypedDagNode<double>* >( newP );
    }
    
    if (oldP == rate)
    {
        rate = static_cast<const TypedDagNode<double>* >( newP );
    }
    
    if (oldP == numCats)
    {
        numCats = static_cast<const TypedDagNode<int>* >( newP );
    }
    
}

void RevBayesCore::DiscretizeGammaFunction::update( void ) {
    
    double a = shape->getValue();
    double b = rate->getValue();
    int nCats = numCats->getValue();
    
    double factor = a / b * nCats;

    if (median) {
        /* the median value for each category is used to represent all of the values
        in that category */
        double interval = 1.0 / (2.0 * nCats);
        for (int i=0; i<nCats; i++) 
            (*value)[i] = RbStatistics::ChiSquare::quantile((i * 2.0 + 1.0) * interval, 2.0 * a) / (2.0 * b);
        double t = 0.0;
        for (int i=0; i<nCats; i++) 
            t += (*value)[i];
        for (int i=0; i<nCats; i++)     
            (*value)[i] *= factor / t;
    }
    else
    {
        /* the mean value for each category is used to represent all of the values
        in that category */
        /* calculate the points in the gamma distribution */
        for (int i=0; i<nCats-1; i++) 
            (*value)[i] = RbStatistics::ChiSquare::quantile((i + 1.0) / nCats, 2.0 * a) / (2.0 * b);
        /* calculate the cumulative values */
        double lnGammaValue = RbMath::lnGamma(a + 1.0);
        for (int i=0; i<nCats-1; i++) 
            (*value)[i] = RbMath::incompleteGamma((*value)[i] * b, a + 1.0, lnGammaValue);
        (*value)[nCats-1] = 1.0;
        /* calculate the relative values and rescale */
        for (int i=nCats-1; i>0; i--){
            (*value)[i] -= (*value)[i-1];
            (*value)[i] *= factor;
        }
        (*value)[0] *= factor;
    }
    
//    *value = dist->quantile( p->getValue() );
}
