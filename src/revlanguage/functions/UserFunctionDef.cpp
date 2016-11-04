#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Signals.h"
#include "TypeSpec.h"
#include "UserFunctionDef.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;


/** Standard constructor */
UserFunctionDef::UserFunctionDef( const ArgumentRules*        argRules,
                                  const TypeSpec&             retType ,
                                  std::list<SyntaxElement*>*  stmts   ,
                                  std::string                 n)      :
    argumentRules( argRules ),
    code( stmts ),
    name( n ),
    referenceCount( 0 ),
    returnType( retType )
{
}


/** Copy constructor */
UserFunctionDef::UserFunctionDef( const UserFunctionDef& x ) :
    argumentRules( new ArgumentRules( *x.argumentRules ) ),
    code( NULL ),
    name( x.name ),
    referenceCount( 0 ),
    returnType( x.returnType )
{
    // Create a new list for the code
    code = new std::list<SyntaxElement*>();
    for ( std::list<SyntaxElement*>::const_iterator it = x.code->begin(); it != x.code->end(); ++it )
    {
        SyntaxElement* element = (*it)->clone();
        code->push_back( element );
    }
}


/** Assignment operator. Deal with argument rules and code. */
UserFunctionDef& UserFunctionDef::operator=( const UserFunctionDef &f )
{
    if ( this != &f ) {
        
        if ( returnType != f.returnType )
            throw RbException( "Illegal assignment of user-function definition involving different return types" );
        
        delete argumentRules;
        argumentRules = new ArgumentRules(*f.argumentRules);
        
        for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++)
        {
            SyntaxElement* theSyntaxElement = *it;
            delete theSyntaxElement;
        }
        code->clear();
        
        // Create a new list for the code
        for (std::list<SyntaxElement*>::const_iterator i=f.code->begin(); i!=f.code->end(); i++)
        {
            SyntaxElement* element = (*i)->clone();
            code->push_back(element);
        }
    }
    
    return *this;
}


/** Destructor */
UserFunctionDef::~UserFunctionDef()
{
    delete argumentRules;
    
    for (std::list<SyntaxElement*>::iterator it = code->begin(); it != code->end(); it++)
    {
        SyntaxElement* theSyntaxElement = *it;
        delete theSyntaxElement;
    }
    
    delete code;
}


/** Decrement reference count */
int UserFunctionDef::decrementReferenceCount( void )
{
    referenceCount--;

    return referenceCount;
}


/** Get argument rules */
const ArgumentRules& UserFunctionDef::getArgumentRules(void) const
{
    return *argumentRules;
}


/** Get code */
const std::list<SyntaxElement*>& UserFunctionDef::getCode(void) const
{
    return *code;
}

/** Get name */
const std::string& UserFunctionDef::getName(void) const
{
    return name;
}

/** Get reference count */
int UserFunctionDef::getReferenceCount( void ) const
{
    return referenceCount;
}


/** Get return type */
const TypeSpec& UserFunctionDef::getReturnType(void) const
{
    return returnType;
}


/** Increment reference count */
void UserFunctionDef::incrementReferenceCount( void )
{
    referenceCount++;
}

