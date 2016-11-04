#include "CategoricalDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>

using namespace RevBayesCore;

CategoricalDistribution::CategoricalDistribution(const TypedDagNode< RbVector<double> > *p) :
    TypedDistribution<int>( new int( 1 ) ),
    probs( p )
{
    // add the parameters to our set (in the base class)
    // in that way other classes can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( probs );
    
    redrawValue();
}


CategoricalDistribution::~CategoricalDistribution( void )
{
    // The parameter is memory-managed by the model
}



CategoricalDistribution* CategoricalDistribution::clone( void ) const
{
    return new CategoricalDistribution( *this );
}


double CategoricalDistribution::computeLnProbability( void )
{
    if ( *value > probs->getValue().size() || *value < 1 )
    {
        return RbConstants::Double::neginf;
    }
    
    return log( probs->getValue()[ *value - 1 ] );  // value is 1-offset
}


/**
 * Redraw the value. Since we cannot rely on the simplex parameter probs being
 * unchanged since the last draw, we choose not to compute a map between the
 * probabilities and the index. This is slightly less efficient but avoids
 * the problem of having to figure out when the probs parameter has changed
 * and the map needs to be recomputed.
 */
void CategoricalDistribution::redrawValue( void )
{
    // Get a random number
    double u = GLOBAL_RNG->uniform01();

    // Find the index matching this probability
    std::vector<double> p = probs->getValue();
    double sum = 0.0;
    int index;
    for ( index = 0; index < p.size() - 1; ++index )
    {
        sum += p[index];
        if ( u < sum )
            break;
    }

    *value = index + 1;
}


/** Swap a parameter of the distribution */
void CategoricalDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == probs)
    {
        probs = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}


