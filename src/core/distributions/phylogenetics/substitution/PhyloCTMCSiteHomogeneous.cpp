#include <cstdlib>
#include <vector>
#include <cmath>
#include "PhyloCTMCSiteHomogeneous.h"
#include "DebugIO.h"

double RevBayesCore::computeRootLikelihood2Nodes(const double *p_left,
                                                      const double *p_right,
                                                      const size_t numSiteRates,
                                                      const double * rootFreq,
                                                      const size_t numStates,
                                                      const size_t * patternCounts,
                                                      const size_t numPatterns,
                                                      const size_t siteOffset,
                                                      const size_t nodeIndex,
                                                      const size_t mixtureOffset,
                                                      const double p_inv,
                                                      const std::vector<bool> & siteInvariant,
                                                      const std::vector<size_t> & invariantSiteIndex) {
    double lnProb = 0.0;
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    // get pointers the likelihood for both subtrees
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    debug_vec("p_left", p_left, numStates * numSiteRates * numPatterns);
    debug_vec("p_right", p_right, numStates * numSiteRates * numPatterns);
    debug_vec("rootFreq", rootFreq, numStates);
    debug_vec("patternCounts", patternCounts, numPatterns);
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        // iterate over all sites
        for (size_t site = 0; site < numPatterns; ++site) {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            for (size_t i = 0; i < numStates; ++i) {
                tmp += p_site_left_j[i] * p_site_right_j[i] * rootFreq[i];
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
        }
        p_mixture_left+=mixtureOffset; p_mixture_right+=mixtureOffset;
    }
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    if ( p_inv > 0.0 ) {
        for (size_t site = 0; site < numPatterns; ++site) {
            if ( siteInvariant[site] ) {
                lnProb += log( p_inv * rootFreq[ invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            } else {
                lnProb += log( oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
            }
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            lnProb += log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
        }
    }
    return lnProb;
}
double RevBayesCore::computeRootLikelihood3Nodes(const double *p_left,
                                                 const double *p_right,
                                                 const double *p_middle,
                                                 const size_t numSiteRates,
                                                 const double * rootFreq,
                                                 const size_t numStates,
                                                 const size_t * patternCounts,
                                                 const size_t numPatterns,
                                                 const size_t siteOffset,
                                                 const size_t nodeIndex,                                                
                                                 const size_t mixtureOffset,
                                                 const double p_inv,
                                                 const std::vector<bool> & siteInvariant,
                                                 const std::vector<size_t> & invariantSiteIndex) {
    double lnProb = 0.0;
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(numPatterns,0.0);
    const double*   p_mixture_left     = p_left;
    const double*   p_mixture_right    = p_right;
    const double*   p_mixture_middle   = p_middle;
    debug_vec("p_left", p_left, numStates * numSiteRates * numPatterns);
    debug_vec("p_right", p_right, numStates * numSiteRates * numPatterns);
    debug_vec("p_middle", p_middle, numStates * numSiteRates * numPatterns);
    debug_vec("rootFreq", rootFreq, numStates);
    debug_vec("patternCounts", patternCounts, numPatterns);
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        const double*   p_site_mixture_left     = p_mixture_left;
        const double*   p_site_mixture_right    = p_mixture_right;
        const double*   p_site_mixture_middle   = p_mixture_middle;
        for (size_t site = 0; site < numPatterns; ++site) {
            double tmp = 0.0;
            const double* p_site_left_j   = p_site_mixture_left;
            const double* p_site_right_j  = p_site_mixture_right;
            const double* p_site_middle_j = p_site_mixture_middle;
            // iterate over all starting states
            for (size_t i = 0; i < numStates; ++i) {
                tmp += p_site_left_j[i] * p_site_right_j[i] * p_site_middle_j[i] * rootFreq[i];
            }
            per_mixture_Likelihoods[site] += tmp;
            // increment the pointers to the next site
            p_site_mixture_left+=siteOffset;
            p_site_mixture_right+=siteOffset;
            p_site_mixture_middle+=siteOffset;
        } // end-for over all sites (=patterns)
        p_mixture_left+=mixtureOffset;
        p_mixture_right+=mixtureOffset;
        p_mixture_middle+=mixtureOffset;
    } // end-for over all mixtures (=rate categories)
    // sum the log-likelihoods for all sites together
    double oneMinusPInv = 1.0 - p_inv;
    if ( p_inv > 0.0 ) {
        for (size_t site = 0; site < numPatterns; ++site) {
            double prob;
            if ( siteInvariant[site] ) {
                prob = p_inv * rootFreq[ invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates;
            } else {
                prob = oneMinusPInv * per_mixture_Likelihoods[site] / numSiteRates;
            }
            lnProb += log(prob) * patternCounts[site];
        }
    } else {
        for (size_t site = 0; site < numPatterns; ++site) {
            lnProb += log( per_mixture_Likelihoods[site] / numSiteRates ) * patternCounts[site];
        }
    }
    return lnProb;
}

void RevBayesCore::computeInternalNodeLikelihood(double * p_node,
                                                             const double *p_left,
                                                             const double *p_right,
                                                             const size_t numSiteRates,
                                                             const size_t numStates,
                                                             const size_t numPatterns,
                                                             const size_t siteOffset,
                                                             const size_t nodeIndex,
                                                             const size_t mixtureOffset,
                                                             const double ** tpMats) {
    debug_vec("p_left", p_left, numStates * numSiteRates * numPatterns);
    debug_vec("p_right", p_right, numStates * numSiteRates * numPatterns);
    debug_mat("tpMats", tpMats, numSiteRates, numStates*numStates);
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        const double*    tp_begin                = tpMats[mixture];
        size_t offset = mixture*mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        for (size_t site = 0; site < numPatterns ; ++site) {
            const double*       tp_a    = tp_begin;
            for (size_t c1 = 0; c1 < numStates; ++c1) {
                double sum = 0.0;
                for (size_t c2 = 0; c2 < numStates; ++c2 ) {
                    sum += p_site_mixture_left[c2] * p_site_mixture_right[c2] * tp_a[c2];
                }
                p_site_mixture[c1] = sum;
                tp_a+=numStates;
            }
            p_site_mixture_left+=siteOffset; p_site_mixture_right+=siteOffset; p_site_mixture+=siteOffset;
        }
    }
    debug_vec("p_node", p_node, numStates * numSiteRates * numPatterns);
}

void RevBayesCore::computeTipNodeLikelihood(double * p_node,
                                             const size_t numSiteRates,
                                             const size_t numStates,
                                             const size_t numPatterns,
                                             const size_t siteOffset,
                                             const size_t nodeIndex,
                                             const size_t mixtureOffset,
                                             const double ** tpMats,
                                             const std::vector<bool> &gap_node,
                                             const std::vector<unsigned long> &char_node,
                                             const bool usingAmbiguousCharacters) {
    debug_vec("char_node", &(char_node[0]), numPatterns);
    debug_mat("tpMats", tpMats, numSiteRates, numStates*numStates);
    double*   p_mixture      = p_node;
    for (size_t mixture = 0; mixture < numSiteRates; ++mixture) {
        const double* tp_begin = tpMats[mixture];
        double*     p_site_mixture      = p_mixture;
        for (size_t site = 0; site != numPatterns; ++site) {
            if ( gap_node[site] ) {
                for (size_t c1 = 0; c1 < numStates; ++c1) {
                    p_site_mixture[c1] = 1.0;
                }
            } else {
                unsigned long org_val = char_node[site];
                for (size_t c1 = 0; c1 < numStates; ++c1) {
                    if ( usingAmbiguousCharacters ) {
                        // compute the likelihood that we had a transition from state c1 to the observed state org_val
                        // note, the observed state could be ambiguous!
                        unsigned long val = org_val;
                        // get the pointer to the transition probabilities for the terminal states
                        const double* d  = tp_begin+(numStates*c1);
                        double tmp = 0.0;
                        while ( val != 0 ) {
                            if ( (val & 1) == 1 ) {
                                tmp += *d;
                            }
                            val >>= 1;
                            ++d;
                        }
                        p_site_mixture[c1] = tmp;
                    } else {
                        p_site_mixture[c1] = tp_begin[c1*numStates+org_val];
                    }
                }
            }
            p_site_mixture += siteOffset; 
        }
        p_mixture+=mixtureOffset;
    }
    debug_vec("p_node", p_node, numStates * numSiteRates * numPatterns);
}

