#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readStochasticVariableTrace.h"
#include "ModelTrace.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlString.h"
#include "RlUtils.h"
#include "RlModelTrace.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "TraceReader.h"
#include "WorkspaceVector.h"

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
Func_readStochasticVariableTrace* Func_readStochasticVariableTrace::clone( void ) const
{
    
    return new Func_readStochasticVariableTrace( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readStochasticVariableTrace::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string&     fn       = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    // get the column delimiter
    const std::string& delimiter    = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    
    
    RevBayesCore::TraceReader reader;
    std::vector<RevBayesCore::ModelTrace> data = reader.readStochasticVariableTrace(fn, delimiter);
    
    WorkspaceVector<ModelTrace> *rv = new WorkspaceVector<ModelTrace>();
    for (std::vector<RevBayesCore::ModelTrace>::iterator it = data.begin(); it != data.end(); ++it)
    {
        rv->push_back( ModelTrace( *it ) );
    }
    
    // return the vector of traces
    return new RevVariable( rv );
}


/** Get argument rules */
const ArgumentRules& Func_readStochasticVariableTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec(), "The name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), "The delimiter used between the output of variables.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readStochasticVariableTrace::getClassType(void)
{
    
    static std::string revType = "Func_readStochasticVariableTrace";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readStochasticVariableTrace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readStochasticVariableTrace::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readStochasticVariableTrace";
    
    return f_name;
}

/** Get type spec */
const TypeSpec& Func_readStochasticVariableTrace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readStochasticVariableTrace::getReturnType( void ) const
{
    static TypeSpec returnTypeSpec = WorkspaceVector<ModelTrace>::getClassTypeSpec();
    return returnTypeSpec;
}


