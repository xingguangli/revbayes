#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_normalTruncatedPositive.h"
#include "TruncatedNormalDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;


/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_normalTruncatedPositive::Dist_normalTruncatedPositive() : PositiveContinuousDistribution()
{
    
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::TruncatedNormalDistribution* Dist_normalTruncatedPositive::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>*         m  = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>*         s  = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>*         mi = static_cast<const RealPos &>( min->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>*         ma = static_cast<const RealPos &>( max->getRevObject() ).getDagNode();
    RevBayesCore::TruncatedNormalDistribution*  d  = new RevBayesCore::TruncatedNormalDistribution(m, s, mi, ma);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_normalTruncatedPositive* Dist_normalTruncatedPositive::clone( void ) const
{
    
    return new Dist_normalTruncatedPositive(*this);
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_normalTruncatedPositive::getClassType(void)
{
    
    static std::string revType = "Dist_normalTruncatedPositive";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_normalTruncatedPositive::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_normalTruncatedPositive::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "normal";
    
    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_normalTruncatedPositive::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Dist_normalTruncatedPositive::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Normal (gaussian) distribution with mean equal to ‘mean’ and standard deviation equal to ‘sd’." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Dist_normalTruncatedPositive::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    std::string details_1 = "";
    details_1 += "The normal distribution has density:";
    details_1 += "f(x) = 1/(sqrt(2 pi) sigma) e^-((x - mu)^2/(2 sigma^2))";
    details_1 += "where mu is the mean of the distribution and sigma the standard deviation.";
    
    details.push_back( details_1 );
    
    std::string details_2 = "";
    details_2 += "f(x) = 1/(sqrt(2 pi) sigma) e^-((x - mu)^2/(2 sigma^2))";
    
    details.push_back( details_2 );
    
    std::string details_3 = "";
    details_3 += "where mu is the mean of the distribution and sigma the standard deviation.";
    
    details.push_back( details_3 );
    
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Dist_normalTruncatedPositive::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "# we simulate some oversations\n";
    example += "x <- rnorm(n=10,mean=5,sd=10)\n";
    example += "# let's see what the minum is (you could do the max too)\n";
    example += "min(x)\n";
    example += "# let's also see what the mean and the variance are\n";
    example += "mean(x)\n";
    example += "var(x)\n";
    example += "sd(x)\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_normalTruncatedPositive::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_normalTruncatedPositive::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "dnLognormal" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_normalTruncatedPositive::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Normal Distribution";
    
    return title;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the branch rate jump process are:
 * (1) the mean of the distribution.
 * (2) the standard deviation.
 *
 * \return The member rules.
 */
const MemberRules& Dist_normalTruncatedPositive::getParameterRules(void) const
{
    
    static MemberRules distNormMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        distNormMemberRules.push_back( new ArgumentRule( "mean", Real::getClassTypeSpec()   , "The mean parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distNormMemberRules.push_back( new ArgumentRule( "sd"  , RealPos::getClassTypeSpec(), "The standard deviation parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distNormMemberRules.push_back( new ArgumentRule( "min" , RealPos::getClassTypeSpec(), "The minimum value the variable can be.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distNormMemberRules.push_back( new ArgumentRule( "max" , RealPos::getClassTypeSpec(), "The maximum value the variable can be.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return distNormMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_normalTruncatedPositive::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_normalTruncatedPositive::printValue(std::ostream& o) const
{
    
    o << " norm(mean=";
    if ( mean != NULL )
    {
        o << mean->getName();
    }
    else
    {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL )
    {
        o << sd->getName();
    }
    else
    {
        o << "?";
    }
    o << ", min=";
    if ( min != NULL )
    {
        o << min->getName();
    }
    else
    {
        o << "?";
    }
    o << ", max=";
    if ( max != NULL )
    {
        o << max->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_normalTruncatedPositive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "mean" )
    {
        mean = var;
    }
    else if ( name == "sd" )
    {
        sd = var;
    }
    else if ( name == "min" )
    {
        min = var;
    }
    else if ( name == "max" )
    {
        max = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
