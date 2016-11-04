//
//  Func_pomoRootFrequencies.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 22/8/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_pomoRootFrequencies.h"
#include "ModelVector.h"
#include "Natural.h"
#include "PomoRootFrequenciesFunction.h"
#include "RlAbstractCharacterData.h"
#include "RlDnaState.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "RlTaxon.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_pomoRootFrequencies::Func_pomoRootFrequencies( void ) : TypedFunction<Simplex>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_pomoRootFrequencies* Func_pomoRootFrequencies::clone( void ) const {
    
    return new Func_pomoRootFrequencies( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >* Func_pomoRootFrequencies::createFunction( void ) const
{
    //Four arguments, root_base_frequencies, root_polymorphism_proportion, Q, virtual_population_size
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rbf = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< double >* rpp = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator >* q = static_cast<const RateGenerator &>( this->args[2].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::PomoRootFrequenciesFunction* pomorf = new RevBayesCore::PomoRootFrequenciesFunction( rbf, rpp, q, n );
    
    
    return pomorf;
}


/* Get argument rules */
const ArgumentRules& Func_pomoRootFrequencies::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        //Four arguments, root_base_frequencies, root_polymorphism_proportion, Q, virtual_population_size

        argumentRules.push_back( new ArgumentRule( "root_base_frequencies"       , Simplex::getClassTypeSpec()   , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "root_polymorphism_proportion", Real::getClassTypeSpec()      , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "mutation_rate_matrix"        , RateGenerator::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "virtualNe"                   , Natural::getClassTypeSpec()   , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_pomoRootFrequencies::getClassType(void)
{
    
    static std::string revType = "Func_pomoRootFrequencies";
    
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_pomoRootFrequencies::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_pomoRootFrequencies::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "pomoRF";
    
    return f_name;
}


const TypeSpec& Func_pomoRootFrequencies::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
