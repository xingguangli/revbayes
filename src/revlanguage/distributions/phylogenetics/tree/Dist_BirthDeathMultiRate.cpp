#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_BirthDeathMultiRate.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_BirthDeathMultiRate::Dist_BirthDeathMultiRate() : TypedDistribution<TimeTree>()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_BirthDeathMultiRate* Dist_BirthDeathMultiRate::clone( void ) const
{
    return new Dist_BirthDeathMultiRate(*this);
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
RevBayesCore::MultiRateBirthDeathProcess* Dist_BirthDeathMultiRate::createDistribution( void ) const
{
    
    // get the parameters
    
    
    // the root age
    RevBayesCore::TypedDagNode<double>* ra                   = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
    
    // speciation rate
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* s       = static_cast<const ModelVector<RealPos> &>( lambda->getRevObject() ).getDagNode();
    // extinction rate
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* e       = static_cast<const ModelVector<RealPos> &>( mu->getRevObject() ).getDagNode();
    // rate matrix
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* q      = static_cast<const RateGenerator &>( Q->getRevObject() ).getDagNode();
    // rate
    RevBayesCore::TypedDagNode<double>* rat      = static_cast<const RealPos &>( event_rate->getRevObject() ).getDagNode();
    // root frequencies
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* p       = static_cast<const Simplex &>( pi->getRevObject() ).getDagNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    // taxon names
    const std::vector<RevBayesCore::Taxon> &t   = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getDagNode()->getValue();
    
    // create the internal distribution object
    RevBayesCore::MultiRateBirthDeathProcess*   d = new RevBayesCore::MultiRateBirthDeathProcess(ra, s, e, q, rat, p, r, cond, t);
    
    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_BirthDeathMultiRate::getClassType( void )
{
    
    static std::string revType = "Dist_BirthDeathMultiRate";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_BirthDeathMultiRate::getClassTypeSpec( void )
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_BirthDeathMultiRate::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "MRBDP" );
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_BirthDeathMultiRate::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "BirthDeathMultiRate";
    
    return d_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the constant-rate birth-death process are:
 * (1) the speciation rate lambda which must be a positive real.
 * (2) the extinction rate mu that must be a positive real.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& Dist_BirthDeathMultiRate::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        memberRules.push_back( new ArgumentRule( "rootAge" , RealPos::getClassTypeSpec(), "The root age.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "rho"     , Probability::getClassTypeSpec(), "The taxon-sampling probability.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(1.0) ) );
        
        memberRules.push_back( new ArgumentRule( "lambda", ModelVector<RealPos>::getClassTypeSpec(), "Vector of speciation rates per rate category.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "mu"    , ModelVector<RealPos>::getClassTypeSpec(), "Vector of extinction rates per rate category.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "Q"     , RateGenerator::getClassTypeSpec(), "Rate matrix of transition rates between diversification-rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "delta" , RealPos::getClassTypeSpec(), "Global rate of transition between rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "pi"    , Simplex::getClassTypeSpec(), "State frequencies at the root.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        memberRules.push_back( new OptionRule( "condition"    , new RlString("survival"), optionsCondition, "The condition of the birth-death process." ) );
        memberRules.push_back( new ArgumentRule( "taxa"      , ModelVector<Taxon>::getClassTypeSpec(), "The taxon names used for initialization.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = TypedDistribution<TimeTree>::getParameterRules();
        memberRules.insert(memberRules.end(), parentRules.begin(), parentRules.end());
        
        rules_set = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_BirthDeathMultiRate::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
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
void Dist_BirthDeathMultiRate::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "mu" )
    {
        mu = var;
    }
    else if ( name == "Q" )
    {
        Q = var;
    }
    else if ( name == "delta" )
    {
        event_rate = var;
    }
    else if ( name == "pi" )
    {
        pi = var;
    }
    else if ( name == "rootAge" )
    {
        rootAge = var;
    }
    else if ( name == "rho" )
    {
        rho = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else if ( name == "condition" )
    {
        condition = var;
    }
    else
    {
        TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
    
}
