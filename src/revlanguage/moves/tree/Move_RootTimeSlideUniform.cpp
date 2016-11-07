#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_RootTimeSlideUniform.h"
#include "RootTimeSlideUniformProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_RootTimeSlideUniform::Move_RootTimeSlideUniform() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_RootTimeSlideUniform* Move_RootTimeSlideUniform::clone(void) const
{
    
	return new Move_RootTimeSlideUniform(*this);
}


void Move_RootTimeSlideUniform::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    RevBayesCore::TypedDagNode<double> *tmp_double = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<double> *d = static_cast<RevBayesCore::StochasticNode<double> *>( tmp_double );
    
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::RootTimeSlideUniformProposal( t, d );
    value = new RevBayesCore::MetropolisHastingsMove(p,w,false);
}


/** Get Rev type of object */
const std::string& Move_RootTimeSlideUniform::getClassType(void)
{
    
    static std::string revType = "Move_RootTimeSlideUniform";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_RootTimeSlideUniform::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_RootTimeSlideUniform::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "RootTimeSlideUniform";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Move_RootTimeSlideUniform::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        memberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), "The tree on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "origin", RealPos::getClassTypeSpec() , "The maximum root age.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_RootTimeSlideUniform::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}



/** Get type spec */
void Move_RootTimeSlideUniform::printValue(std::ostream &o) const
{
    
    o << "Move_RootTimeSlideUniform(";
    if (tree != NULL)
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ", ";
    if (origin != NULL)
    {
        o << origin->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_RootTimeSlideUniform::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "origin" )
    {
        origin = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}
