//
//  RateGenerator_Epoch.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__RateGenerator_Epoch__
#define __revbayes_proj__RateGenerator_Epoch__


#include "RateGenerator.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateGenerator_Epoch : public RateGenerator {
        
    public:
        RateGenerator_Epoch(size_t n, size_t ne);                                                                                                                   //!< Construct rate matrix with n states
        RateGenerator_Epoch(const RateGenerator_Epoch& m);                                                                                                          //!< Copy constructor
        virtual                             ~RateGenerator_Epoch(void);                                                                                             //!< Destructor
        
        // overloaded operators
        RateGenerator_Epoch&                operator=(const RateGenerator_Epoch& r);
        
        // RateMatrix functions
        RateGenerator_Epoch&                assign(const Assignable &m);
        void                                calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition matrix
        RateGenerator_Epoch*                clone(void) const;
        double                              getRate(size_t from, size_t to, double age, double rate) const;                                    //!< Calculate the rate from state i to state j over the given time interval scaled by a rate
        const RbVector<RateGenerator>&      getRateGenerators(void) const;                                                                                         //!< Return the epoch generators
        const RbVector<double>&             getEpochTimes(void) const;                                                                                             //!< Return the epoch times
        const RbVector<double>&             getEpochRates(void) const;                                                                                             //!< Return the epoch rates
        void                                setEpochGenerators(const RbVector<RateGenerator>& rg);                                                              //!< Directly set the epoch generators
        void                                setEpochTimes(const RbVector<double> &t);                                                                           //!< Directly set the epoch times
        void                                setEpochRates(const RbVector<double>& r);                                                                           //!< Directly set the epoch rates
        void                                update(void);
        
    private:
        size_t                              findEpochIndex( double t ) const;

        // parameters
        RbVector<RateGenerator>             epochRateGenerators;
        RbVector<double>                    epochTimes;
        RbVector<double>                    epochRates;

        // helper variables
        size_t                              numEpochs;
        bool                                needs_update;


    };
    
    std::ostream&                       operator<<(std::ostream& o, const RateGenerator_Epoch& x);                                           //!< Overloaded output operator

}

#endif /* defined(__revbayes_proj__RateGenerator_Epoch__) */
