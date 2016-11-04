/*
 * File:   Move_MatrixSingleElementSlide.cpp
 * Author: nl
 *
 * Created on 13 juillet 2014, 18:13
 */

#include "Move_MatrixSingleElementSlide.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "MatrixReal.h"
#include "MatrixRealSingleElementSlidingProposal.h"
#include "MetropolisHastingsMove.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlMatrixReal.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_MatrixSingleElementSlide::Move_MatrixSingleElementSlide() : Move()
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_MatrixSingleElementSlide* Move_MatrixSingleElementSlide::clone(void) const
{
    
    return new Move_MatrixSingleElementSlide(*this);
}


void Move_MatrixSingleElementSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal >* tmp = static_cast<const MatrixReal &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::MatrixReal > *n = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MatrixReal> *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::MatrixRealSingleElementSlidingProposal(n,l);
    value = new RevBayesCore::MetropolisHastingsMove(p,w,t);
    
}


/** Get class name of object */
const std::string& Move_MatrixSingleElementSlide::getClassType(void) {
    
    static std::string revClassType = "Move_MatrixSingleElementSlide";
    
    return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_MatrixSingleElementSlide::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revClassTypeSpec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_MatrixSingleElementSlide::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "MatrixElementSlide";
    
    return c_name;
}



/** Return member rules (no members) */
const MemberRules& Move_MatrixSingleElementSlide::getParameterRules(void) const
{
    
    static MemberRules move_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        move_member_rules.push_back( new ArgumentRule( "x"     , MatrixReal::getClassTypeSpec(), "The variable on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        move_member_rules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()   , "The scaling factor (strength) of the proposal.", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        move_member_rules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec() , "Should we tune the scaling factor during burnin?", ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        move_member_rules.insert( move_member_rules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return move_member_rules;
}

/** Get type spec */
const TypeSpec& Move_MatrixSingleElementSlide::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Move_MatrixSingleElementSlide::printValue(std::ostream &o) const
{
    
    o << "Move_MatrixSingleElementSlide(";
    if (v != NULL)
    {
        o << v->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_MatrixSingleElementSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "x" )
    {
        v = var;
    }
    else if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "weight" ) {
        weight = var;
    }
    else if ( name == "tune" ) {
        tune = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}

