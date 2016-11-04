#ifndef ScalarVectorAddition_H
#define ScalarVectorAddition_H

#include "RbVector.h"
#include "StringUtilities.h"    // For string concatenation through addition
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore {
    
    
    /**
     * \brief Addition between a scalar and a vector.
     *
     * Here we add a scalar to a vector as a function to enable the use in deterministic expressions.
     * This class has three template parameters to specify the corresponding types.
     * We completely rely on overloade operator+ functionality.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-08-13
     *
     */
    template <class firstValueType, class secondValueType, class returnType>
    class ScalarVectorAddition : public TypedFunction< RbVector<returnType> > {
        
    public:
        ScalarVectorAddition(const TypedDagNode<firstValueType> *a, const TypedDagNode< RbVector<secondValueType> > *b);
        
        ScalarVectorAddition*                               clone(void) const;                                                          //!< Create a clon.
        void                                                update(void);                                                               //!< Recompute the value
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        const TypedDagNode<firstValueType>*                 a;
        const TypedDagNode< RbVector<secondValueType> >*    b;
        
    };
}



template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorAddition<firstValueType, secondValueType, returnType>::ScalarVectorAddition(const TypedDagNode<firstValueType> *l, const TypedDagNode< RbVector<secondValueType> > *r) : TypedFunction< RbVector<returnType> >( new RbVector<returnType>(r->getValue().size(),returnType()) ),
a( l ),
b( r )
{
    this->addParameter( l );
    this->addParameter( r );
    
}


template<class firstValueType, class secondValueType, class returnType>
RevBayesCore::ScalarVectorAddition<firstValueType, secondValueType, returnType>* RevBayesCore::ScalarVectorAddition<firstValueType, secondValueType, returnType>::clone( void ) const
{
    
    return new ScalarVectorAddition(*this);
}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::ScalarVectorAddition<firstValueType, secondValueType, returnType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == a)
    {
        a = static_cast<const TypedDagNode<firstValueType>* >( newP );
    }
    
    if (oldP == b)
    {
        b = static_cast<const TypedDagNode< RbVector<secondValueType> >* >( newP );
    }
}


template<class firstValueType, class secondValueType, class returnType>
void RevBayesCore::ScalarVectorAddition<firstValueType, secondValueType, returnType>::update( void )
{
    // remove the old values
    
    const firstValueType &lhs = a->getValue();
    const std::vector<secondValueType> &rhs = b->getValue();
    size_t l = rhs.size();
    for (size_t i=0; i<l; ++i)
    {
        (*this->value)[i] = lhs + rhs[i];
    }
    
}


#endif
