#ifndef PowerPosteriorAnalysis_H
#define PowerPosteriorAnalysis_H

#include "Cloneable.h"
#include "Parallelizable.h"
#include "RbVector.h"

namespace RevBayesCore {
    
    class MonteCarloSampler;
    
    /**
     * @brief Power posterior analysis class.
     *
     * A power posterior analysis runs an analysis for a vector of powers
     * where the likelihood during each analysis run is raised to the given power.
     * The likelihood values and the current powers are stored in a file.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class PowerPosteriorAnalysis : public Cloneable, public Parallelizable {
        
    public:
        PowerPosteriorAnalysis(MonteCarloSampler *m, const std::string &fn, size_t k);
        PowerPosteriorAnalysis(const PowerPosteriorAnalysis &a);
        virtual                                ~PowerPosteriorAnalysis(void);                               //!< Virtual destructor
        
        PowerPosteriorAnalysis&                 operator=(const PowerPosteriorAnalysis &a);
        
        // public methods
        PowerPosteriorAnalysis*                 clone(void) const;
        void                                    burnin(size_t g, size_t ti);
        void                                    runAll(size_t g);
        void                                    runStone(size_t idx, size_t g);
        void                                    summarizeStones(void);
        void                                    setPowers(const std::vector<double> &p);
        void                                    setSampleFreq(size_t sf);
        
    private:
        
        void                                    initMPI(void);
        
        // members
        std::string                             filename;
        std::vector<double>                     powers;
        MonteCarloSampler*                      sampler;
        size_t                                  sampleFreq;                                                                     //!< The rate of the distribution
        size_t                                  processors_per_likelihood;

    };
    
}

#endif
