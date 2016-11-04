/**
 * @file
 * This file contains commonly used statistics functions that are used
 * in RevBayes. The probability density (pdf), log of the probability
 * density (lnPdf), cumulative probability (cdf), and quantiles of
 * common probability distributions.
 *
 * \brief Namespace containing statistical functions
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbConstants_H
#define RbConstants_H

#include <cfloat>
#include <limits>
#include <cstdlib>

namespace RbConstants {
    
    namespace Double {
        const double    inf         = std::numeric_limits<double>::infinity();
        const double    max         = std::numeric_limits<double>::max();
        // (unused)        const double    min         = std::numeric_limits<double>::min();
        const double    nan         = std::numeric_limits<double>::quiet_NaN();
        const double    neginf      = -std::numeric_limits<double>::infinity();
    }
    
    namespace Integer {
        const int       inf         = std::numeric_limits<int>::infinity();
        const int       max         = std::numeric_limits<int>::max();
        // (unused)        const int       min         = std::numeric_limits<int>::min();
        const int       nan         = std::numeric_limits<int>::quiet_NaN();
        const int       neginf      = -2147483647;
    }
    
    namespace Object {
        const double    undefined   = 1E+37;
        const double    neginf      = 1E-37;
    }
    
    namespace Size_t {
        const size_t    inf         = -1;
        const size_t    max         = std::numeric_limits<std::size_t>::max();
        //        const size_t    min         = std::numeric_limits<std::size_t>::min();
        const size_t    nan         = std::numeric_limits<std::size_t>::quiet_NaN();
    }
    
    
    /* ----- The following constants and entry points are part of the R API ---- */
    
    /* 30 Decimal-place constants */
    /* Computed with bc -l (scale=32; proper round) */
    
    /* SVID & X/Open Constants */
    /* Names from Solaris math.h */
    const double    E               = 2.718281828459045235360287471353;	/* e */
    const double    LOG2E           = 1.442695040888963407359924681002;	/* log2(e) */
    const double    LOG10E          = 0.434294481903251827651128918917;	/* log10(e) */
    const double    LN2             = 0.693147180559945309417232121458;	/* ln(2) */
    const double    LN10            = 2.302585092994045684017991454684;	/* ln(10) */
    const double    PI              = 3.141592653589793238462643383280;	/* pi */
    const double    TwoPI             = 6.283185307179586476925286766559;	/* 2*pi */
    const double    PI_2            = 1.570796326794896619231321691640;	/* pi/2 */
    const double    PI_4            = 0.785398163397448309615660845820;	/* pi/4 */
    // (unused)    const double    1_PI            = 0.318309886183790671537767526745;	/* 1/pi */
    // (unused)    const double    2_PI            = 0.636619772367581343075535053490;	/* 2/pi */
    // (unused)    const double    2_SQRTPI        = 1.128379167095512573896158903122;	/* 2/sqrt(pi) */
    const double    SQRT2           = 1.414213562373095048801688724210;	/* sqrt(2) */
    const double    SQRT1_2         = 0.707106781186547524400844362105;	/* 1/sqrt(2) */
    
    /* R-Specific Constants */
    
    const double     SQRT_3         = 1.732050807568877293527446341506;	/* sqrt(3) */
    const double     SQRT_32        = 5.656854249492380195206754896838;	/* sqrt(32) */
    const double     LOG10_2        = 0.301029995663981195213738894724;	/* log10(2) */
    const double     SQRT_PI        = 1.772453850905516027298167483341;	/* sqrt(pi) */
    const double     SQRT_2PI       = 2.506628274631000502416;	/* sqrt(2*pi) */
    const double     ONE_SQRT_2PI     = 0.398942280401432677939946059934;	/* 1/sqrt(2pi) */
    const double     SQRT_2dPI      = 0.797884560802865355879892119869;	/* sqrt(2/pi) */
    const double     LN_SQRT_PI     = 0.572364942924700087071713675677;	/* log(sqrt(pi)) == log(pi)/2 */
    const double     LN_SQRT_2PI	= 0.918938533204672741780329736406;	/* log(sqrt(2*pi)) == log(2*pi)/2 */
    const double     LN_SQRT_PId2	= 0.225791352644727432363097614947;	/* log(sqrt(pi/2)) */
    
}

#endif
