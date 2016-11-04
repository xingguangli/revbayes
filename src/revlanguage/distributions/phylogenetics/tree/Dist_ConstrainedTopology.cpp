#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_ConstrainedTopology.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_ConstrainedTopology::Dist_ConstrainedTopology() : TypedDistribution<TimeTree>()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_ConstrainedTopology* Dist_ConstrainedTopology::clone( void ) const
{
    return new Dist_ConstrainedTopology(*this);
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
RevBayesCore::TopologyConstrainedTreeDistribution* Dist_ConstrainedTopology::createDistribution( void ) const
{
    
    // get the parameters
    const RevBayesCore::RbVector<RevBayesCore::Clade>& c = static_cast<const ModelVector<Clade> &>( constraints->getRevObject() ).getValue();
    const Distribution&                                     rlDistribution  = static_cast<const Distribution &>( baseDistribution->getRevObject() );
    RevBayesCore::TypedDistribution<RevBayesCore::Tree>*    base            = static_cast<RevBayesCore::TypedDistribution<RevBayesCore::Tree>* >( rlDistribution.createDistribution() );
    
    // create the internal distribution object
    RevBayesCore::TopologyConstrainedTreeDistribution* d = new RevBayesCore::TopologyConstrainedTreeDistribution(base, c);
    
    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_ConstrainedTopology::getClassType( void )
{
    
    static std::string revType = "Dist_ConstrainedTopology";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_ConstrainedTopology::getClassTypeSpec( void )
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_ConstrainedTopology::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_ConstrainedTopology::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "ConstrainedTopology";
    
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
const MemberRules& Dist_ConstrainedTopology::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {

        memberRules.push_back( new ArgumentRule( "treeDistribution", TypedDistribution<TimeTree>::getClassTypeSpec(), "The base distribution for the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "constraints", ModelVector<Clade>::getClassTypeSpec(), "The topological constraints.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return memberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_ConstrainedTopology::getTypeSpec( void ) const
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
void Dist_ConstrainedTopology::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "constraints" )
    {
        constraints = var;
    }
    else if ( name == "treeDistribution" )
    {
        baseDistribution = var;
    }
    else
    {
        TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
    
}
