#ifndef RlDiversityDependentPureBirthProcess_H
#define RlDiversityDependentPureBirthProcess_h

#include "DiversityDependentPureBirthProcess.h"
#include "RlBirthDeathProcess.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the DiversityDependentPureBirthProcess
     *
     * The RevLanguage wrapper of the diversity-dependent pure-birth process connects
     * the variables/parameters of the process and creates the internal DiversityDependentPureBirthProcess object.
     * Please read the DiversityDependentPureBirthProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class DiversityDependentPureBirthProcess :  public BirthDeathProcess {
        
    public:
        DiversityDependentPureBirthProcess( void );
        
        // Basic utility functions
        DiversityDependentPureBirthProcess*                 clone(void) const;                                                                  //!< Clone the object
        static const std::string&                           getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&                                     getTypeSpec(void) const;                                                            //!< Get the type spec of the instance
        const MemberRules&                                  getMemberRules(void) const;                                                         //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::DiversityDependentPureBirthProcess*   createDistribution(void) const;                                                     //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
        
        void                                                setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);  //!< Set member variable
        
        
    private:
        
        RbPtr<const Variable>                               initialLambda;
        RbPtr<const Variable>                               capacity;
        
    };
    
}

#endif