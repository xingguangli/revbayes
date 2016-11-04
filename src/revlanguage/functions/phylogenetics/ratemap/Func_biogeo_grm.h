/**
 * @file
 * This file contains the declaration of the RevLanguage dec_ddd function,
 * which is used to created deterministic variable associated with the dec_ddd
 * function.
 *
 * @brief Declaration and implementation of Func_biogeo_grm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_biogeo_grm_H
#define Func_biogeo_grm_H

#include "RlGeographyRateModifier.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_biogeo_grm : public TypedFunction<GeographyRateModifier> {
        
    public:
        Func_biogeo_grm( void );
        
        // Basic utility functions
        Func_biogeo_grm*                                                            clone(void) const;                                          //!< Clone the object
        static const std::string&                                                   getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                                      getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                                                                 getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                                                             getTypeSpec(void) const;                                    //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::GeographyRateModifier >*         createFunction(void) const;                                 //!< Create internal function object
        const ArgumentRules&                                                        getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}

#endif
