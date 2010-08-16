/**
 * @file
 * This file contains the implementation of ConstructorFunction, which is used
 * for functions that construct member objects.
 *
 * @brief Implementation of ConstructorFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "MemberObject.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Constructor */
ConstructorFunction::ConstructorFunction(MemberObject* obj) 
    : RbFunction(), templateObject(obj) {
}


/** Clone the object */
ConstructorFunction* ConstructorFunction::clone(void) const {

    return new ConstructorFunction(*this);
}


/** Pointer-based equals comparison */
bool ConstructorFunction::equals(const RbObject* x) const {

    return false;
}


/** Execute operation: we reset our template object here */
RbObject* ConstructorFunction::executeOperation(const std::vector<DAGNode*>& args) {

    const ArgumentRules&          argRules = getArgumentRules();

    MemberObject* copy = (MemberObject*)(templateObject->clone());

    ArgumentRules::const_iterator i;
    std::vector<DAGNode*>::const_iterator j;
    for (i=argRules.begin(), j=args.begin(); i!=argRules.end(); i++, j++) {
        if ((*i)->isWrapperRule())
            copy->setVariable((*i)->getLabel(), (*j));
        else
            copy->setValue((*i)->getLabel(), (*j)->getValue()->clone());
    }
 
    return copy;
}


/** Get argument rules */
const ArgumentRules& ConstructorFunction::getArgumentRules(void) const {

    return templateObject->getMemberRules();
}


/** Get class vector describing type of object */
const StringVector& ConstructorFunction::getClass(void) const { 

    static StringVector rbClass = StringVector(ConstructorFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const std::string& ConstructorFunction::getReturnType(void) const {

    return templateObject->getType();
}

