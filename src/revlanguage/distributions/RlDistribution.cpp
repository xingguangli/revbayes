#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OptionRule.h"
#include "RlDistribution.h"
#include "StringUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Distribution::Distribution() : RevObject() 
{
    
}


/**
 * Default destructor.
 * 
 * The default destructor does nothing except deleting the object.
 */
Distribution::~Distribution() 
{
    
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Distribution::getClassType(void) 
{ 
    
    static std::string revType = "Distribution";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Distribution::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the aliases of the Rev name for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Distribution::getConstructorFunctionAliases( void ) const
{
    // create a constructor function name alias variable that is the same for all instance of this class
    std::vector<std::string> aliases;
    
    std::vector<std::string> dist_aliases = getDistributionFunctionAliases();
    for (size_t i=0; i < dist_aliases.size(); ++i)
    {
        std::string tmp = dist_aliases[i];
        std::string c_name = "dn" + StringUtilities::firstCharToUpper( tmp );
        
        aliases.push_back( c_name );
    }
    
    return aliases;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Distribution::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string tmp = getDistributionFunctionName();
    std::string c_name = "dn" + StringUtilities::firstCharToUpper( tmp );
    
    return c_name;
}


RevBayesCore::RbHelpDistribution* Distribution::constructTypeSpecificHelp( void ) const
{
    
    return new RevBayesCore::RbHelpDistribution();
}


/** 
 * Get the help entry for this class 
 */
void Distribution::addSpecificHelpFields(RevBayesCore::RbHelpEntry *e) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpDistribution *help = static_cast<RevBayesCore::RbHelpDistribution*>( e );
    RevBayesCore::RbHelpDistribution &helpEntry = *help;
    
    // create the constructor
    RevBayesCore::RbHelpFunction help_constructor = RevBayesCore::RbHelpFunction();

    // usage
    help_constructor.setUsage( getConstructorUsage() );
    
    // arguments
    const MemberRules& rules = getParameterRules();
    std::vector<RevBayesCore::RbHelpArgument> arguments = std::vector<RevBayesCore::RbHelpArgument>();
    
    for ( size_t i=0; i<rules.size(); ++i )
    {
        const ArgumentRule &the_rule = rules[i];
        
        RevBayesCore::RbHelpArgument argument = RevBayesCore::RbHelpArgument();
        
        argument.setLabel( the_rule.getArgumentLabel() );
        argument.setDescription( the_rule.getArgumentDescription() );
        
        std::string type = "<any>";
        if ( the_rule.getArgumentDagNodeType() == ArgumentRule::CONSTANT )
        {
            type = "<constant>";
        }
        else if ( the_rule.getArgumentDagNodeType() == ArgumentRule::STOCHASTIC )
        {
            type = "<stochastic>";
        }
        else if ( the_rule.getArgumentDagNodeType() == ArgumentRule::DETERMINISTIC )
        {
            type = "<deterministic>";
        }
        argument.setArgumentDagNodeType( type );
        
        std::string passing_method = "pass by value";
        if ( the_rule.getEvaluationType() == ArgumentRule::BY_CONSTANT_REFERENCE )
        {
            passing_method = "pass by const reference";
        }
        else if ( the_rule.getEvaluationType() == ArgumentRule::BY_REFERENCE )
        {
            passing_method = "pass by reference";
        }
        argument.setArgumentPassingMethod(  passing_method );
        
        argument.setValueType( the_rule.getArgumentTypeSpec()[0].getType() );

        if ( the_rule.hasDefault() )
        {
            std::stringstream ss;
            the_rule.getDefaultVariable().getRevObject().printValue( ss, true);
            argument.setDefaultValue( ss.str() );
        }
        else
        {
            argument.setDefaultValue( "" );
        }
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        const OptionRule *opt_rule = dynamic_cast<const OptionRule*>( &the_rule );
        if ( opt_rule != NULL )
        {
            options = opt_rule->getOptions();
        }
        argument.setOptions( options );
        
        // add the argument to the argument list
        arguments.push_back( argument );
    }
    
    help_constructor.setArguments( arguments );
    
    // return value
    help_constructor.setReturnType( getVariableTypeSpec().getType() );
    
    // details
    help_constructor.setDetails( getConstructorDetails() );
    
    // example
    help_constructor.setExample( getConstructorExample() );
    
    //
    std::vector<RevBayesCore::RbHelpFunction> constructors;
    constructors.push_back( help_constructor );
    helpEntry.setConstructors( constructors );
    
    helpEntry.setMethods( getHelpMethods() );
    
}


// Get the distribution specific member methods.
MethodTable Distribution::getDistributionMethods( void ) const
{
    MethodTable methods;
    
    return methods;
}



/**
 * Print the value of this object for the user.
 *
 * There is not much of a value to print for a distribution. 
 * Thus, we simply print the name of the distribution.
 *
 * \param[in]    the stream to which to print.
 */
void Distribution::printValue(std::ostream &o) const 
{
    o << getClassType() << "(...)" << std::endl;
}
