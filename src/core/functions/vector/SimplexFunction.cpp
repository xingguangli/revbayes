//
//  SimplexFunction.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SimplexFunction.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;

SimplexFunction::SimplexFunction(const std::vector<const TypedDagNode<double> *> &args) : TypedFunction< RbVector<double> >( new RbVector<double>() ),
    simplexParams( args )
{
    // add the lambda parameter as a parent
    std::vector<const TypedDagNode<double>* >::iterator it;
    for (it = simplexParams.begin(); it != simplexParams.end(); ++it) {
        this->addParameter( *it );
    }
    
    update();
}


SimplexFunction::~SimplexFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



SimplexFunction* SimplexFunction::clone( void ) const {
    return new SimplexFunction( *this );
}


void SimplexFunction::update( void ) {
    
    // empty current simplex
    value->clear();
    
    double sum = 0.0;
    std::vector<const TypedDagNode<double>* >::iterator it;
    for (it = simplexParams.begin(); it != simplexParams.end(); ++it) {
        sum += (*it)->getValue();
    }
    for (it = simplexParams.begin(); it != simplexParams.end(); ++it) {
        value->push_back( (*it)->getValue() / sum );
    }
}



void SimplexFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    for (size_t i = 0; i < simplexParams.size(); ++i) {
        if (oldP == simplexParams[i])
        {
            simplexParams[i] = static_cast<const TypedDagNode<double>* >( newP );
            // don't jump out of the loop because we could have the same parameter multiple times for this vector, e.g., v(a,a,b,a)
        }
    }
    
}
