#include "AbstractHomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DistanceMatrix.h"
#include "DistanceMatrixReader.h"
#include "Ellipsis.h"
#include "Func_readDistanceMatrix.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlDistanceMatrix.h"
#include "RlString.h"
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
Func_readDistanceMatrix* Func_readDistanceMatrix::clone( void ) const
{
	
	return new Func_readDistanceMatrix( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readDistanceMatrix::execute( void )
{
	
	// get the information from the arguments for reading the file
	const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	
	RevBayesCore::DistanceMatrixReader* dmr = new RevBayesCore::DistanceMatrixReader( fn.getValue(), ' ' );
	RevBayesCore::DistanceMatrix* dm = new RevBayesCore::DistanceMatrix(dmr);
		
	return new RevVariable( new RlDistanceMatrix(dm) );
}


/** Get argument rules */
const ArgumentRules& Func_readDistanceMatrix::getArgumentRules( void ) const
{
	
	static ArgumentRules argumentRules = ArgumentRules();
	static bool rules_set = false;
	
	if (!rules_set)
	{
		
		argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "Relative or absolute name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
		rules_set = true;
		
	}
	
	return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readDistanceMatrix::getClassType(void)
{
	
	static std::string revType = "Func_readDistanceMatrix";
	
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readDistanceMatrix::getClassTypeSpec(void)
{
	
	static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
	
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readDistanceMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readDistanceMatrix";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readDistanceMatrix::getTypeSpec( void ) const
{
	
	static TypeSpec type_spec = getClassTypeSpec();
	
	return type_spec;
}


/** Get return type */
const TypeSpec& Func_readDistanceMatrix::getReturnType( void ) const
{
	
	static TypeSpec returnTypeSpec = RlDistanceMatrix::getClassTypeSpec();
	return returnTypeSpec;
}

