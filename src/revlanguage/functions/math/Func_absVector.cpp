#include "AbsoluteValueVectorFunction.h"
#include "Func_absVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_absVector::Func_absVector( void ) : TypedFunction< ModelVector<RealPos> >( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_absVector* Func_absVector::clone( void ) const
{
    
    return new Func_absVector( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::RbVector<double> >* Func_absVector::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::AbsoluteValueVectorFunction* f = new RevBayesCore::AbsoluteValueVectorFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_absVector::getArgumentRules( void ) const
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


const std::string& Func_absVector::getClassType(void)
{
    
    static std::string revType = "Func_absVector";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_absVector::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<ModelVector<RealPos> >::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_absVector::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "abs";
    
    return f_name;
}



/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_absVector::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_absVector::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "The 'abs' function returns the absolute value of a number." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_absVector::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_absVector::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "# compute the absolute value of a real number\n";
    example += "number <- -3.0\n";
    example += "absoluteValueOfTheNumber <- abs(number)\n";
    example += "if (number + absoluteValueOfTheNumber != 0.0) {\n";
    example += "    print(\"Problem when computing an absolute value.\")\n";
    example += "} else {\n";
    example += "    print(\"Correct computation of an absolute value.\")\n";
    example += "}\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_absVector::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_absVector::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "ceil" );
    see_also.push_back( "floor" );
    see_also.push_back( "round" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_absVector::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Absolute value of a number";
    
    return title;
}


const TypeSpec& Func_absVector::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
