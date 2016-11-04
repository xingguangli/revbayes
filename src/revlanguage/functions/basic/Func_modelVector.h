#ifndef Func_modelVector_H
#define Func_modelVector_H

#include "ModelVector.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    /**
     * @brief Func_modelVector: function creating model vectors
     *
     * This templated function constructs vectors and is used for language
     * constructs such as "v( x1, x2, ..., xn)" and "[ x1, x2, ..., xn ]" when
     * the elements are non-abstract model objects with non-abstract value types.
     */
    template <typename valType>
    class Func_modelVector : public TypedFunction< ModelVector< valType> > {
        
    public:
        Func_modelVector(void);                                                                 //!< Default constructor
        
        // Basic utility functions
        Func_modelVector*                                                                               clone(void) const;                                          //!< Clone the object
        static const std::string&                                                                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                                                          getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                                                                                     getFunctionName(void) const;
        const TypeSpec&                                                                                 getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename valType::valueType > >*            createFunction(void) const;                                 //!< Create a function object
        const ArgumentRules&                                                                            getArgumentRules(void) const;                               //!< Get argument rules
        
    protected:
        
        std::vector<std::string>                                                                        getHelpAuthor(void) const;                                  //!< Get the author(s) of this function
        std::vector<std::string>                                                                        getHelpDescription(void) const;                             //!< Get the description for this function
        std::vector<std::string>                                                                        getHelpDetails(void) const;                                 //!< Get the more detailed description of the function
        std::string                                                                                     getHelpExample(void) const;                                 //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>                                                      getHelpReferences(void) const;                              //!< Get some references/citations for this function
        std::vector<std::string>                                                                        getHelpSeeAlso(void) const;                                 //!< Get suggested other functions
        std::string                                                                                     getHelpTitle(void) const;                                   //!< Get the title of this help entry

    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorFunction.h"


/** Default constructor */
template <typename valType>
RevLanguage::Func_modelVector<valType>::Func_modelVector() :
    TypedFunction< ModelVector<valType> >()
{
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
template <typename valType>
RevLanguage::Func_modelVector<valType>* RevLanguage::Func_modelVector<valType>::clone( void ) const
{
    return new Func_modelVector( *this );
}


/** Execute function: create deterministic ModelVector<valType> object */
template <typename valType>
RevBayesCore::TypedFunction< RevBayesCore::RbVector< typename valType::valueType> >* RevLanguage::Func_modelVector<valType>::createFunction( void ) const
{
    std::vector<const RevBayesCore::TypedDagNode<typename valType::valueType>* > params;
    for ( size_t i = 0; i < this->args.size(); i++ )
    {
        const valType &val = static_cast<const valType &>( this->args[i].getVariable()->getRevObject() );
        params.push_back( val.getDagNode() );
    }
    
    RevBayesCore::VectorFunction<typename valType::valueType>* func = new RevBayesCore::VectorFunction<typename valType::valueType>( params );
    
    return func;
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_modelVector<valType>::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "", valType::getClassTypeSpec(), "first value", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new Ellipsis ( "more values", valType::getClassTypeSpec() ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
template <typename valType>
const std::string& RevLanguage::Func_modelVector<valType>::getClassType( void )
{
    static std::string revType = "Func_modelVector<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getClassTypeSpec( void )
{
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
template <typename valType>
std::string RevLanguage::Func_modelVector<valType>::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "v";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_modelVector<valType>::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Michael Landis" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_modelVector<valType>::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "'v' creates a vector of the elements '...'" );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_modelVector<valType>::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    details.push_back( "'v' creates a vector of the elements '...', which are objects of a common base type. Vector elements may themselves be vectors." );
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
template <typename valType>
std::string RevLanguage::Func_modelVector<valType>::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";

    example += "# create a vector, Natural[]\n";
    example += "x <- v(1,2,3)\n";
    example += "x <- x + 1\n";
    example += "x\n";
    example += "\n";
    example += "y <- v(2,4,6)\n";
    example += "# create a vector of Natural[][]\n";
    example += "z <- v(x,y)\n";
    example += "z\n";
    example += "z[0]\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
template <typename valType>
std::vector<RevBayesCore::RbHelpReference> RevLanguage::Func_modelVector<valType>::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_modelVector<valType>::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "simplex" );
    see_also.push_back( "rep" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
template <typename valType>
std::string RevLanguage::Func_modelVector<valType>::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Create a vector";
    
    return title;
}


/** Get Rev type spec of object (dynamic version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getTypeSpec( void ) const
{
    return this->getClassTypeSpec();
}


#endif

