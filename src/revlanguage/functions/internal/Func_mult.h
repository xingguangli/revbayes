/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_mult, which is used to mult two
 * variables.
 *
 * @brief Declaration and implementation of Func_mult
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_mult.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Func_mult_H
#define Func_mult_H

#include "Function.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename firstValType, typename secondValType, typename retType>
    class Func_mult :  public Function {
        
    public:
        Func_mult( void );
        
        // Basic utility functions
        Func_mult*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    private:
        
    };
    
}

#include "DeterministicNode.h"
#include "BinaryMultiplication.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func_mult<firstValType, secondValType, retType>::Func_mult( void ) : Function( ) {
    
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func_mult<firstValType, secondValType, retType>* RevLanguage::Func_mult<firstValType, secondValType, retType>::clone( void ) const {
    
    return new Func_mult( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevLanguage::RevObject* RevLanguage::Func_mult<firstValType, secondValType, retType>::execute() {
    
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<typename secondValType::valueType>* secondArg = static_cast<const secondValType &>( this->args[1].getVariable()->getValue() ).getValueNode();
    RevBayesCore::BinaryMultiplication<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType> *func = new RevBayesCore::BinaryMultiplication<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType>(firstArg, secondArg);
    RevBayesCore::DeterministicNode<typename retType::valueType> *detNode = new RevBayesCore::DeterministicNode<typename retType::valueType>("", func);
    
    retType* value = new retType( detNode );
    
    return value;
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func_mult<firstValType, secondValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "first", true, firstValType::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "second", true, secondValType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func_mult<firstValType, secondValType, retType>::getClassName(void) { 
    
    static std::string rbClassName = "mult<" + firstValType::getClassName() + "," + secondValType::getClassName() + "," + retType::getClassName() + ">";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_mult<firstValType, secondValType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_mult<firstValType, secondValType, retType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = retType::getClassTypeSpec();
    
    return returnTypeSpec;
}


template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func_mult<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif