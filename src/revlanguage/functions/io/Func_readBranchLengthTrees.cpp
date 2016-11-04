#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readBranchLengthTrees.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlBranchLengthTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_readBranchLengthTrees* Func_readBranchLengthTrees::clone( void ) const
{
    
    return new Func_readBranchLengthTrees( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readBranchLengthTrees::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader reader = RevBayesCore::NclReader();
    
    ModelVector<BranchLengthTree> *trees = new ModelVector<BranchLengthTree>();
    std::vector<RevBayesCore::Tree*> tmp = *(reader.readBranchLengthTrees( fn.getValue() ));
    for (std::vector<RevBayesCore::Tree*>::iterator t = tmp.begin(); t != tmp.end(); ++t)
    {
        trees->push_back( BranchLengthTree(*t) );
    }
    
    return new RevVariable( trees );
}


/** Get argument rules */
const ArgumentRules& Func_readBranchLengthTrees::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
    
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "The name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readBranchLengthTrees::getClassType(void)
{
    
    static std::string revType = "Func_readBranchLengthTrees";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readBranchLengthTrees::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readBranchLengthTrees::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readBranchLengthTrees";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readBranchLengthTrees::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readBranchLengthTrees::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = ModelVector<BranchLengthTree>::getClassTypeSpec();
    return returnTypeSpec;
}




