/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RevObject.
 *
 * @brief Partial implementation of RevObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RevObject
 *
 * $Id$
 */

#include "MemberProcedure.h"
#include "MethodTable.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevObject.h"
#include "RlUtils.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>
#include <stdio.h>

using namespace RevLanguage;


/** 
 * Destructor. 
 * Nothing to do here because we don't own anything.
 */
RevObject::~RevObject( void )
{
    
}


/**
 * Addition operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RevObject* RevObject::add(const RevObject &rhs) const
{
    throw RbException("Cannot add a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}


/** Clone the model DAG connected to this object. */
RevObject* RevObject::cloneDAG( std::map<const RevBayesCore::DagNode*, RevBayesCore::DagNode*>& nodesMap ) const
{
    throw RbException( "Rev object with no DAG node should not be included in model DAG" );
}


/** The default implementation does nothing because we don't have an internal object */
void RevObject::constructInternalObject( void ) {
    // nothing to do
}



/** Convert to type and dim. The caller manages the returned object. */
RevObject* RevObject::convertTo(const TypeSpec& typeSpec) const {
        
    throw RbException("Failed conversion from type '" + getType() + "' to type '" + typeSpec.getType() + "'" );
    
    return NULL;
}


/**
 * Decrement operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RevObject::decrement( void ) 
{
    throw RbException("Cannot decrement a value of type '" + this->getType() + "'.");

}


/**
 * Division operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RevObject* RevObject::divide(const RevObject &rhs) const
{
    throw RbException("Cannot divide a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}


/** 
 * Execute simple method. 
 */
RevPtr<Variable> RevObject::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
     if ( name == "methods" )
    {
        // just print the method names (including inherited methods)
        const MethodTable &m = getMethods();
        m.printValue(std::cout, true);
        
        return NULL;
    }
    else 
    {
        throw RbException( "No mapping from member method '" + name + "' to internal function call provided" );
    }
}


/** Get class vector describing type of object */
const std::string& RevObject::getClassType(void)
{
    
    static std::string revType = "RevObject";
	return revType; 
}


/**
 * Get class vector describing type of object. We are
 * the base class of all other classes, so parent is NULL.
 */
const TypeSpec& RevObject::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), NULL );
	
    return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& RevObject::getParameterRules(void) const
{
    
    static const MemberRules rules = MemberRules();
    
    return rules;
}


/**
 * Get common member methods. Here we
 */
const MethodTable& RevObject::getMethods( void ) const
{
    
    // initialize the methods if it hasn't happened yet
    if ( methodsInitialized == false )
    {
        initializeMethods();
        
        methodsInitialized = true;
    }
    
    return methods;
}


/**
 * Get the Rev type of the object
 */
const std::string& RevObject::getType( void ) const
{
    
    return getTypeSpec().getType();
}


/**
 * Get the value as a DAG node. This default implementation throws an error.
 */
RevBayesCore::DagNode* RevObject::getDagNode( void ) const
{
    
    throw RbException("RevLanguage only objects cannot be used inside DAG's! You tried to access the DAG node of a '" + getClassType() + "'.");
    
    return NULL;
}


/**
 * Increment operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 */
void RevObject::increment( void ) 
{
    
    throw RbException("Cannot increment a value of type '" + this->getType() + "'.");
    
}


/**
 * The default implementation is that the object is not an abstract type object.
 * Only abstract type objects need to override this.
 */
bool RevObject::isAbstract( void ) const
{
    return false;
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? By default we
 * return true, which is the safest option if a derived class
 * forgets to override this function.
 */
bool RevObject::isAssignable( void ) const
{
    return true;
}


/**
 * The default implementation is that the variable is constant. Only variables which actually store
 * internally DAG nodes have to ask the DAG nodes if they are constant.
 */
bool RevObject::isConstant( void ) const
{

    return true;
}


/** Is convertible to type? */
bool RevObject::isConvertibleTo(const TypeSpec& type, bool once) const
{
    
    return false;
}


/* Are we of specified language type? */
bool RevObject::isTypeSpec(const TypeSpec& typeSpec) const
{
    
    return getTypeSpec().isDerivedOf( typeSpec );
}


/**
 * The default implementation is that the variable is not a model object. Only variables which actually store
 * internally DAG nodes are model objects.
 */
bool RevObject::isModelObject( void ) const
{
    
    return false;
}


/**
  * Converting the value of the internal variable to a constant. The default implementation does nothing because we don't have a DAG node as our internal variable.
  * Note, RevLanguage types which can be used as types in the DAG should overwrite this method.
  */
void RevObject::makeConstantValue( void )
{
    // do nothing

}


/**
 * Convert a model object to a conversion object, the value of which is determined by a type
 * conversion from a specified variable. By default we throw an error, since we do not have
 * a DAG node and cannot perform the requested action.
 */
void RevObject::makeConversionValue( RevPtr<Variable> var )
{
    throw RbException( "Object without DAG node cannot be made a conversion value" );
}


/**
 * Make a new object that is an indirect deterministic reference to the object.
 * The default implementation throws an error.
 */
RevObject* RevObject::makeIndirectReference(void)
{
    std::ostringstream msg;
    msg << "The type '" << getClassType() << "' not supported in indirect reference assignments (yet)";
    throw RbException( msg );
}


/**
 * Make methods common to all member objects.
 * We support one method:
 * 1) methods()
 */
void RevObject::initializeMethods(void) const
{
    
    ArgumentRules* getMethodsArgRules = new ArgumentRules();
    
    // Add the 'methods()' method
    methods.addFunction("methods", new MemberProcedure(RlUtils::Void, getMethodsArgRules) );
        
}


/**
 * Convert the object to a deterministic object with a user-defined Rev function inside it.
 */
void RevObject::makeUserFunctionValue( UserFunction* fxn )
{
    std::ostringstream msg;
    msg << "The type '" << getClassType() << "' not supported in user-defined function nodes (yet)";
    throw RbException( msg );
}


/**
 * Multiplication operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RevObject* RevObject::multiply(const RevObject &rhs) const
{
    throw RbException("Cannot multiply a value of type '" + this->getType() + "' to a value of type '" + rhs.getType() + "'.");
    
    return NULL;
}


/**
 * Print the structural information for this object. Here we print the
 * type and type spec, as well as the value. Objects that have more
 * complex structure need to override this function, best by calling
 * it first and then provide the additional information.
 */
void RevObject::printStructure( std::ostream &o, bool verbose ) const
{
    o << "_RevType      = " << getType() << std::endl;
    o << "_RevTypeSpec  = [ " << getTypeSpec() << " ]" << std::endl;
    o << "_value        = ";
    
    std::ostringstream o1;
    printValue( o1 );
    o << StringUtilities::oneLiner( o1.str(), 54 ) << std::endl;

}


/**
 * Replace the variable. This default implementation does nothing.
 */
void RevObject::replaceVariable(RevObject *newVar)
{
    
}


/** Set a member variable */
void RevObject::setConstParameter(const std::string& name, const RevPtr<const Variable> &var)
{
    
    throw RbException("No constant member with name \"" + name + "\" found to set.");
}


/* Set a member variable.
 * In this default implementation, we delegate to setConstParameter.
 * Derived classes of MemberObject who need non-const variable should overwrite this function.
 * If you don't care if the variable is const, then you should only overwrite the setConstParameter.
 */
void RevObject::setParameter(const std::string& name, const RevPtr<Variable> &var)
{
    
    setConstParameter(name, RevPtr<const Variable>( var ) );
    
}



/** 
 * Setting the name of the internal variable. The default implementation does nothing.
 * Note: Rev types that can be used as types in the DAG should override this method.
 */
void RevObject::setName(std::string const &n)
{
}


/**
 * Subtraction operation.
 * Since we don't know the types and thus don't know the special behavior we simply throw and error.
 *
 * \param[in]   rhs     The right hand side value of the division operation.
 *
 * \return              NULL
 */
RevObject* RevObject::subtract(const RevObject &rhs) const
{
    throw RbException("Cannot subtract a value of type '" + rhs.getType() + "' from a value of type '" + this->getType() + "'.");
    
    return NULL;
}


/**
 * Get the info about this object as a string.
 * By default we simply call the overloaded operator<<.
 *
 */
std::string RevObject::toString( void ) const
{
    std::stringstream o;
    printValue(o);
    
    return o.str();
}

/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RevObject& x) {
    
    x.printValue(o);
    return o;
}

/** Make sure we can print the value of the object easily */
std::ostream& RevLanguage::operator<<(std::ostream& o, const RevObject& x) {
    
    x.printValue(o);
    return o;
}

