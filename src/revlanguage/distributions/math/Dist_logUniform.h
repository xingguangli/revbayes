#ifndef Dist_logUniform_H
#define Dist_logUniform_H

#include "LogUniformDistribution.h"
#include "RlPositiveContinuousDistribution.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the log uniform distribution.
     *
     * The RevLanguage wrapper of the log uniform distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the LoguniforDistribution for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna and Nicolas Lartillot)
     * @since 2012-08-08, version 1.0
     *
     */
    class Dist_logUniform :  public PositiveContinuousDistribution {
        
    public:
        Dist_logUniform( void );
        virtual ~Dist_logUniform();
        
        // Basic utility functions
        Dist_logUniform*                                clone(void) const;                                                                      //!< Clone the object
        static const std::string&                       getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::string                                     getDistributionFunctionName(void) const;                                                //!< Get the Rev-name for this distribution.
        const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                          //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                                      //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::LogUniformDistribution*           createDistribution(void) const;
        
    protected:
        
        std::vector<std::string>                        getHelpAuthor(void) const;                                                              //!< Get the author(s) of this function
        std::vector<std::string>                        getHelpDescription(void) const;                                                         //!< Get the description for this function
        std::vector<std::string>                        getHelpDetails(void) const;                                                             //!< Get the more detailed description of the function
        std::string                                     getHelpExample(void) const;                                                             //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>      getHelpReferences(void) const;                                                          //!< Get some references/citations for this function
        std::vector<std::string>                        getHelpSeeAlso(void) const;                                                             //!< Get suggested other functions
        std::string                                     getHelpTitle(void) const;                                                               //!< Get the title of this help entry

        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        
    private:
        RevPtr<const RevVariable>                       min;
        RevPtr<const RevVariable>                       max;
        
    };
    
}

#endif
