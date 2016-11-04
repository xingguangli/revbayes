#ifndef ProbabilityMonitor_H
#define ProbabilityMonitor_H

#include "AbstractFileMonitor.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief A monitor class that monitors all stochastic variables of a model and prints their probability into a file.
     *
     * @file
     * The probability monitor is a convenience monitor that simply monitors all stochastic variables of a model.
     * The current probability of the values will be printed into a file.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-06-21, version 1.0
     *
     */
    class ProbabilityMonitor : public AbstractFileMonitor {
        
    public:
        // Constructors and Destructors
        ProbabilityMonitor(unsigned long g, const std::string &fname, const std::string &del);                                  //!< Constructor
        virtual ~ProbabilityMonitor(void);
        
        
        
        // basic methods
        ProbabilityMonitor*                 clone(void) const;                                                  //!< Clone the object
        
        // public (overloaded) methods
        void                                monitorVariables(unsigned long gen);                                //!< Monitor at generation gen

        
        // getters and setters
        void                                setModel(Model* m);
        
    private:
        // helper methods
        void                                resetDagNodes(void);                                                //!< Extract the variable to be monitored again.
        
    };
    
}

#endif

