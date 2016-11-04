//
//  Func_cladoProbs.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "Func_cladoProbs.h"
#include "ConstantNode.h"
#include "CladogenicStateFunction.h"
#include "MatrixReal.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_cladoProbs::Func_cladoProbs( void ) : TypedFunction<MatrixReal>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_cladoProbs* Func_cladoProbs::clone( void ) const {
    
    return new Func_cladoProbs( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_cladoProbs::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ep = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getValue();
    unsigned ns = static_cast<const Natural &>( this->args[2].getVariable()->getRevObject() ).getValue();
    std::string pt = static_cast<const RlString &> ( this->args[3].getVariable()->getRevObject() ).getValue();
    bool ept = (pt == "pattern");
    bool wa = static_cast<const RlBoolean &>( this->args[4].getVariable()->getRevObject() ).getValue();
    
//    if ( er->getValue().size() != (bf->getValue().size() * (bf->getValue().size()-1) / 2.0) )
//    {
//        throw RbException("The dimension betwee the base frequencies and the substitution rates does not match.");
//    }
    RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >* er = new RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >("er", new RevBayesCore::RbVector<double>(2,.5) );
    RevBayesCore::CladogenicStateFunction* f = new RevBayesCore::CladogenicStateFunction( ep, er, nc, ns, ept, wa );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_cladoProbs::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "eventProbs", Simplex::getClassTypeSpec(), "The probabilities of the different event types.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
//        argumentRules.push_back( new ArgumentRule( "eventRates", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "numCharacters", Natural::getClassTypeSpec(), "The number of characters.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "num_states", Natural::getClassTypeSpec(), "The number of states,", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> options;
        options.push_back( "pattern" );
        options.push_back( "class" );
        argumentRules.push_back( new OptionRule( "probType", new RlString("pattern"), options, "Assign event weights over classes of patterns or over specific patterns" ) );
        
        argumentRules.push_back( new ArgumentRule( "widespreadAllopatry", RlBoolean::getClassTypeSpec(), "Allopatry may result in both daughter ranges being larger than size 1.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean( false ) ) );
        
        
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_cladoProbs::getClassType(void)
{
    
    static std::string revType = "Func_cladoProbs";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_cladoProbs::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_cladoProbs::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnCladoProbs";
    
    return f_name;
}


const TypeSpec& Func_cladoProbs::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
