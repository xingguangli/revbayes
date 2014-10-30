#include "TestingDefs.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "FilteredPhyloCTMCSiteHomogeneous.h"
#include "PhyloCTMCSiteHomogeneous.h"
#include "JcRateMatrixFunction.h"
#include "NclReader.h"
#include "NormalizeVectorFunction.h"
#include "QuantileFunction.h" 
#include "RbFileManager.h"
#include "StochasticNode.h"
#include "TestFilteredStandardLikelihood.h"
#include "BranchLengthTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "GammaDistribution.h"
#include "PhyloCTMCSiteHomogeneousNucleotide.h"
#include "GtrRateMatrixFunction.h"
#include "TimeTree.h"
#include "TreeUtilities.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "RbVector.h"

#include <vector>

using namespace RevBayesCore;

TestFilteredStandardLikelihood::TestFilteredStandardLikelihood(const std::string &afn, const std::string &tFn) : alignmentFilename( afn ), treeFilename( tFn ) {
}

TestFilteredStandardLikelihood::~TestFilteredStandardLikelihood() {
}


bool TestFilteredStandardLikelihood::run( void ) {
    std::cerr << "  starting TestFilteredStandardLikelihood...\n" ;
    /* First, we read in the data */
    // the matrix
    NclReader& reader = NclReader::getInstance();
    std::vector<AbstractCharacterData*> data = reader.readMatrices(alignmentFilename);
    AbstractDiscreteCharacterData * discrD = dynamic_cast<AbstractDiscreteCharacterData *>(data[0]);
#   if defined(USE_TIME_TREE)
        std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( treeFilename );
        ConstantNode<TimeTree> *tau = new ConstantNode<TimeTree>( "tau", new TimeTree( *trees[0] ) );
#   else
        std::vector<BranchLengthTree*> *trees = NclReader::getInstance().readBranchLengthTrees( treeFilename );
        ConstantNode<BranchLengthTree> *tau = new ConstantNode<BranchLengthTree>( "tau", new BranchLengthTree( *(*trees)[0] ) );
#   endif
    std::cout << "tau:\t" << tau->getValue() << std::endl;
    size_t numChar = discrD->getNumberOfCharacters();

#   if defined(USE_RATE_HET)
        ConstantNode<double> *alpha = new ConstantNode<double>("alpha", new double(0.5) );
        std::cout << "alpha:\t" << alpha->getValue() << std::endl;
        ConstantNode<double> *q1 = new ConstantNode<double>("q1", new double(0.125) );
        DeterministicNode<double> *q1_value = new DeterministicNode<double>("q1_value", new QuantileFunction(q1, new GammaDistribution(alpha, alpha) ) );
        ConstantNode<double> *q2 = new ConstantNode<double>("q2", new double(0.375) );
        DeterministicNode<double> *q2_value = new DeterministicNode<double>("q2_value", new QuantileFunction(q2, new GammaDistribution(alpha, alpha) ) );
        ConstantNode<double> *q3 = new ConstantNode<double>("q3", new double(0.625) );
        DeterministicNode<double> *q3_value = new DeterministicNode<double>("q3_value", new QuantileFunction(q3, new GammaDistribution(alpha, alpha) ) );
        ConstantNode<double> *q4 = new ConstantNode<double>("q4", new double(0.875) );
        DeterministicNode<double> *q4_value = new DeterministicNode<double>("q4_value", new QuantileFunction(q4, new GammaDistribution(alpha, alpha) ) );
        std::vector<const TypedDagNode<double>* > gamma_rates = std::vector<const TypedDagNode<double>* >();
        gamma_rates.push_back(q1_value);
        gamma_rates.push_back(q2_value);
        gamma_rates.push_back(q3_value);
        gamma_rates.push_back(q4_value);
        DeterministicNode<RbVector<double> > *site_rates = new DeterministicNode<RbVector<double> >( "site_rates", new VectorFunction<double>(gamma_rates) );
        ConstantNode<double> *sumNV = new ConstantNode<double>("sumnv", new double(1.0) );
        DeterministicNode<RbVector<double> > *site_rates_norm = new DeterministicNode<RbVector<double> >( "site_rates_norm", new NormalizeVectorFunction(site_rates, sumNV) );
        std::cout << "rates:\t" << site_rates->getValue() << std::endl;
        std::cout << "rates:\t" << site_rates_norm->getValue() << std::endl;
#   endif

#if defined(USE_3_STATES) && defined(USE_NUCLEOTIDE)
#error "cannot use 3 state and nucleotide type"
#endif
#if defined(USE_3_STATES) && defined(USE_GTR_RATE_MAT)
#error "cannot use 3 state and USE_GTR_RATE_MAT"
#endif

#   if defined(USE_GTR_RATE_MAT)
        ConstantNode<RbVector<double> > *pi = new ConstantNode<RbVector<double> >( "pi", new RbVector<double>(4, 1.0/4.0) );
        ConstantNode<RbVector<double> > *er = new ConstantNode<RbVector<double> >( "er", new RbVector<double>(6, 1.0/6.0) );
        DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );
        std::cout << "Q:\t" << q->getValue() << std::endl;
#       if defined (USE_NUCLEOTIDE)
#           if defined(USE_TIME_TREE)
                PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<DnaState, TimeTree>(tau, false, numChar);
#           else
                PhyloCTMCSiteHomogeneousNucleotide<DnaState, BranchLengthTree> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<DnaState, BranchLengthTree>(tau, false, numChar );
#           endif
#       else
#           if defined(USE_TIME_TREE)
                PhyloCTMCSiteHomogeneous<DnaState, TimeTree> *charModel = new PhyloCTMCSiteHomogeneous<DnaState, TimeTree>(tau, 4, false, numChar);
#           else
                PhyloCTMCSiteHomogeneous<DnaState, BranchLengthTree> *charModel = new PhyloCTMCSiteHomogeneous<DnaState, BranchLengthTree>(tau, 4, false, numChar );
#           endif
#       endif
#   else
        DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new JcRateMatrixFunction(
#                                                                                                           if defined(USE_3_STATES)
                                                                                                                3
#                                                                                                           else
                                                                                                                4
#                                                                                                           endif
                                                                                                            ));
#       if defined (USE_NUCLEOTIDE)
#          if defined(USE_TIME_TREE)
                PhyloCTMCSiteHomogeneousNucleotide<StandardState, TimeTree> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<StandardState, TimeTree>(tau, false, numChar);
#           else
                PhyloCTMCSiteHomogeneousNucleotide<StandardState, BranchLengthTree> *charModel = new PhyloCTMCSiteHomogeneousNucleotide<StandardState, BranchLengthTree>(tau, false, numChar );
#           endif
#       else
#          if defined(USE_TIME_TREE)
                PhyloCTMCSiteHomogeneous<StandardState, TimeTree> *charModel = new PhyloCTMCSiteHomogeneous<StandardState, TimeTree>(tau, 4, false, numChar);
#           else
                PhyloCTMCSiteHomogeneous<StandardState, BranchLengthTree> *charModel = new PhyloCTMCSiteHomogeneous<StandardState, BranchLengthTree>(tau, 
#                                                                                                               if defined(USE_3_STATES)
                                                                                                                    3,
#                                                                                                               else
                                                                                                                    4,
#                                                                                                               endif
                                                                                                                     false, numChar );
#           endif
#       endif
#   endif
#   if defined(USE_RATE_HET)
        charModel->setSiteRates( site_rates_norm );
#   endif
    charModel->setRateMatrix( q );
    StochasticNode< AbstractDiscreteCharacterData > *charactermodel = new StochasticNode< AbstractDiscreteCharacterData >("S", charModel);
    charactermodel->clamp( discrD );
    double lnp = charactermodel->getLnProbability();
    std::cerr << "    lnProb = " << lnp << std::endl;
    if (lnp >= 0.0) {
        std::cerr << "    lnProb is too high!" << std::endl;
        return false;
    }
    std::cout << "RevBayes LnL:\t\t" << charactermodel->getLnProbability() << std::endl;
    std::cout << "Finished GTR+Gamma model test." << std::endl;
    return true;
}
