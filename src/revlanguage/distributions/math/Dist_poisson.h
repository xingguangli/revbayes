#ifndef Dist_poisson_h
#define Dist_poisson_h

#include "PoissonDistribution.h"
#include "Natural.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Poisson distribution.
     *
     * The RevLanguage wrapper of the Poisson distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the Dist_poisson.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Nicolas Lartillot)
     * @since 2014-03-18, version 1.0
     *
     */
    class Dist_poisson :  public TypedDistribution<Natural> {
        
    public:
        Dist_poisson( void );                                                                                                                           //!< Default constructor
        
        // Basic utility functions
        Dist_poisson*                                   clone(void) const;                                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
        std::string                                     getDistributionFunctionName(void) const;                                                        //!< Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                                  //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::PoissonDistribution*              createDistribution(void) const;                                                                 //!< Create the internal distribution object
        
    protected:
        
        std::vector<std::string>                        getHelpAuthor(void) const;                                                              //!< Get the author(s) of this function
        std::vector<std::string>                        getHelpDescription(void) const;                                                         //!< Get the description for this function
        std::vector<std::string>                        getHelpDetails(void) const;                                                             //!< Get the more detailed description of the function
        std::string                                     getHelpExample(void) const;                                                             //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>      getHelpReferences(void) const;                                                          //!< Get some references/citations for this function
        std::vector<std::string>                        getHelpSeeAlso(void) const;                                                             //!< Get suggested other functions
        std::string                                     getHelpTitle(void) const;                                                               //!< Get the title of this help entry

        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);               //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       lambda;                    
    };
    
}


#endif
