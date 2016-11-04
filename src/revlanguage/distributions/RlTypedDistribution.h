#ifndef RlTypedDistribution_H
#define RlTypedDistribution_H

#include "Real.h"
#include "RlDistribution.h"
#include "TypedDistribution.h"

namespace RevLanguage {
    
    template <typename rlType>
    class TypedDistribution : public Distribution {
        
    public:
        virtual                                         ~TypedDistribution(void);                                                                  //!< Destructor
        TypedDistribution<rlType>(const TypedDistribution<rlType> &x);                                                                //!< Copy constuctor
        
        // The value type definition
        typedef typename rlType::valueType              rbValueType;
        typedef rlType                                  rlValueType;
        
        virtual const TypeSpec&                         getVariableTypeSpec(void) const;                                                    //!< Get the variable type spec of this distribution
        virtual rlType*                                 createRandomVariable(void) const;                                                   //!< Create a random variable from this distribution
        void                                            setVariable(rlType* v);
        void                                            printValue(std::ostream& o, bool user) const;                                                      //!< Print the general information on the function ('usage')

        // Basic utility functions you have to override
        virtual TypedDistribution<rlType>*              clone(void) const = 0;                                                              //!< Clone object
        static const std::string&                       getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
        
        
        // Distribution functions you have to override
        virtual RevBayesCore::TypedDistribution<rbValueType>*     createDistribution(void) const = 0;                                                 //!< Create a random variable from this distribution
        
        
    protected:
        TypedDistribution<rlType>(void);                                                                                                 //!< Basic constructor
        
        // this is the random variable is associated with the distribution
        // it should only be used for linking functions from the variable to the distribution
        rlType*                                         variable;
        
    };
    
    
}

#include "RlStochasticNode.h"
#include "TypedDistribution.h"

template <typename rlType>
RevLanguage::TypedDistribution<rlType>::TypedDistribution() : Distribution()
{
    
}



template <typename rlType>
RevLanguage::TypedDistribution<rlType>::TypedDistribution( const TypedDistribution<rlType> &d ) : Distribution(d)
{
    
}



template <typename rlType>
RevLanguage::TypedDistribution<rlType>::~TypedDistribution()
{
    
}


template <typename rlType>
rlType* RevLanguage::TypedDistribution<rlType>::createRandomVariable( void ) const
{
    
    RevBayesCore::TypedDistribution<typename rlType::valueType>* d = createDistribution();
    TypedDistribution<rlType> *rl_dist_copy = this->clone();
    RevBayesCore::TypedDagNode<typename rlType::valueType>* rv  = new StochasticNode<typename rlType::valueType>("", d, rl_dist_copy );
    
    rlType *rl_rv = new rlType(rv);
    rl_dist_copy->setVariable( rl_rv );
    rl_rv->addMethods( rl_dist_copy->getDistributionMethods() );
    
    return rl_rv;
}


/* Get Rev type of object */
template <typename rlType>
const std::string& RevLanguage::TypedDistribution<rlType>::getClassType(void)
{
    
//    static std::string revType = "Distribution<"+ rlType::getClassType() +">";
    static std::string revType = "Distribution__"+ rlType::getClassType();
    
	return revType; 
}


/* Get class type spec describing type of object */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TypedDistribution<rlType>::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/* Get Rev type of distribution variable */
template <typename rlType>
const RevLanguage::TypeSpec& RevLanguage::TypedDistribution<rlType>::getVariableTypeSpec( void ) const
{
    
    return rlType::getClassTypeSpec();
}


/* Set the internal variable */
template <typename rlType>
void RevLanguage::TypedDistribution<rlType>::setVariable(rlType *v)
{
    variable = v;
}


/** Print value for user */
template <typename rlType>
void RevLanguage::TypedDistribution<rlType>::printValue(std::ostream& o, bool user) const
{
    
    o << getDistributionFunctionName();
}


#endif

