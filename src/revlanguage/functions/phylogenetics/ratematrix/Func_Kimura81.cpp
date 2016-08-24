#include "Func_Kimura81.h"
#include "Kimura81RateMatrixFunction.h"
#include "RateMatrix_Kimura80.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_Kimura81::Func_Kimura81( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_Kimura81* Func_Kimura81::clone( void ) const
{
    
    return new Func_Kimura81( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_Kimura81::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< double >* k1 = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* k2 = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* bf = static_cast<const Simplex &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::Kimura81RateMatrixFunction* f = new RevBayesCore::Kimura81RateMatrixFunction( k1, k2, bf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_Kimura81::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "kappa1" , RealPos::getClassTypeSpec(), "The transversion rate from purine to pyrimidine.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "kappa2" , RealPos::getClassTypeSpec(), "The transversion rate from pyrimidine to purine.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", Simplex::getClassTypeSpec(), "The stationary frequencies of the states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Simplex( RevBayesCore::RbVector<double>(4,0.25) ) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_Kimura81::getClassType(void)
{
    
    static std::string revType = "Func_Kimura81";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_Kimura81::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the alternative Rev names (aliases) for the function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Func_Kimura81::getFunctionNameAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "fnKimura81" );
    
    return a_names;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_Kimura81::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnK81";
    
    return f_name;
}


const TypeSpec& Func_Kimura81::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
