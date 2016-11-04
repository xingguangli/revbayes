#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ChisqDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_chisq.h"
#include "Natural.h"
#include "Probability.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_chisq::Dist_chisq(void) : PositiveContinuousDistribution()
{
    
}


Dist_chisq::~Dist_chisq(void)
{
    
}



Dist_chisq* Dist_chisq::clone( void ) const
{
    
    return new Dist_chisq(*this);
}


RevBayesCore::ChisqDistribution* Dist_chisq::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<int>*    df = static_cast<const Natural     &>( degrees->getRevObject() ).getDagNode();
    RevBayesCore::ChisqDistribution* d  = new RevBayesCore::ChisqDistribution( df );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_chisq::getClassType(void)
{
    
    static std::string revType = "Dist_chisq";
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_chisq::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_chisq::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "chisq";
    
    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_chisq::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Dist_chisq::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "The chi-square probability distribution." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Dist_chisq::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Dist_chisq::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "# The most important use of the chi-square distribution\n";
    example += "# is arguable the quantile function.\n";
    example += "# You can access it the following way:\n";
    example += "df <- 10\n";
    example += "a := qchisq(0.025, df)\n";
    example += "a\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_chisq::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_chisq::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_chisq::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Chi-Square Distribution";
    
    return title;
}


/** Return member rules (no members) */
const MemberRules& Dist_chisq::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        dist_member_rules.push_back( new ArgumentRule( "df", Natural::getClassTypeSpec(), "The degrees of freedom.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_chisq::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    return ts;
}


/** Print value for user */
void Dist_chisq::printValue(std::ostream& o) const
{
    
    o << "chisq(df=";
    if ( degrees != NULL )
    {
        o << degrees->getName();
    }
    else
    {
        o << "?";
    }
    
    o << ")";
}


/** Set a member variable */
void Dist_chisq::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "df" )
    {
        degrees = var;
    }
    else
    {
        TypedDistribution<RealPos>::setConstParameter(name, var);
    }
}
