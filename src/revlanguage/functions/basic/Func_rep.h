#ifndef Func_rep_H
#define Func_rep_H

#include "Procedure.h"
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Function that creates a numerical repuence by replicating an element n times.
     *
     * This function is the equivalent to the 'R' function rep().
     * The function creates a vector by replicating it n times:
     *
     * a <- rep(1.23,4)
     *
     * Which gives:
     * {1.23,1.23,1.23,1.23}
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-08-22
     *
     */
    
    template <typename valType>
    class Func_rep : public Procedure {
        
    public:
        Func_rep();
        
        // Basic utility functions
        Func_rep*                                       clone(void) const;                                          //!< Clone the object
        static const std::string&                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                     //!< Get class type spec
        std::string                                     getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                                 getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&                            getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<RevVariable>                             execute(void);                                              //!< Execute function
        
    protected:
        
        std::vector<std::string>                        getHelpAuthor(void) const;                                  //!< Get the author(s) of this function
        std::vector<std::string>                        getHelpDescription(void) const;                             //!< Get the description for this function
        std::vector<std::string>                        getHelpDetails(void) const;                                 //!< Get the more detailed description of the function
        std::string                                     getHelpExample(void) const;                                 //!< Get an executable and instructive example
        std::vector<RevBayesCore::RbHelpReference>      getHelpReferences(void) const;                              //!< Get some references/citations for this function
        std::vector<std::string>                        getHelpSeeAlso(void) const;                                 //!< Get suggested other functions
        std::string                                     getHelpTitle(void) const;                                   //!< Get the title of this help entry

    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"



template <typename valType>
RevLanguage::Func_rep<valType>::Func_rep() : Procedure()
{
    
}

/* Clone object */
template <typename valType>
RevLanguage::Func_rep<valType>* RevLanguage::Func_rep<valType>::clone( void ) const
{
    
    return new Func_rep( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
template <typename valType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::Func_rep<valType>::execute( void )
{
    
    typename valType::valueType v   = static_cast<const valType &>( args[0].getVariable()->getRevObject() ).getValue();
    int                         n   = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
    
    ModelVector<valType> *rep = new ModelVector<valType>();
    for ( int i=0; i<n; ++i )
    {
        rep->push_back( valType( v ) );
    }
    
    return new RevVariable( rep );
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_rep<valType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", valType::getClassTypeSpec(), "The value that we replicate.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "n", Natural::getClassTypeSpec(), "How often we replicate the value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
template <typename valType>
const std::string& RevLanguage::Func_rep<valType>::getClassType(void)
{
    
    static std::string revType = "Func_rep<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get class type spec describing type of object */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
template <typename valType>
std::string RevLanguage::Func_rep<valType>::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "rep";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_rep<valType>::getHelpAuthor(void) const
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
std::vector<std::string> RevLanguage::Func_rep<valType>::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "'rep' creates a vector of 'n' copies of the value 'x'." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_rep<valType>::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    details.push_back( "'rep' creates a vector of 'n' elements, each with value 'x', preserving the type of 'x' in the returned vector." );
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
template <typename valType>
std::string RevLanguage::Func_rep<valType>::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "rep(0.1, 3)\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
template <typename valType>
std::vector<RevBayesCore::RbHelpReference> RevLanguage::Func_rep<valType>::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
template <typename valType>
std::vector<std::string> RevLanguage::Func_rep<valType>::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "seq" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
template <typename valType>
std::string RevLanguage::Func_rep<valType>::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Create a vector of replicated values";
    
    return title;
}


/** Get type spec */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_rep<valType>::getReturnType( void ) const
{
    
    return ModelVector<valType>::getClassTypeSpec();
}


#endif


