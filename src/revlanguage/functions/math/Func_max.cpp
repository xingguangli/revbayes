//
//  Func_max.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 02/11/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MaxFunction.h"
#include "Func_max.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_max::Func_max( void ) : TypedFunction<Real>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_max* Func_max::clone( void ) const
{
    
    return new Func_max( *this );
}


RevBayesCore::TypedFunction<double>* Func_max::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MaxFunction* f = new RevBayesCore::MaxFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_max::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of numbers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_max::getClassType(void)
{
    
    static std::string revType = "Func_max";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_max::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_max::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "max";
    
    return f_name;
}


const TypeSpec& Func_max::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
