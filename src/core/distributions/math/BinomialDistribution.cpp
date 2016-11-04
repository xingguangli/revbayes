#include "BinomialDistribution.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

BinomialDistribution::BinomialDistribution(const TypedDagNode<int> *m, const TypedDagNode<double> *q) : TypedDistribution<int>( new int( 0 ) ),
    n( m ),
    p( q )
{
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( n );
    addParameter( q );
    
    *value = RbStatistics::Binomial::rv(n->getValue(), p->getValue(), *GLOBAL_RNG);
}


BinomialDistribution::~BinomialDistribution(void) {

    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BinomialDistribution* BinomialDistribution::clone( void ) const {

    return new BinomialDistribution( *this );
}


double BinomialDistribution::computeLnProbability( void )
{
    
    // check that the value is inside the boundaries
    if ( *value > n->getValue() || *value < 0 )
    {
        return RbConstants::Double::neginf;
    }
    
    return RbStatistics::Binomial::lnPdf(n->getValue(), p->getValue(), *value);
}



void BinomialDistribution::redrawValue( void ) {

    *value = RbStatistics::Binomial::rv(n->getValue(), p->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void BinomialDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    if (oldP == p)
    {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == n)
    {
        n = static_cast<const TypedDagNode<int>* >( newP );
    }

}


