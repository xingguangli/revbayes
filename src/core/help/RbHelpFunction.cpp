#include "RbHelpFunction.h"

using namespace RevBayesCore;


RbHelpFunction::RbHelpFunction( void ) : RbHelpEntry()
{
    
}

const std::vector<RbHelpArgument>& RbHelpFunction::getArguments( void ) const
{
    // return a const reference to the internal value
    return arguments;
}


const std::string& RbHelpFunction::getExample( void ) const
{
    // return a const reference to the internal value
    return example;
}


const std::string& RbHelpFunction::getReturnType( void ) const
{
    // return a const reference to the internal value
    return returnType;
}


const std::string& RbHelpFunction::getUsage( void ) const
{
    // return a const reference to the internal value
    return usage;
}


void RbHelpFunction::setArguments(const std::vector<RbHelpArgument> &a)
{
    // overwrite the internal value with the new value
    arguments = a;
}


void RbHelpFunction::setExample(const std::string &e)
{
    // overwrite the internal value with the new value
    example = e;
}


void RbHelpFunction::setReturnType(const std::string &r)
{
    // overwrite the internal value with the new value
    returnType = r;
}


void RbHelpFunction::setUsage(const std::string &u)
{
    // overwrite the internal value with the new value
    usage = u;
}
