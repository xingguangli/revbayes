#include "Func_hiddenStateRateMatrix.h"
#include "hiddenStateRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_hiddenStateRateMatrix::Func_hiddenStateRateMatrix( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_hiddenStateRateMatrix* Func_hiddenStateRateMatrix::clone( void ) const
{
    
    return new Func_hiddenStateRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_hiddenStateRateMatrix::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< double >* w = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* b = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    int n_states = static_cast<const Natural &>( this->args[2].getVariable()->getRevObject() ).getDagNode()->getValue();
    int n_cats   = static_cast<const Natural &>( this->args[3].getVariable()->getRevObject() ).getDagNode()->getValue();
    bool r = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getDagNode()->getValue();
    
    RevBayesCore::HiddenStateRateMatrixFunction* f = new RevBayesCore::HiddenStateRateMatrixFunction( w, b, n_states, n_cats, r );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_hiddenStateRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "withinCategoryRate", RealPos::getClassTypeSpec(),   "The transition rates between states within a category.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "betweenCategoryRate",RealPos::getClassTypeSpec(),   "The transition rates between states between categories.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "nStates",            Natural::getClassTypeSpec(), "The number of (original) states in a category.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "nCategories",        Natural::getClassTypeSpec(), "The number of categories (hidden states).", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rescaled",           RlBoolean::getClassTypeSpec(), "Should the matrix be normalized?", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_hiddenStateRateMatrix::getClassType(void)
{
    
    static std::string revType = "Func_hiddenStateRateMatrix";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_hiddenStateRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_hiddenStateRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnHiddenStateRateMatrix";
    
    return f_name;
}


const TypeSpec& Func_hiddenStateRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
