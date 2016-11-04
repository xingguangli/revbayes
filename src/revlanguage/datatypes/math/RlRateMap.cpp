#include "RlRateMap.h"

#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"

using namespace RevLanguage;

RateMap::RateMap(void) : ModelObject<RevBayesCore::RateMap>()
{
    
    // add method for call "x.nChars()" as a function
    ArgumentRules* nCharsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nChars", Natural::getClassTypeSpec(), nCharsArgRules) );
    
    // add method for call "x.nStates()" as a function
    ArgumentRules* nStatesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nStates", Natural::getClassTypeSpec(), nStatesArgRules) );

}


RateMap::RateMap( RevBayesCore::RateMap *v) : ModelObject<RevBayesCore::RateMap>( v )
{
    
    // add method for call "x.nChars()" as a function
    ArgumentRules* nCharsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nChars", Natural::getClassTypeSpec(), nCharsArgRules) );
    
    // add method for call "x.nStates()" as a function
    ArgumentRules* nStatesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nStates", Natural::getClassTypeSpec(), nStatesArgRules) );
    
}


RateMap::RateMap( RevBayesCore::TypedDagNode<RevBayesCore::RateMap> *m) : ModelObject<RevBayesCore::RateMap>( m )
{
    
    // add method for call "x.nChars()" as a function
    ArgumentRules* nCharsArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nChars", Natural::getClassTypeSpec(), nCharsArgRules) );
    
    // add method for call "x.nStates()" as a function
    ArgumentRules* nStatesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "nStates", Natural::getClassTypeSpec(), nStatesArgRules) );
    
}


RateMap* RateMap::clone() const
{
    return new RateMap( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> RateMap::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
   
    if (name == "nChars")
    {
        found = true;
        
        int n = (int)this->dagNode->getValue().getNumberOfCharacters();
        return new RevVariable( new Natural(n) );
    }
    else if (name == "nStates")
    {
        found = true;
        
        int n = (int)this->dagNode->getValue().getNumberOfStates();
        return new RevVariable( new Natural(n) );
    }

    return ModelObject<RevBayesCore::RateMap>::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& RateMap::getClassType(void) {
    
    static std::string revType = "RateMap";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RateMap::getClassTypeSpec(void) {
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMap::getTypeSpec(void) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}
