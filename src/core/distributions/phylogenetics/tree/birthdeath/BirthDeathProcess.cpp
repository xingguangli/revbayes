#include "Clade.h"
#include "BirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathLogic.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    o         Origin or time of the process.
 * \param[in]    ra        Age or the root (=time of the process).
 * \param[in]    r         Sampling probability of a species at present.
 * \param[in]    ss        The sampling strategy (uniform/diversified).
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
BirthDeathProcess::BirthDeathProcess(const TypedDagNode<double> *ra, const TypedDagNode<double> *rh,
                                     const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
                                     const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( ra, cdt, tn ),
    rho( rh ),
    sampling_strategy( ss ),
    incomplete_clades( ic )
{
    
    addParameter( rho );
    
    log_p_survival = std::vector<double>(tn.size()-2,0.0);
    rate_integral  = std::vector<double>(tn.size()-2,0.0);
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double BirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double ln_prob_times = 0;
    
    // retrieved the speciation times
    recomputeDivergenceTimesSinceOrigin();
    
    double sampling_probability = 1.0;
    if ( sampling_strategy == "uniform" ) 
    {
        sampling_probability = rho->getValue();
    }
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = ra;

    
    // multiply the probability of a descendant of the initial species
    ln_prob_times += lnP1(0,presentTime,sampling_probability);
    
    // we started at the root thus we square the survival prob
    ln_prob_times *= 2.0;
    
    for (size_t i = 1; i < num_taxa-1; ++i)
    {
        if ( RbMath::isFinite(ln_prob_times) == false )
        {
            return RbConstants::Double::nan;
        }
        
        // We will only multiply here the probability densities of the speciation events
        ln_prob_times += lnSpeciationRate(divergence_times[i]);
    }
    // add the P1 for ALL speciation events
    ln_prob_times += lnP1(presentTime,sampling_probability);
    
    // if we assume diversified sampling, we need to multiply with the probability that all missing species happened after the last speciation event
    if ( sampling_strategy == "diversified" ) 
    {
        // We use equation (5) of Hoehna et al. "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        double last_event = divergence_times[divergence_times.size()-2];
        
        double p_0_T = 1.0 - pSurvival(0,presentTime,1.0) * exp( rateIntegral(0,presentTime) );
        double p_0_t = (1.0 - pSurvival(last_event,presentTime,1.0) * exp( rateIntegral(last_event,presentTime) ));
        double F_t = p_0_t / p_0_T;
        
        // get an estimate of the actual number of taxa
        double m = round(num_taxa / rho->getValue());
        ln_prob_times += (m-num_taxa) * log(F_t) + log(RbMath::choose(m,num_taxa));
    }
    
    // now iterate over the vector of missing species per interval
    for (size_t i=0; i<incomplete_clades.size(); ++i)
    {
        // We use equation (5) of Hoehna et al.
        // "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        
//        double last_event = presentTime - this->value->getTmrca( incomplete_clades[i] );
        double last_event = presentTime - incomplete_clade_ages[i];
        
        double p_0_T = 1.0 - pSurvival(0,presentTime,1.0) * exp( rateIntegral(0,presentTime) );
        double p_0_t = (1.0 - pSurvival(last_event,presentTime,1.0) * exp( rateIntegral(last_event,presentTime) ));
        double log_F_t = log(p_0_t) - log(p_0_T);

        // get an estimate of the actual number of taxa
        int m = incomplete_clades[i].getNumberMissingTaxa();
        // remove the number of species that we started with
        
        // multiply the probability for the missing species
        // lnl = lnl + sum( m * log_F_t ) #+ lchoose(m-k,nTaxa-k)
        ln_prob_times += m * log_F_t; // + log(RbMath::choose(m,num_taxa));

    }
    
    return ln_prob_times;
}


double BirthDeathProcess::lnP1(double end, double r) const
{
    
    double ln_p = 0;
    double log_r = log(r);
    prepareSurvivalProbability(end,r);
    prepareRateIntegral(end);
    
    for (size_t i = 0; i < num_taxa-2; ++i)
    {
        // get the survival probability
        double a = log_p_survival[i];
        double b = rate_integral[i];
        
        // compute the probability of observing/sampling exactly one lineage
        ln_p += 2.0 * a + b;
    }
    ln_p -= log_r * (num_taxa-2);
    
    return ln_p;
    
}


void BirthDeathProcess::prepareRateIntegral(double end) const
{
    
    for (size_t i = 1; i < num_taxa-1; ++i)
    {
        double t = divergence_times[i];
        rate_integral[i-1] = rateIntegral(t, end);
    }

}

void BirthDeathProcess::prepareSurvivalProbability(double end, double r) const
{
    
    for (size_t i = 1; i < num_taxa-1; ++i)
    {
        double t = divergence_times[i];
        log_p_survival[i-1] = log( pSurvival(t,end,r) );
    }
    
}


double BirthDeathProcess::lnP1(double t, double T, double r) const
{
    
    // get the survival probability
    double a = log( pSurvival(t,T,r) );
    double b = rateIntegral(t, T) - log(r);
    
    // compute the probability of observing/sampling exactly one lineage
    double p = 2.0 * a + b;
    
    return p;
    
}


/**
 *
 */
double BirthDeathProcess::lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const
{
    
    double p = 0;
    double r = rho->getValue();
    if ( n < 1 )
    {
        // we assume conditioning on survival
        p = 0.0;
    }
    else if (n == 1)
    {
        if ( MRCA == true )
        {
            // we assume conditioning on survival of the two species
            p = 0.0;
        }
        else
        {
            double ln_ps = log( pSurvival(start, end, r) );
            double rate = rateIntegral(start, end) - log(r);
            p = 2*ln_ps + rate;
        }
    }
    else
    {
        double p_s = pSurvival(start, end, r);
        double rate = rateIntegral(start, end) - log(r);
        double e = p_s * exp(rate);
        
        if ( MRCA == false )
        {
            p = 2*log(p_s) + rate + log( 1 - e) * (n-1);
        }
        else
        {
            p = log(n-1) + 4*log(p_s) + 2*rate + log( 1 - e) * (n-2);
        }
    }
    
    return p;
}


/**
 * Compute the probabililty of survival (no extinction) of the process including uniform taxon sampling at the present time.
 * The probability of survival is given by
 * [1 + int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds ) ]^{-1}
 * and can be simplified to
 * [1 + int_{t_low}^{t_high} ( mu'(s) exp(rate'(t,s)) ds ) - (r-1)/r*exp(rate'(t_low,t_high)) ]^{-1}
 * where mu' and rate' are the diversification rate function without incomplete taxon sampling.
 * Therefore we can just call pSurvival without incomplete taxon sampling that will be computed in the derived classes,
 * and add the sampling here so that sampling will be available for all models :)
 * For more information please read Hoehna, S. 2014. The time-dependent reconstructed evolutionary process with a key-role for mass-extinction events.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 * \param[in]    r          Sampling probability.
 *
 * \return The probability of survival of the process.
 */
double BirthDeathProcess::pSurvival(double start, double end, double r) const
{
    double rate = rateIntegral(start, end);
    double ps = 1.0 / computeProbabilitySurvival(start, end);
    
    return 1.0 / (ps - (r-1.0)/r * exp(rate) );
}


double BirthDeathProcess::pSurvival(double start, double end) const
{
    double sampling_prob = rho->getValue();
//    sampling_prob = 1.0;
    
    return pSurvival(start, end, sampling_prob);
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void BirthDeathProcess::restoreSpecialization(DagNode *affecter)
{
    
    AbstractRootedTreeDistribution::restoreSpecialization(affecter);
    if ( affecter == this->dag_node )
    {
        incomplete_clade_ages.clear();
        incomplete_clade_ages.resize(incomplete_clades.size());
        
        for (size_t i=0; i<incomplete_clades.size(); ++i)
        {
            incomplete_clade_ages[i] = this->value->getTmrca( incomplete_clades[i] );
        }
        
    }
    
}



double BirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    
//    if ( sampling_strategy == "uniform" )
//    {
        return simulateDivergenceTime( origin, present, rho->getValue() );
//    }
//    else
//    {
//        std::vector<double>* all = simulateDivergenceTime( round(n/rho->getValue()), origin, 1.0 );
//        all->resize(n);
//        return all;
//    }
    
}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void BirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == rho ) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }

}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void BirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    AbstractRootedTreeDistribution::touchSpecialization(affecter, touchAll);
    if ( affecter == this->dag_node )
    {
        incomplete_clade_ages.clear();
        incomplete_clade_ages.resize(incomplete_clades.size());
        
        for (size_t i=0; i<incomplete_clades.size(); ++i)
        {
            incomplete_clade_ages[i] = this->value->getTmrca( incomplete_clades[i] );
        }

    }
    
}
