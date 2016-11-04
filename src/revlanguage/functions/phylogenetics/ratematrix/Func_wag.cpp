#include "DeterministicNode.h"
#include "Func_wag.h"
#include "Natural.h"
#include "RateMatrix_Wag.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_wag::Func_wag( void ) : Procedure( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_wag* Func_wag::clone( void ) const
{
    
    return new Func_wag( *this );
}


RevPtr<RevVariable> Func_wag::execute()
{
    
    RevBayesCore::RateMatrix_Wag *rmj = new RevBayesCore::RateMatrix_Wag();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_wag::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_wag::getClassType(void)
{
    
    static std::string revType = "Func_wag";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_wag::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_wag::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnWAG";
    
    return f_name;
}


/* Get return type */
const TypeSpec& Func_wag::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RateGenerator::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_wag::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
