
//
//  Func_pomoStateConverter.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 22/8/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_pomoStateConverter.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlDeterministicNode.h"
#include "RlTaxon.h"
#include "PomoStateConverter.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_pomoStateConverter::Func_pomoStateConverter( void ) : Procedure( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_pomoStateConverter* Func_pomoStateConverter::clone( void ) const {
    
    return new Func_pomoStateConverter( *this );
}


RevPtr<RevVariable> Func_pomoStateConverter::execute() {
    
    const RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* aln = static_cast<const AbstractHomologousDiscreteCharacterData&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();


    RevBayesCore::PomoStateConverter* c = new RevBayesCore::PomoStateConverter(  );
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::Taxon> >* taxa  = static_cast< const ModelVector<Taxon> &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    std::map <std::string, std::string > gene2species;

    for (RevBayesCore::RbIterator<RevBayesCore::Taxon> it=taxa->getValue().begin(); it!=taxa->getValue().end(); ++it)
    {
        gene2species[it->getName()] = it->getSpeciesName();
    }
    
    AbstractHomologousDiscreteCharacterData PomoAln = c->convertData( aln->getValue(), n->getValue(), gene2species ) ;
        
    return new RevVariable( new AbstractHomologousDiscreteCharacterData( PomoAln ) );
}


/* Get argument rules */
const ArgumentRules& Func_pomoStateConverter::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "aln"      , AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "virtualNe", Natural::getClassTypeSpec()                                , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "taxa"     , ModelVector<Taxon>::getClassTypeSpec()                     , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );

        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_pomoStateConverter::getClassType(void)
{
    
    static std::string revType = "Func_pomoStateConverter";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_pomoStateConverter::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_pomoStateConverter::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "pomoStateConvert";
    
    return f_name;
}


/* Get return type */
const TypeSpec& Func_pomoStateConverter::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = AbstractHomologousDiscreteCharacterData::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_pomoStateConverter::getTypeSpec( void ) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
