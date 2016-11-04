/**
 * @file
 * This file contains the implementation of Func_range, which is
 * the function used to compute range (colon) vectors.
 *
 * @brief Implementation of Func_range
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_source.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_range.h"
#include "Integer.h"
#include "ModelVector.h"
#include "RbException.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Default constructor */
Func_range::Func_range( void ) : Procedure() {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_range* Func_range::clone( void ) const {
    
    return new Func_range( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_range::execute( void )
{
    
    int f = static_cast<const Integer &>( args[0].getVariable()->getRevObject() ).getValue();
    int l = static_cast<const Integer &>( args[1].getVariable()->getRevObject() ).getValue();
    
    ModelVector<Integer> *range = new ModelVector<Integer>( RevBayesCore::RbVector<int>() );
    if (f < l)
    {
        for ( int i = f; i <= l; i++ )
        {
            range->push_back( Integer(i) );
        }
    }
    else
    {
        for ( int i = f; i >= l; i-- )
        {
            range->push_back( Integer(i) );
        }
    }
    
    return new RevVariable( range );
}


/** Get argument rules */
const ArgumentRules& Func_range::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "first", Integer::getClassTypeSpec(), "Lower value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "last" , Integer::getClassTypeSpec(), "Upper value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_range::getClassType(void)
{
    
    static std::string revType = "Func_range";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_range::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_range::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "range";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_range::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_range::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Create a sequence of number in the given range (interval)." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_range::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    details.push_back( "This function is a simplified version of the sequence function 'seq'. The range function creates a sequence of integer numbers with a step size of 1." );
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_range::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "range(1,20)\n";
    example += "range(20,-20)\n";
    example += "\n";
    example += "# this function is actually the same as the ':'\n";
    example += "20:-20\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_range::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_range::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "seq" );
    see_also.push_back( "rep" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_range::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "A range of consecutive integer numbers";
    
    return title;
}



/** Get type spec */
const TypeSpec& Func_range::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_range::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = ModelVector<Integer>::getClassTypeSpec();
    
    return returnTypeSpec;
}

