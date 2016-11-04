#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Function.h"
#include "ModelVector.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RevObject.h"
#include "RbUtil.h"

#include <sstream>

using namespace RevLanguage;

/** Basic constructor. */
Function::Function(void) : RevObject( false ),
    args( )
{

    argsProcessed = false;
}

/** Copy constructor. */
Function::Function(const Function &x) : RevObject( x ), 
    argsProcessed( x.argsProcessed ),
    args( x.args ),
    env( x.env )
{
    
}


/** Destructor. We need to free the arguments here. */
Function::~Function(void)
{
    
    // we don't own the enclosing environment -> we don't delete it.
}


/** Debug info about object */
std::string Function::callSignature(void) const
{
    
    std::ostringstream o;
    o << getType() << ": " << std::endl;
    
    if (argsProcessed)
    {

        for ( size_t i = 0;  i < args.size(); i++ )
        {
            o << " args[" << i << "] = ";
            args[i].getVariable()->getRevObject().printValue( o, true);
            o << std::endl;
        }
    }
    else
    {
        
        for ( size_t i = 0;  i < getArgumentRules().size(); i++ )
        {
            o << " args[" << i << "] = ";
            getArgumentRules()[i].printValue( o );
            o << std::endl;
        }
    }
    
    return o.str();
}


/**
 * @brief Check arguments
 *
 * This function processes arguments based on argument rules. First it deletes
 * any previously stored arguments. If the matching of the new arguments
 * succeeds, the processedArguments will be set to the new vector of processed
 * arguments and the function returns true. Any subsequent calls to execute()
 * will then use the processed arguments. You can also call the function with
 * the arguments directly, in which case processArguments will be called first
 * before the operation is actually performed.
 *
 * In matching arguments to argument rules, we use the same rules as in R with
 * the addition that types are also used in the matching process, after arguments
 * have been reordered as in R. The FunctionTable ensure that all argument rules
 * are distinct. However, several functions can nevertheless match the same
 * arguments because of the inheritance hierarchy. In these cases, the closest
 * match is chosen based on the first argument, then on the second, etc.
 *
 * The function returns a match score based on how closely the arguments match the
 * rules, with 0 being perfect match, 1 being a match to an immediate base class type,
 * 2 a match to a grandparent class, etc. A large number is used for arguments that
 * need type conversion.
 *
 * The evaluateOnce parameter MemberObject whether the function is to be evaluated once,
 * immediately after the call to processArguments, or whether the processed arguments
 * will be used in repeated function calls in a function node. Argument matching is
 * based on current values in the first case, but on the wrapper type in the second.
 *
 * @todo Labels need to be stored for ellipsis arguments.
 *
 * These are the argument matching rules:
 *
 *  1. If the last argument rule is an ellipsis, and it is the kth argument passed
 *     in, then all arguments passed in, from position k to the end, are wrapped
 *     in a single ContainerNode object. These arguments are not matched to any
 *     rules.
 *  2. The remaining arguments are matched to labels using exact matching. If the
 *     type does not match the type of the rule, it is an error.
 *  3. The remaining arguments are matched to any remaining slots using partial
 *     matching. If there is ambiguity or the types do not match, it is an error.
 *  4. The remaining arguments are used for the empty slots in the order they were
 *     passed in. If the types do not match, it is an error.
 *  5. Any remaining empty slots are filled with default values stored in the argument
 *     rules (we use copies of the values, of course).
 *  6. If there are still empty slots, the arguments do not match the rules.
 *
 * @todo Fredrik: The code and the logic has been changed without changing the comments, so these
 *       are out of date. Also note that the argument matching is problematic for unlabeled
 *       arguments (order can be changed based on argument types, which may cause unintended
 *       consequences). Furthermore, there is redundant code left from the old implementation.
 *       Finally, the ellipsis arguments no longer have to be last among the rules, but they
 *       are still the last arguments after processing.
 */
bool Function::checkArguments( const std::vector<Argument>& passedArgs, std::vector<double>* matchScore, bool once)
{
    
    /*********************  0. Initialization  **********************/
    
    double MAX_ARGS = 1000.0;
    
    /* Get the argument rules */
    const ArgumentRules& theRules = getArgumentRules();
    
    /* Get the number of argument rules */
    size_t nRules = theRules.size();
    
    /* Keep track of which arguments we have used, and which argument slots we have filled, and with what passed arguments */
    std::vector<bool>   taken           = std::vector<bool>( passedArgs.size(), false );
    std::vector<bool>   filled          = std::vector<bool>( theRules.size(), false );
    
    
    /*********************  1. Do exact matching  **********************/
    
    /* Do exact matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) 
    {
        
        /* Test if swallowed by ellipsis; if so, we can quit because the remaining passedArgs will also be swallowed */
        if ( taken[i] )
        {
            break;
        }
        
        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
        {
            continue;
        }
        
        /* Check for matches in all regular rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (size_t j=0; j<nRules; j++) 
        {
            
            if ( passedArgs[i].getLabel() == theRules[j].getArgumentLabel() ) 
            {
                
                if ( filled[j] )
                    return false;

                Argument &arg = const_cast<Argument&>(passedArgs[i]);
                double penalty = theRules[j].isArgumentValid( arg, once );
                if ( penalty != -1 )
                {
                    taken[i]          = true;
                    filled[j]         = true;
                    
                    if ( matchScore != NULL) 
                    {
                        double score = computeMatchScore(passedArgs[i].getVariable(), theRules[j]);
                        score += abs(int(i)-int(j)) / MAX_ARGS;
                        score += penalty*100.0;
                        matchScore->push_back(score);
                    }
                }
                else
                {
                    return false;
                }
                
                // We got an exact match -> we can skip the other labels for checking
                break;
            }
        }
    }
    
    
    /*********************  2. Do partial matching  **********************/
    
    /* Do partial matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) 
    {
        
        /* Skip if already matched */
        if ( taken[i] )
        {
            continue;
        }
        
        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
        {
            continue;
        }
        
        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;
        
        /* Try all rules */
        for (size_t j=0; j<nRules; j++) 
        {
            
            if ( !filled[j] && theRules[j].getArgumentLabel().compare(0, passedArgs[i].getLabel().size(), passedArgs[i].getLabel()) == 0 ) 
            {
                ++nMatches;
                matchRule = static_cast<int>( j );
            }
        }
        
        if (nMatches == 1)
        {
            
            Argument &arg = const_cast<Argument&>(passedArgs[i]);
            double penalty = theRules[matchRule].isArgumentValid(arg, once );
            if ( penalty != -1 )
            {
                taken[i]                  = true;
                filled[matchRule]         = true;
            
                if ( matchScore != NULL)
                {
                    double score = computeMatchScore(passedArgs[i].getVariable(), theRules[matchRule]);
                    score += abs(int(i)-int(matchRule)) / MAX_ARGS;
                    score += penalty*100.0;
                    matchScore->push_back(score);
                }
            }
            else
            {
                return false;
            }
        }
    
    }
    
    
    /*********************  3. Fill with unused passedArgs  **********************/
    
    /* Fill in empty slots using the remaining args in order */
    for(size_t i=0; i<passedArgs.size(); i++) 
    {
        
        /* Skip if already matched */
        if ( taken[i] )
            continue;
        
        /* Find first empty slot and try to fit argument there */
        for (size_t j=0; j<nRules; j++) 
        {
            
            if ( filled[j] == false ) 
            {
                
                Argument &arg = const_cast<Argument&>(passedArgs[i]);
                double penalty = theRules[j].isArgumentValid( arg, once );
                if ( penalty != -1 )
                {
                    taken[i]          = true;
                    if ( !theRules[j].isEllipsis() ) 
                    {
                        filled[j]     = true;
                    }
                    
                    if ( matchScore != NULL) 
                    {
                        const RevPtr<const RevVariable>& argVar = passedArgs[i].getVariable();
                        double score = computeMatchScore(argVar, theRules[j]);
                        score += abs(int(i)-int(j)) / MAX_ARGS;
                        score += penalty*100.0;
                        matchScore->push_back(score);
                    }
                    
                    break;
                }
            }
        }
        
        /* Final test if we found a match */
        if ( !taken[i] )
        {
            return false;
        }
        
    }
    
    
    
    /*********************  4. Fill with default values  **********************/
    
    /* Fill in empty slots using default values */
    for(size_t i=0; i<nRules; i++) 
    {
        
        if ( filled[i] == true || theRules[i].isEllipsis() )
        {
            continue;
        }
        
        if ( !theRules[i].hasDefault() )
        {
            return false;
        }
        
    }
    
    return true;
}


/* Delete processed args */
void Function::clear(void)
{

    args.clear();
    
    argsProcessed = false;
    
    clearArguments();
}


/* Delete processed args */
void Function::clearArguments(void)
{
    
    // We have already done everything, but derived classes might want to overwrite this function.
    
}


/** Compute the match score between the argument and the argument rule. */
double Function::computeMatchScore(const RevVariable *var, const ArgumentRule &rule)
{
   
    double     score = 10000;   // Needs to be larger than the max depth of the class hierarchy

    const TypeSpec& argClass = var->getRevObject().getTypeSpec();
    const std::vector<TypeSpec> &ruleArgTypes = rule.getArgumentTypeSpec();
    for ( std::vector<TypeSpec>::const_iterator it = ruleArgTypes.begin(); it != ruleArgTypes.end(); ++it) 
    {
        int j = 0;
        const TypeSpec* parent = &argClass;
        do
        {
            if (*parent == *it )
            {
                score = j;
                break;
            }
            parent = parent->getParentTypeSpec();
            j++;
            if ( j >= score ) 
            {
                break;
            }
        } while (parent != NULL);
    }
    
    return score;    // We needed type conversion for this argument
}


const std::vector<Argument>& Function::getArguments(void) const
{
    return args;
}


std::vector<Argument>& Function::getArguments(void)
{
    return args;
}


/** Get Rev type of object */
const std::string& Function::getClassType(void)
{
    
    static std::string revType = "Function";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Function::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/** Get execution environment of function */
Environment* Function::getEnvironment(void) const
{
    
    return env;
}


RevBayesCore::RbHelpFunction* Function::constructTypeSpecificHelp( void ) const
{
    
    return new RevBayesCore::RbHelpFunction();
}


/**
 * Get the help entry for this class
 */
void Function::addSpecificHelpFields(RevBayesCore::RbHelpEntry *e) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpFunction *help = static_cast<RevBayesCore::RbHelpFunction*>( e );
    RevBayesCore::RbHelpFunction &helpEntry = *help;
    
    // usage
    helpEntry.setUsage( getHelpUsage() );
    
    // arguments
    const MemberRules& rules = getArgumentRules();
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
    
    helpEntry.setArguments( arguments );
    
    // return value
    helpEntry.setReturnType( getReturnType().getType() );
    
    // details
    helpEntry.setDetails( getHelpDetails() );
    
    // example
    helpEntry.setExample( getHelpExample() );
    
}


/**
 * Get the name for this procedure.
 */
std::string Function::getConstructorFunctionName( void ) const
{
    return getFunctionName();
}


/**
 * Get the name for this procedure.
 */
std::vector<std::string> Function::getConstructorFunctionAliases( void ) const
{
    
    return getFunctionNameAliases();
}


/**
 * Get the usage for this function.
 */
std::string Function::getHelpUsage( void ) const
{
    std::string usage = getRevDeclaration();
    
    return usage;
}


/**
 * Get the name for this procedure.
 */
std::vector<std::string> Function::getFunctionNameAliases( void ) const
{
    std::vector<std::string> aliases;
    
    return aliases;
}


/** 
 * Get Rev declaration of the function, formatted for output
 */
std::string Function::getRevDeclaration(void) const
{
    
    std::ostringstream o;
 
    // Sebastian: We don't want to print the return type in the usage.
    // It only confuses.
//    o << getReturnType().getType();
    if ( getFunctionName() == "" )
    {
        o << "<unnamed>(";
    }
    else
    {
        o << "" << getFunctionName() << "(";
    }
    
    const ArgumentRules& argRules = getArgumentRules();
    for (size_t i=0; i<argRules.size(); ++i)
    {
        if (i != 0)
        {
            o << ", ";
        }
        argRules[i].printValue(o);
    }
    o << ")";
    
    return o.str();
}


/** Print value for user */
void Function::printValue(std::ostream& o, bool user) const
{
    const ArgumentRules& argRules = getArgumentRules();

    size_t n_space = getFunctionName().size() + 2;
    std::string space(n_space, ' ');
    
    o << getFunctionName() << " (";
    for (size_t i=0; i<argRules.size(); i++)
    {
        if (i != 0)
        {
            o << ",\n";
            o << space;
        }
        argRules[i].printValue(o);
    }
    o << ")";
}


/**
 * @brief Process arguments
 *
 * This function processes arguments based on argument rules. First it deletes
 * any previously stored arguments. If the matching of the new arguments
 * succeeds, the processedArguments will be set to the new vector of processed
 * arguments and the function returns true. Any subsequent calls to execute()
 * will then use the processed arguments. You can also call the function with
 * the arguments directly, in which case processArguments will be called first
 * before the operation is actually performed.
 *
 * In matching arguments to argument rules, we use the same rules as in R with
 * the addition that types are also used in the matching process, after arguments
 * have been reordered as in R. The FunctionTable ensure that all argument rules
 * are distinct. However, several functions can nevertheless match the same
 * arguments because of the inheritance hierarchy. In these clases, the closest
 * match is chosen based on the first argument, then on the second, etc.
 *
 * The function returns a match score based on how closely the arguments match the
 * rules, with 0 being perfect match, 1 being a match to an immediate base class type,
 * 2 a match to a grandparent class, etc. A large number is used for arguments that
 * need type conversion.
 *
 * The evaluateOnce parameter MemberObject whether the function is to be evaluated once,
 * immediately after the call to processArguments, or whether the processed arguments
 * will be used in repeated function calls in a function node. Argument matching is
 * based on current values in the first case, but on the wrapper type in the second.
 *
 * @todo Labels need to be stored for ellipsis arguments.
 *
 * These are the argument matching rules:
 *
 *  1. If the last argument rule is an ellipsis, and it is the kth argument passed
 *     in, then all arguments passed in, from position k to the end, are wrapped
 *     in a single ContainerNode object. These arguments are not matched to any
 *     rules.
 *  2. The remaining arguments are matched to labels using exact matching. If the
 *     type does not match the type of the rule, it is an error.
 *  3. The remaining arguments are matched to any remaining slots using partial
 *     matching. If there is ambiguity or the types do not match, it is an error.
 *  4. The remaining arguments are used for the empty slots in the order they were
 *     passed in. If the types do not match, it is an error.
 *  5. Any remaining empty slots are filled with default values stored in the argument
 *     rules (we use copies of the values, of course).
 *  6. If there are still empty slots, the arguments do not match the rules.
 *
 * @todo Fredrik: The code and the logic has been changed without changing the comments, so these
 *       are out of date. Also note that the argument matching is problematic for unlabeled
 *       arguments (order can be changed based on argument types, which may cause unintended
 *       consequences). Furthermore, there is redundant code left from the old implementation.
 *       Finally, the ellipsis arguments no longer have to be last among the rules, but they
 *       are still the last arguments after processing.
 *
 * @todo Fredrik: Static and dynamic type conversion added, but partly hack-ish, so the implementation
 *       needs to be revised
 */
void Function::processArguments( const std::vector<Argument>& passedArgs, bool once )
{

    /*********************  0. Initialization  **********************/
    /* Get my own copy of the argument vector */
    std::vector<Argument> pArgs = passedArgs;
    
    /* Get the argument rules */
    const ArgumentRules& theRules = getArgumentRules();

    /* Get the number of argument rules */
    size_t nRules = theRules.size();

    /* Clear previously processed arguments */
    args.clear();
    
    /* Keep track of which arguments we have used, and which argument slots we have filled, and with what passed arguments */
    std::vector<bool>   taken           = std::vector<bool>( passedArgs.size(), false );
    std::vector<bool>   filled          = std::vector<bool>( nRules, false );
    std::vector<size_t> passedArgIndex  = std::vector<size_t>( nRules, 1000 );
    std::vector<Argument> ellipsisArgs;

    /*********************  1. Do exact matching  **********************/

    /* Do exact matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++)
    {

        /* Test if swallowed by ellipsis; if so, we can quit because the remaining passedArgs will also be swallowed */
        if ( taken[i] )
        {
            break;
        }
        
        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
        {
            continue;
        }
        
        /* Check for matches in all regular rules (we assume that all labels are unique; this is checked by FunctionTable) */
        for (size_t j=0; j<nRules; j++) {

            if ( passedArgs[i].getLabel() == theRules[j].getArgumentLabel() )
            {

                if ( filled[j] )
                {
                    throw RbException( "Duplicate argument labels '" + passedArgs[i].getLabel() );
                }
                
                pArgs[i]            = theRules[j].fitArgument( pArgs[i], once );
                taken[i]            = true;
                filled[j]           = true;
                passedArgIndex[j]   = static_cast<int>( i );
                
                // We got an exact match -> we can skip the other labels for checking
                break;
            }
        }
    }

 
    /*********************  2. Do partial matching  **********************/

    /* Do partial matching of labels */
    for(size_t i=0; i<passedArgs.size(); i++) 
    {

        /* Skip if already matched */
        if ( taken[i] )
        {
            continue;
        }
        
        /* Skip if no label */
        if ( passedArgs[i].getLabel().size() == 0 )
        {
            continue;
        }
        
        /* Initialize match index and number of matches */
        int nMatches = 0;
        int matchRule = -1;

        /* Try all rules */
        for (size_t j=0; j<nRules; j++) 
        {

            if ( !filled[j] && theRules[j].getArgumentLabel().compare(0, passedArgs[i].getLabel().size(), passedArgs[i].getLabel()) == 0 ) 
            {
                ++nMatches;
                matchRule = static_cast<int>( j );
            }
        }

        if (nMatches > 1)
        {
            throw RbException( "Argument label '" + passedArgs[i].getLabel() + "' matches mutliple parameter labels." );
        }
        else if (nMatches < 1)
        {
            throw RbException( "Argument label '" + passedArgs[i].getLabel() + "' matches no untaken parameter labels." );
        }
        
        if ( nMatches == 1)
        {
            pArgs[i]                    = theRules[matchRule].fitArgument( pArgs[i], once );
            taken[i]                    = true;
            filled[matchRule]           = true;
            passedArgIndex[matchRule]   = static_cast<int>( i );
        }
        
    }


    /*********************  3. Fill with unused passedArgs  **********************/

    /* Fill in empty slots using the remaining args in order */
    for(size_t i=0; i<passedArgs.size(); i++) 
    {

        /* Skip if already matched */
        if ( taken[i] )
        {
            continue;
        }
        
        /* Find first empty slot and try to fit argument there */
        for (size_t j=0; j<nRules; j++) 
        {

            if ( filled[j] == false &&
                 ( (!theRules[j].isEllipsis() && passedArgs[i].getLabel().size() == 0) || (theRules[j].isEllipsis()) ) )
            {
                
                Argument &arg = const_cast<Argument&>(passedArgs[i]);
                double penalty = theRules[j].isArgumentValid( arg, once );
                if ( penalty != -1 )
                {
                    pArgs[i]          = theRules[j].fitArgument( pArgs[i], once );
                    taken[i]          = true;
                    if ( !theRules[j].isEllipsis() ) 
                    {
                        filled[j]     = true;
                        passedArgIndex[j] = static_cast<int>( i );
                    }
                    else 
                    {
                        ellipsisArgs.push_back( pArgs[i] );
                    }
                    
                    break;
                }
            }
        }
        
        /* Final test if we found a match */
        if ( !taken[i] )
        {
            throw RbException("Superfluous argument of type '" + passedArgs[i].getVariable()->getRevObject().getType() + "' and name '" + passedArgs[i].getLabel() + "' passed to function '" + getType() + "'.");
        }
    }

    /*********************  4. Fill with default values  **********************/

    /* Fill in empty slots using default values */
    for(size_t i=0; i<nRules; i++) 
    {

        if ( filled[i] == true || theRules[i].isEllipsis())
        {
            continue;
        }
        
        // we just leave the optional arguments empty
        if ( !theRules[i].hasDefault() )
        {
            throw RbException("No argument found for parameter '" + theRules[i].getArgumentLabel() + "'.");
        }
        
        const ArgumentRule& theRule = theRules[i];
        RevPtr<RevVariable> theVar = theRule.getDefaultVariable().clone();
        theVar->setName( "." + theRule.getArgumentLabel() );
        theVar->setRequiredTypeSpec( theRule.getDefaultVariable().getRequiredTypeSpec() );
        size_t idx = pArgs.size();
        passedArgIndex[i] = idx;
        pArgs.push_back( Argument( theVar, theRule.getArgumentLabel(), theRule.getEvaluationType() != ArgumentRule::BY_CONSTANT_REFERENCE ) );
    }

    argsProcessed = true;
    
    /*********************  5. Insert arguments into argument list  **********************/
    for (size_t j=0; j<nRules; j++) 
    {
        if ( passedArgIndex[j] < 1000 )
        {
            args.push_back( pArgs[ passedArgIndex[j] ] );
        }
        
    }
    
    /*********************  6. Insert ellipsis arguments  **********************/
    for (std::vector<Argument>::iterator i = ellipsisArgs.begin(); i != ellipsisArgs.end(); i++) 
    {
        args.push_back( *i );
    }

}


void Function::setExecutionEnviroment(Environment *e)
{
    
    env = e;

}




