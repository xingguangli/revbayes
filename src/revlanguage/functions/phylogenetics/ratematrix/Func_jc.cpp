#include "Func_jc.h"
#include "JcRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "RlRateMatrix.h"

using namespace RevLanguage;

/** default constructor */
Func_jc::Func_jc( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_jc* Func_jc::clone( void ) const
{
    
    return new Func_jc( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_jc::createFunction( void ) const
{
    
    int ns = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::JcRateMatrixFunction* f = new RevBayesCore::JcRateMatrixFunction( size_t(ns) );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_jc::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "num_states", Natural::getClassTypeSpec(), "The number of state or state space.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_jc::getClassType(void)
{
    
    static std::string revType = "Func_jc";
    
	return revType; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_jc::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_jc::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnJC";
    
    return f_name;
}


const TypeSpec& Func_jc::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
