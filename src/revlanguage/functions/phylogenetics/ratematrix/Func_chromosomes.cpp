#include "ChromosomesRateMatrixFunction.h"
#include "Func_chromosomes.h"
#include "RateMatrix_Chromosomes.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_chromosomes::Func_chromosomes( void ) : TypedFunction<RateMatrix>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_chromosomes* Func_chromosomes::clone( void ) const
{
    
    return new Func_chromosomes( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_chromosomes::createFunction( void ) const {
    
    
    RevBayesCore::TypedDagNode< int >* n           = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* lambda   = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* delta    = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* rho      = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* mu       = static_cast<const RealPos &>( this->args[4].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* lambda_l = static_cast<const Real &>( this->args[5].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* delta_l  = static_cast<const Real &>( this->args[6].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::ChromosomesRateMatrixFunction* f = new RevBayesCore::ChromosomesRateMatrixFunction( n, lambda, delta, rho, mu, lambda_l, delta_l );
    
	return f;
}


/* Get argument rules */
const ArgumentRules& Func_chromosomes::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
      
        argumentRules.push_back( new ArgumentRule( "maxChromosomes", Natural::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "lambda"        , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "delta"         , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "rho"           , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "mu"            , RealPos::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "lambda_l"      , Real::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0.0) ) );
        argumentRules.push_back( new ArgumentRule( "delta_l"       , Real::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0.0) ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_chromosomes::getClassType(void)
{
    
    static std::string revType = "Func_chromosomes";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_chromosomes::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_chromosomes::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnChromosomes";
    
    return f_name;
}


const TypeSpec& Func_chromosomes::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
