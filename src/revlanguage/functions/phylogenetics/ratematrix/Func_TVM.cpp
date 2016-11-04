#include "TvmRateMatrixFunction.h"
#include "Func_TVM.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_TVM::Func_TVM( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_TVM* Func_TVM::clone( void ) const
{
    
    return new Func_TVM( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_TVM::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    if ( bf->getValue().size() != 4 )
    {
        throw RbException("The TVM rate matrix requires 4 base frequencies.");
    }
    
    if ( er->getValue().size() != 5 )
    {
        throw RbException("The TVM rate matrix requires 5 rate parameters.");
    }
    
    RevBayesCore::TvmRateMatrixFunction* f = new RevBayesCore::TvmRateMatrixFunction( er, bf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_TVM::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "exchangeRates"  , Simplex::getClassTypeSpec(), "The exchangeability rates between states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", Simplex::getClassTypeSpec(), "The stationary frequencies of the states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_TVM::getClassType(void)
{
    
    static std::string revType = "Func_TVM";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_TVM::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_TVM::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnTVM";
    
    return f_name;
}


const TypeSpec& Func_TVM::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
