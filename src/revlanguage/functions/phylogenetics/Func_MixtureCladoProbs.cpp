//
//  Func_MixtureCladoProbs.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/22/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "Func_MixtureCladoProbs.h"
#include "ConstantNode.h"
#include "MixtureCladogeneticStateFunction.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlCladogeneticProbabilityMatrix.h"
#include "RlDeterministicNode.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_MixtureCladoProbs::Func_MixtureCladoProbs( void ) : TypedFunction<CladogeneticProbabilityMatrix>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_MixtureCladoProbs* Func_MixtureCladoProbs::clone( void ) const {
    
    return new Func_MixtureCladoProbs( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::CladogeneticProbabilityMatrix >* Func_MixtureCladoProbs::createFunction( void ) const
{
    
    // supplied arguments
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::CladogeneticProbabilityMatrix> >* cp = static_cast<const ModelVector<CladogeneticProbabilityMatrix> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ep = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    size_t n_mixture = cp->getValue().size();
    if (n_mixture > 0) {
        for (size_t i = 1; i < n_mixture; i++)
        {
            if (cp->getValue()[i].size() != cp->getValue()[i-1].size())
            {
                throw RbException("All cladogenetic probability functions must be of equal size.");
            }
            
        }
    }
    
    if (ep->getValue().size() != cp->getValue().size())
    {
        throw RbException("mixtureProbabilities and cladogeneticProbabilities must be of equal size.");
    }
    
//    size_t nc = cp->getValue()[0].size();

    // create P matrix
    RevBayesCore::MixtureCladogeneticStateFunction* f = NULL;
    f = new RevBayesCore::MixtureCladogeneticStateFunction( ep, cp, 0, 0 );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_MixtureCladoProbs::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "cladogeneticProbabilities",
                                                  ModelVector<CladogeneticProbabilityMatrix>::getClassTypeSpec(),
                                                  "The cladogenetic event probability functions.",
                                                  ArgumentRule::BY_CONSTANT_REFERENCE,
                                                  ArgumentRule::ANY ));
        argumentRules.push_back( new ArgumentRule( "mixtureWeights", Simplex::getClassTypeSpec(), "The mixture probabilities.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        
        rules_set = true;
    }
    
    return argumentRules;
}



const std::string& Func_MixtureCladoProbs::getClassType(void)
{
    
    static std::string revType = "Func_MixtureCladoProbs";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_MixtureCladoProbs::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_MixtureCladoProbs::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnMixtureCladoProbs";
    
    return f_name;
}

const TypeSpec& Func_MixtureCladoProbs::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
