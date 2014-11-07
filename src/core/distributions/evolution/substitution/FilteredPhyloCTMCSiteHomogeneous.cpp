#include <cstdlib>
#include <vector>
#include <cmath>
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "AscertainmentBiasCorrection.h"
using namespace RevBayesCore;
std::vector<RevBayesCore::AscertainmentBiasCorrectionStruct *> RevBayesCore::allocAscBiasCorrStructs(const size_t numCopies,
                                                                                                     const size_t numNodes,
                                                                                                     const size_t numStates,
                                                                                                     const size_t numMixtures) {
    std::vector<RevBayesCore::AscertainmentBiasCorrectionStruct *> x(numCopies*numNodes, 0L);
    try{ 
        for (size_t i = 0 ; i < numCopies*numNodes; ++i) {
            //x[i] = new RevBayesCore::VariableOnlyNoMissingAscertainmentBiasCorrectionStruct(numStates, numMixtures);
            x[i] = new RevBayesCore::MissingAwareVariableOnlyAscertainmentBiasCorrection(numStates, numMixtures);

        }
    } catch (...) {
        freeAscBiasCorrStructs(x);
        throw;
    }
    return x;
}

void RevBayesCore::freeAscBiasCorrStructs(std::vector<RevBayesCore::AscertainmentBiasCorrectionStruct *> &x) {
    for (size_t i = 0 ; i < x.size(); ++i) {
        delete x[i];
        x[i] = NULL;
    }
    x.clear();
}
double RevBayesCore::computeRootFilteredLikelihood2Nodes(const double *p_left,
                                   const double *p_right,
                                   const size_t numSiteRates,
                                   const double * rootFreq,
                                   const size_t numStates,
                                   const size_t * patternCounts,
                                   const size_t numPatterns,
                                   const size_t siteOffset,
                                   const size_t mixtureOffset,
                                   const double p_inv,
                                   const std::vector<bool> & siteInvariant,
                                   const std::vector<size_t> & invariantSiteIndex,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight,
                                   double & uncorrectedLnProb,
                                   double & ascBiasLnProb
                                   ) {
    ascBiasLnProb = ascLeft->computeAscBiasLnProbCorrection2Node(ascRight, numSiteRates, rootFreq, numStates, patternCounts, numPatterns,  p_inv, siteInvariant, invariantSiteIndex);
    uncorrectedLnProb = computeRootLikelihood2Nodes(p_left,
                                                      p_right,
                                                      numSiteRates,
                                                      rootFreq,
                                                      numStates,
                                                      patternCounts,
                                                      numPatterns,
                                                      siteOffset,
                                                      mixtureOffset,
                                                      p_inv,
                                                      siteInvariant,
                                                      invariantSiteIndex);
    return uncorrectedLnProb - ascBiasLnProb;
}
double RevBayesCore::computeRootFilteredLikelihood3Nodes(const double *p_left,
                                   const double *p_right,
                                   const double *p_middle,
                                   const size_t numSiteRates,
                                   const double * rootFreq,
                                   const size_t numStates,
                                   const size_t * patternCounts,
                                   const size_t numPatterns,
                                   const size_t siteOffset,
                                   const size_t mixtureOffset,
                                   const double p_inv,
                                   const std::vector<bool> & siteInvariant,
                                   const std::vector<size_t> & invariantSiteIndex,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight,
                                   const AscertainmentBiasCorrectionStruct *ascMiddle,
                                   double & uncorrectedLnProb,
                                   double & ascBiasLnProb) {
    ascBiasLnProb = ascLeft->computeAscBiasLnProbCorrection3Node(ascRight, ascMiddle, numSiteRates, rootFreq, numStates, patternCounts, numPatterns, p_inv, siteInvariant, invariantSiteIndex);
    uncorrectedLnProb = RevBayesCore::computeRootLikelihood3Nodes(p_left,
                                                 p_right,
                                                 p_middle,
                                                 numSiteRates,
                                                 rootFreq,
                                                 numStates,
                                                 patternCounts,
                                                 numPatterns,
                                                 siteOffset,
                                                 mixtureOffset,
                                                 p_inv,
                                                 siteInvariant,
                                                 invariantSiteIndex);
    return uncorrectedLnProb - ascBiasLnProb;
}
void RevBayesCore::computeInternalNodeFilteredLikelihood(double * p_node,
                                            AscertainmentBiasCorrectionStruct *ascNode,
                                    const double *p_left,
                                    const double *p_right,
                                    const size_t numSiteRates,
                                    const size_t numStates,
                                    const size_t numPatterns,
                                    const size_t siteOffset,
                                    const size_t mixtureOffset,
                                    const double ** tpMats,
                                   const AscertainmentBiasCorrectionStruct *ascLeft,
                                   const AscertainmentBiasCorrectionStruct *ascRight) {
    ascNode->computeInternalAscBias(ascLeft, ascRight, numSiteRates, numStates, numPatterns, tpMats);
    computeInternalNodeLikelihood(p_node, p_left, p_right, numSiteRates, numStates, numPatterns, siteOffset, mixtureOffset, tpMats);
}
void RevBayesCore::computeTipNodeFilteredLikelihood(double * p_node,
                                            AscertainmentBiasCorrectionStruct *ascNode,
                               const size_t numSiteRates,
                               const size_t numStates,
                               const size_t numPatterns,
                               const size_t siteOffset,
                               const size_t mixtureOffset,
                               const double ** tpMats,
                               const std::vector<bool> &gap_node,
                               const std::vector<unsigned long> &char_node,
                               const bool usingAmbiguousCharacters) {
    ascNode->computeTipAscBias(numSiteRates, numStates, numPatterns, tpMats, gap_node, char_node, usingAmbiguousCharacters);
    computeTipNodeLikelihood(p_node, numSiteRates, numStates, numPatterns, siteOffset, mixtureOffset, tpMats, gap_node, char_node, usingAmbiguousCharacters);
}

