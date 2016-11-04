#include "FreeKRateMatrixFunction.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"

using namespace RevBayesCore;

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode< RbVector<double> > *trf) : TypedFunction<RateGenerator>( new RateMatrix_FreeK( 0.5+sqrt(0.25+trf->getValue().size() ) ) ),
    transition_rates( NULL ),
    transition_ratesFlat( trf )
{
    
    // add the rate and frequency parameters as parents
    addParameter( transition_ratesFlat );
    
    update();
}

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode< RbVector<RbVector<double> > >*tr) : TypedFunction<RateGenerator>( new RateMatrix_FreeK( tr->getValue().size() ) ),
    transition_rates( tr ),
    transition_ratesFlat( NULL )
{
    
    // add the rate and frequency parameters as parents
    addParameter( transition_rates );
    
    update();
}

FreeKRateMatrixFunction::~FreeKRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



FreeKRateMatrixFunction* FreeKRateMatrixFunction::clone( void ) const
{
    return new FreeKRateMatrixFunction( *this );
}


void FreeKRateMatrixFunction::update( void )
{
    // get the information from the arguments for reading the file
    if (transition_rates != NULL && transition_ratesFlat == NULL) {
        const RbVector<RbVector<double> >& r = transition_rates->getValue();
        
        size_t n = r.size();
        std::vector<double> r_flat( n * (n-1) );
        size_t k = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (i != j) {
                    r_flat[k++] = r[i][j];
                }
            }
        }

        // set the flattened rates
        static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r_flat);
    }
    
    else if (transition_rates == NULL && transition_ratesFlat != NULL) {
        const std::vector<double>& r = transition_ratesFlat->getValue();
        
        // set the flattened rates
        static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r);
    }


    
    value->update();
}



void FreeKRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == transition_rates)
    {
        transition_rates = static_cast<const TypedDagNode< RbVector<RbVector<double> > >* >( newP );
    }
    else if (oldP == transition_ratesFlat)
    {
        transition_ratesFlat = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

