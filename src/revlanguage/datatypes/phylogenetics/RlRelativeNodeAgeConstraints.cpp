#include "ArgumentRule.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbVector.h"
#include "RealPos.h"
#include "RlRelativeNodeAgeConstraints.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlSimplex.h"
#include "RevVariable.h"
#include <vector>

using namespace RevLanguage;

RlRelativeNodeAgeConstraints::RlRelativeNodeAgeConstraints(void) : ModelObject<RevBayesCore::RelativeNodeAgeConstraints>( )
{
    
    ArgumentRules* nameArgRules               = new ArgumentRules();
    
    methods.addFunction( new MemberProcedure( "filename", RlString::getClassTypeSpec(), nameArgRules           ) );
    
}


RlRelativeNodeAgeConstraints::RlRelativeNodeAgeConstraints( RevBayesCore::RelativeNodeAgeConstraints *v) : ModelObject<RevBayesCore::RelativeNodeAgeConstraints>( v )
{
    
    ArgumentRules* nameArgRules               = new ArgumentRules();
    
    methods.addFunction( new MemberProcedure( "filename", RlString::getClassTypeSpec(), nameArgRules           ) );
    
}


RlRelativeNodeAgeConstraints::RlRelativeNodeAgeConstraints( const RevBayesCore::RelativeNodeAgeConstraints &v) : ModelObject<RevBayesCore::RelativeNodeAgeConstraints>( v.clone() )
{
    
    ArgumentRules* nameArgRules               = new ArgumentRules();
    
    methods.addFunction( new MemberProcedure( "filename", RlString::getClassTypeSpec(), nameArgRules           ) );
    
}


RlRelativeNodeAgeConstraints::RlRelativeNodeAgeConstraints( RevBayesCore::TypedDagNode<RevBayesCore::RelativeNodeAgeConstraints> *m) : ModelObject<RevBayesCore::RelativeNodeAgeConstraints>( m )
{
    
    ArgumentRules* nameArgRules               = new ArgumentRules();
    
    methods.addFunction( new MemberProcedure( "filename", RlString::getClassTypeSpec(), nameArgRules           ) );
    
}


RlRelativeNodeAgeConstraints* RlRelativeNodeAgeConstraints::clone() const {
    return new RlRelativeNodeAgeConstraints( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> RlRelativeNodeAgeConstraints::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "filename")
    {
        found = true;
        
        std::string name = this->dagNode->getValue().getFilename();
        RlString *n = new RlString(name);
        return new RevVariable( n );
    }    
    
    return ModelObject<RevBayesCore::RelativeNodeAgeConstraints>::executeMethod( name, args, found );
}




/*
const Real* RlRelativeNodeAgeConstraints::getElement(size_t idx ) const
{
    double element = static_cast< RevBayesCore::RelativeNodeAgeConstraints& >( this->dagNode->getValue() ).getElement(idx - 1);
    
    return new Real( element ) ;
    
}
*/

/**
 * Size of the matrix.
 */
size_t RlRelativeNodeAgeConstraints::getNumberOfConstraints( void ) const
{
    return this->dagNode->getValue().getNumberOfConstraints();
}


/* Get Rev type of object */
const std::string& RlRelativeNodeAgeConstraints::getClassType(void) {
    
    static std::string revType = "RlRelativeNodeAgeConstraints";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlRelativeNodeAgeConstraints::getClassTypeSpec(void) {
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlRelativeNodeAgeConstraints::getTypeSpec(void) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}

