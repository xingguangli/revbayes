#ifndef RateMatrix_H
#define RateMatrix_H

#include "RateGenerator.h"
#include "MatrixReal.h"
#include "MemberObject.h"
#include <vector>


namespace RevBayesCore {

    class TransitionProbabilityMatrix;


    class RateMatrix : public RateGenerator, public MemberObject<RbVector<double> > {

    public:
        virtual                            ~RateMatrix(void);                                                                           //!< Destructor

        // overloaded operators
        virtual bool                        operator==(const RateMatrix &rm) const { return this == &rm; }
        virtual bool                        operator!=(const RateMatrix &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const RateMatrix &rm) const { return this < &rm; }
        virtual bool                        operator<=(const RateMatrix &rm) const { return operator<(rm) || operator==(rm); }

        // pure virtual methods you have to overwrite
        virtual double                      averageRate(void) const = 0;                                                                //!< Calculate the average rate
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition matrixrate matrix
        virtual RateMatrix*                 clone(void) const = 0;
        void                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;     //!< Map the member methods to internal function calls
        virtual double                      getRate(size_t from, size_t to, double age, double rate) const = 0;                         //!< Calculate the rate from state i to state j over the given time interval scaled by a rate
        virtual double                      getRate(size_t from, size_t to, double rate=1.0) const = 0;
        virtual std::vector<double>         getStationaryFrequencies(void) const = 0;                                                   //!< Return the stationary frequencies
        virtual void                        rescaleToAverageRate(double r) = 0;                                                         //!< Rescale the rate matrix such that the average rate is "r"
        virtual void                        setDiagonal(void) = 0;                                                                      //!< Set the diagonal such that each row sums to zero
        virtual void                        update(void) = 0;                                                                           //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)

        virtual RateMatrix&                 assign(const Assignable &m);

    protected:
        // prevent instantiation
        RateMatrix(size_t n);                                                                                                           //!< Construct rate matrix with n states

    };

}

#endif

