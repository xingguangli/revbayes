//
//  Func_checkNodeOrderConstraints.cpp
//  RevLanguage
//
//  Created by Bastien Boussau on 11/23/15.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "Func_checkNodeOrderConstraints.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRelativeNodeAgeConstraints.h"
#include "CheckNodeOrderConstraintsFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_checkNodeOrderConstraints::Func_checkNodeOrderConstraints( void ) : TypedFunction<RlBoolean>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_checkNodeOrderConstraints* Func_checkNodeOrderConstraints::clone( void ) const
{
    
    return new Func_checkNodeOrderConstraints( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::Boolean>* Func_checkNodeOrderConstraints::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RelativeNodeAgeConstraints>* c = static_cast<const RlRelativeNodeAgeConstraints &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    try {
        RevBayesCore::CheckNodeOrderConstraintsFunction* f = new RevBayesCore::CheckNodeOrderConstraintsFunction( tau, c );
        return f;
    }
    catch (RbException e)
    {
        std::cerr << e.getMessage() << std::endl;
    }
    
    return NULL;
}


/* Get argument rules */
const ArgumentRules& Func_checkNodeOrderConstraints::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree which is used to compute the MRCA.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "constraints", RlRelativeNodeAgeConstraints::getClassTypeSpec()   , "The constraints on relative node ages that need to be tested on the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_checkNodeOrderConstraints::getClassType(void)
{
    
    static std::string revType = "Func_checkNodeOrderConstraints";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_checkNodeOrderConstraints::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_checkNodeOrderConstraints::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "checkNodeOrderConstraints";
    
    return f_name;
}


const TypeSpec& Func_checkNodeOrderConstraints::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
