#ifndef EpisodicBirthDeathProcess_H
#define EpisodicBirthDeathProcess_H

#include "BirthDeathProcess.h"

#include <vector>

namespace RevBayesCore {
    
    class Clade;
    
    class EpisodicBirthDeathProcess : public BirthDeathProcess {
        
    public:
        EpisodicBirthDeathProcess(const TypedDagNode<double> *ra,
                                  const TypedDagNode<RbVector<double> > *s, const TypedDagNode<RbVector<double> > *st,
                                  const TypedDagNode<RbVector<double> > *e, const TypedDagNode<RbVector<double> > *et,
                                  const TypedDagNode<double> *r, const std::string& ss, const std::vector<Clade> &ic,
                                  const std::string &cdt, const std::vector<Taxon> &tn);
        
        // public member functions
        EpisodicBirthDeathProcess*                          clone(void) const;                                                                      //!< Create an independent clone
        
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);                        //!< Swap a parameter
//        double                                              computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        void                                                prepareProbComputation(void) const;
        
        void                                                prepareRateIntegral(double end) const;                        //!< Compute the rate integral.
        void                                                prepareSurvivalProbability(double end, double r) const;                        //!< Compute the rate integral.

        // helper functions
        double                                              lnSpeciationRate(double t) const;
        double                                              rateIntegral(double t_low, double t_high) const;
        double                                              computeProbabilitySurvival(double start, double end) const;
        double                                              simulateDivergenceTime(double origin, double present, double rho) const;                //!< Simulate a speciation event.

    private:
        
//        size_t                                              l(double t) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1]
//        size_t                                              l(double t, size_t min, size_t max) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1]
//        double                                              p(size_t i, double t) const;
//        double                                              q(size_t i, double t) const;
//        int                                                 survivors(double t) const;                                                                          //!< Number of species alive at time t.

        size_t                                              lower_index(double t) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1]
        size_t                                              lower_index(double t, size_t min, size_t max) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1]
        
        
        
        // members
        const TypedDagNode<RbVector<double> >*              lambda_rates;                                                                                             //!< The speciation rates.
        const TypedDagNode<RbVector<double> >*              lambda_times;                                                                                        //!< The time of the speciation rate changes.
        const TypedDagNode<RbVector<double> >*              mu_rates;                                                                                                 //!< The extinction rates.
        const TypedDagNode<RbVector<double> >*              mu_times;                                                                                            //!< The times of the extinction rate changes.
        const TypedDagNode<double>*                         rho;                                                                                                //!< The instantaneous sampling probability.

        mutable std::vector<double>                         rate_change_times;
        mutable std::vector<double>                         birth;
        mutable std::vector<double>                         death;

    };
    
}

#endif
