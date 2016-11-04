//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ExponentialFunction.h"
#include "Func_exp.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_exp::Func_exp( void ) : TypedFunction<RealPos>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_exp* Func_exp::clone( void ) const {
    
    return new Func_exp( *this );
}


RevBayesCore::TypedFunction<double>* Func_exp::createFunction() const
{
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialFunction* f = new RevBayesCore::ExponentialFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_exp::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", Real::getClassTypeSpec(), "A number.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_exp::getClassType(void)
{
    
    static std::string revType = "Func_exp";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_exp::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_exp::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "exp";
    
    return f_name;
}


const TypeSpec& Func_exp::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
