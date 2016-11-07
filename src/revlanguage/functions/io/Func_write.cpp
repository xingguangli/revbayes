#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_write.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"

#include <string>

#if defined (RB_MPI)
#include <mpi.h>
#endif

using namespace RevLanguage;


Func_write::Func_write( void ) :
    processID( 0 )
{
#if defined (RB_MPI)
    processID = MPI::COMM_WORLD.Get_rank();
#endif
}

using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_write* Func_write::clone( void ) const
{
    
    return new Func_write( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_write::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string& fn = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    bool  append = static_cast<const RlBoolean&>( args[2].getVariable()->getRevObject() ).getValue();
    const std::string& separator = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    
    if ( processID == 0 )
    {
    if ( fn != "" ) 
    {
        std::ofstream outStream;
        
        if ( append == true )
        {
            
            // open the stream to the file
            outStream.open(fn.c_str(), std::fstream::out | std::fstream::app);
        }
        else 
        {
            
            // open the stream to the file
            outStream.open(fn.c_str(), std::fstream::out);
        }
        
        // print the arguments
        args[0].getVariable()->getRevObject().printValue(outStream, false);
        for (size_t i = 4; i < args.size(); i++) 
        {
            outStream << separator;
            args[i].getVariable()->getRevObject().printValue( outStream , false );
        }
        
        outStream.close();
    }
    else
    {
        
        std::ostream& o = std::cout;
        
        // print the arguments
        args[0].getVariable()->getRevObject().printValue( o, false );
        for (size_t i = 4; i < args.size(); i++) 
        {
            o << separator;
            args[i].getVariable()->getRevObject().printValue( o, false );
        }
        o << std::endl;
    }
    }

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_write::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set) 
    {
        
        argumentRules.push_back( new ArgumentRule( "", RevObject::getClassTypeSpec(), "A variable to write.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new Ellipsis( "Additional variables to write.", RevObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "filename" , RlString::getClassTypeSpec() , "Writing to this file, or to the screen if name is empty.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("") ) );
        argumentRules.push_back( new ArgumentRule( "append"   , RlBoolean::getClassTypeSpec(), "Append or overwrite existing file?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        argumentRules.push_back( new ArgumentRule( "separator", RlString::getClassTypeSpec() , "How to separate values between variables.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_write::getClassType(void)
{
    
    static std::string revType = "Func_write";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_write::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Func_write::getFunctionNameAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "print" );
    
    return a_names;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_write::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "write";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_write::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_write::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RevNullObject::getClassTypeSpec();
    return returnTypeSpec;
}




