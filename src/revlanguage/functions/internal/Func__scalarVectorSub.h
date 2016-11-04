#ifndef Func__scalarVectorSub_H
#define Func__scalarVectorSub_H

#include "ModelVector.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the scalar-vector-subtraction function.
     *
     * The RevLanguage wrapper of the scalar-vector-subtraction function connects
     * the variables/parameters of the function and creates the internal ScalarVectorSubtraction object.
     * RevBayes > a <- [1,2,3]
     * RevBayes > b <- 2
     * RevBayes > a - b
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-07-31, version 1.0
     *
     */
    template <typename firstValType, typename secondValType, typename retType>
    class Func__scalarVectorSub :  public TypedFunction< ModelVector< retType > > {
        
    public:
        Func__scalarVectorSub( void );
        
        // Basic utility functions
        Func__scalarVectorSub*                                          clone(void) const;                              //!< Clone the object
        static const std::string&                                       getClassType(void);                             //!< Get class name
        static const TypeSpec&                                          getClassTypeSpec(void);                         //!< Get class type spec
        std::string                                                     getFunctionName(void) const;                    //!< Get the primary name of the function in Rev
        const TypeSpec&                                                 getTypeSpec(void) const;                        //!< Get the type spec of the instance
        bool                                                            isInternal(void) const { return true; }         //!< Is this an internal function?

        // Implementations of pure virtual functions of the base class(es)
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename retType::valueType> >*       createFunction(void) const ;                    //!< Create a random variable from this distribution
        const ArgumentRules&                                            getArgumentRules(void) const;                   //!< Get argument rules
        
    private:
        
    };
    
}

#include "ScalarVectorSubtraction.h"
#include "RlDeterministicNode.h"
#include "StringUtilities.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::Func__scalarVectorSub( void ) : TypedFunction<ModelVector<retType> >( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>* RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::clone( void ) const
{
    
    return new Func__scalarVectorSub( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename retType::valueType> >* RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::createFunction( void ) const
{
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<typename secondValType::valueType> >* secondArg = static_cast<const ModelVector<secondValType> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::ScalarVectorSubtraction<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType> *func = new RevBayesCore::ScalarVectorSubtraction<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType>(firstArg, secondArg);
    
    return func;
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "first" , firstValType::getClassTypeSpec() , "The left hand side variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "second", ModelVector<secondValType>::getClassTypeSpec(), "The right hand side variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::getClassType(void)
{
    static std::string revType = "Func__scalarVectorSub<" + firstValType::getClassType() + "," + secondValType::getClassType() + "," + retType::getClassType() + ">";
    
    return revType;
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( Func__scalarVectorSub<firstValType, secondValType, retType>::getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
template <typename firstValType, typename secondValType, typename retType>
std::string RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "sub";
    
    return f_name;
}


template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__scalarVectorSub<firstValType, secondValType, retType>::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}

#endif
