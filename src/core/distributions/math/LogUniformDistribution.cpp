#include "LogUniformDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>

using namespace RevBayesCore;

LogUniformDistribution::LogUniformDistribution(const TypedDagNode<double> *mi, const TypedDagNode<double> *ma) : ContinuousDistribution( new double( 1.0 ) ),
    min( mi ),
    max( ma )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( min );
    addParameter( max );
    
    double u = GLOBAL_RNG->uniform01();
    double ln_a = log( min->getValue() );
    *value = exp( u*(log( max->getValue() ) - ln_a) + ln_a);
}


LogUniformDistribution::~LogUniformDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double LogUniformDistribution::cdf( void ) const {
    return 0.0;
}


LogUniformDistribution* LogUniformDistribution::clone( void ) const {
    return new LogUniformDistribution( *this );
}


double LogUniformDistribution::computeLnProbability( void ) 
{
    
    double v = *value; 
    double mi = min->getValue();
    double ma = max->getValue();
    if ( v >= mi && v <= ma ) 
    {
       return 1.0 / ( (log(max->getValue()) - log( min->getValue() )) * (*value) ); 
    } 
    else 
    {
        return RbConstants::Double::neginf;
    }
    
    
}


double LogUniformDistribution::getMax( void ) const 
{

    return max->getValue();
}


double LogUniformDistribution::getMin( void ) const 
{

    return min->getValue();
}


double LogUniformDistribution::quantile(double p) const 
{

    return 0.0;
}


void LogUniformDistribution::redrawValue( void ) 
{

    double u = GLOBAL_RNG->uniform01();
    double ln_a = log( min->getValue() );
    *value = exp( u*(log( max->getValue() ) - ln_a) + ln_a);

}


/** Swap a parameter of the distribution */
void LogUniformDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == min) 
    {
        min = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == max) 
    {
        max = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}
