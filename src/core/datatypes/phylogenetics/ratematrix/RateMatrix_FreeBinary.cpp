//
//  RateMatrix_FreeBinary.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMatrix_FreeBinary.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_FreeBinary::RateMatrix_FreeBinary(void) : GeneralRateMatrix( 2 ) {

    update();
}


/** Destructor */
RateMatrix_FreeBinary::~RateMatrix_FreeBinary(void) {

}

double RateMatrix_FreeBinary::averageRate(void) const
{
    return 1.0;
}

/** Calculate the transition probabilities */
void RateMatrix_FreeBinary::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {

//    double expPart = exp( - (transition_rates[0] + transition_rates[1]) * t);
    double t = rate * (startAge - endAge);
    const MatrixReal& m = *the_rate_matrix;
    double expPart = exp( -(m[0][1] + m[1][0]) * t);
    double pi0 = m[1][0] / (m[0][1] + m[1][0]);
    double pi1 = 1.0 - pi0;
    P[0][0] = pi0 + pi1 * expPart;
    P[0][1] = pi1 - pi1 * expPart;
    P[1][0] = pi0 - pi0 * expPart;
    P[1][1] = pi1 + pi0 * expPart;

}


RateMatrix_FreeBinary* RateMatrix_FreeBinary::clone( void ) const {
    return new RateMatrix_FreeBinary( *this );
}



void RateMatrix_FreeBinary::fillRateMatrix( void )
{

    MatrixReal& m = *the_rate_matrix;

    // set the off-diagonal portions of the rate matrix
    m[0][0] = -transition_rates[0];
    m[0][1] = transition_rates[0];
    m[1][0] = transition_rates[1];
    m[1][1] = -transition_rates[1];

    // set flags
    needs_update = true;
}

std::vector<double> RateMatrix_FreeBinary::getStationaryFrequencies( void ) const
{
    std::vector<double> stationary_freqs;

    stationary_freqs.push_back( transition_rates[1]/(transition_rates[0] + transition_rates[1]) );
    stationary_freqs.push_back( transition_rates[0]/(transition_rates[0] + transition_rates[1]) );

    return stationary_freqs;
}


void RateMatrix_FreeBinary::update( void ) {

    if ( needs_update )
    {
        fillRateMatrix();

        // clean flags
        needs_update = false;
    }
}
