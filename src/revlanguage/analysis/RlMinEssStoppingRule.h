#ifndef RlMinEssStoppingRule_H
#define RlMinEssStoppingRule_H

#include "FileMonitor.h"
#include "RlAbstractConvergenceStoppingRule.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * RevLanguage wrapper class for the min-ESS stopping rule of an analysis.
     *
     *
     * The wraper class provides the Rev interface to the core class MinEssStoppingRule.
     * See MinEssStoppingRule.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-12, version 1.0
     */
    class MinEssStoppingRule : public AbstractConvergenceStoppingRule {
        
    public:
        
        MinEssStoppingRule(void);                                                                                                           //!< Default constructor
        
        // Basic utility functions
        virtual MinEssStoppingRule*                 clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal Filemonitor.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                 getConstructorFunctionName(void) const;                                                 //!< Get the name used for the constructor function in Rev.
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
    protected:
        
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        RevPtr<const RevVariable>                   minEss;
        
    };
    
}

#endif
