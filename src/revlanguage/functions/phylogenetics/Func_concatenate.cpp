#include "Argument.h"
#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_concatenate.h"
#include "RbException.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "Workspace.h"

using namespace RevLanguage;

/** Default constructor */
Func_concatenate::Func_concatenate( void ) : Procedure()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_concatenate* Func_concatenate::clone( void ) const
{
    
    return new Func_concatenate( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_concatenate::execute( void )
{
    
    const AbstractHomologousDiscreteCharacterData& a = static_cast<const AbstractHomologousDiscreteCharacterData &>( args[0].getVariable()->getRevObject() );
    const AbstractHomologousDiscreteCharacterData& b = static_cast<const AbstractHomologousDiscreteCharacterData &>( args[1].getVariable()->getRevObject() );
    
    AbstractHomologousDiscreteCharacterData* d = a.concatenate( b );
    for (size_t i = 2; i < args.size(); ++i)
    {
        const AbstractHomologousDiscreteCharacterData& c = static_cast<const AbstractHomologousDiscreteCharacterData &>( args[i].getVariable()->getRevObject() );
        
        AbstractHomologousDiscreteCharacterData* tmp = d->concatenate( c );
        delete d;
        
        d = tmp;
    }
    
    return new RevVariable( d );
}


/** Get argument rules */
const ArgumentRules& Func_concatenate::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "a", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "First character data object.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "v", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "Second character data object.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new Ellipsis( "Additional character data objects.", AbstractHomologousDiscreteCharacterData::getClassTypeSpec() ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_concatenate::getClassType(void)
{
    
    static std::string revType = "Func_concatenate";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_concatenate::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_concatenate::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "concatenate";
    
    return f_name;
}

/** Get type spec */
const TypeSpec& Func_concatenate::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_concatenate::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlBoolean::getClassTypeSpec();
    
    return returnTypeSpec;
}

