#include "Mntr_JointConditionalAncestralState.h"
#include "AbstractCharacterData.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OptionRule.h"
#include "Model.h"
#include "RlMonitor.h"
#include "ModelVector.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlModel.h"
#include "RlTimeTree.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "NaturalNumbersState.h"
#include "DnaState.h"
#include "StandardState.h"
#include "RnaState.h"
#include "AminoAcidState.h"
#include "PomoState.h"
#include "RestrictionState.h"

using namespace RevLanguage;

Mntr_JointConditionalAncestralState::Mntr_JointConditionalAncestralState(void) : Monitor()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Mntr_JointConditionalAncestralState* Mntr_JointConditionalAncestralState::clone(void) const
{
    
    return new Mntr_JointConditionalAncestralState(*this);
}


void Mntr_JointConditionalAncestralState::constructInternalObject( void )
{
    const std::string&                  fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&                  sep     = static_cast<const RlString &>( separator->getRevObject() ).getValue();
    int                                 g       = static_cast<const Natural  &>( printgen->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* t = static_cast<const Tree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* ctmc_tdn = NULL;
    RevBayesCore::StochasticNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* ctmc_sn = NULL;
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* cdbdp_tdn = NULL;
    RevBayesCore::StochasticNode<RevBayesCore::Tree>* cdbdp_sn = NULL;
    
    if ( static_cast<const RevLanguage::AbstractHomologousDiscreteCharacterData&>( ctmc->getRevObject() ).isModelObject() )
    {
        ctmc_tdn = static_cast<const RevLanguage::AbstractHomologousDiscreteCharacterData&>( ctmc->getRevObject() ).getDagNode();
        ctmc_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* >(ctmc_tdn);
    }
    else if ( static_cast<const RevLanguage::Tree&>( cdbdp->getRevObject() ).isModelObject() )
    {
        cdbdp_tdn = static_cast<const RevLanguage::Tree&>( cdbdp->getRevObject() ).getDagNode();
        cdbdp_sn  = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree>* >( cdbdp_tdn );
    }
    else
    {
        throw RbException("mnJointConditionalAncestralStateMonitor requires either a CTMC or a character-dependent birth death process (CDBDP).");
    }
    
    bool                                ap      = static_cast<const RlBoolean &>( append->getRevObject() ).getValue();
    bool                                wt      = static_cast<const RlBoolean &>( withTips->getRevObject() ).getValue();
    bool                                wss     = static_cast<const RlBoolean &>( withStartStates->getRevObject() ).getValue();
    std::string							character = static_cast<const RlString &>( monitorType->getRevObject() ).getValue();
    
    delete value;
    if (character == "AA" || character == "Protein")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::AminoAcidState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::AminoAcidState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else if (character == "DNA")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else if (character == "NaturalNumbers")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::NaturalNumbersState> *m;
        if ( static_cast<const RevLanguage::AbstractHomologousDiscreteCharacterData&>( ctmc->getRevObject() ).isModelObject() )
        {
            m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::NaturalNumbersState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        }
        else
        {
            m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::NaturalNumbersState>(cdbdp_sn, (unsigned long)g, fn, sep, wt, wss);
        }
        m->setAppend( ap );
        value = m;
    }
    else if (character == "Pomo")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::PomoState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::PomoState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else if (character == "RNA")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::DnaState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else if (character == "Standard")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::StandardState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::StandardState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else if (character == "Restriction")
    {
        RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::RestrictionState> *m;
        m = new RevBayesCore::JointConditionalAncestralStateMonitor<RevBayesCore::RestrictionState>(t, ctmc_sn, (unsigned long)g, fn, sep, wt, wss);
        m->setAppend( ap );
        value = m;
    }
    else
    {
        throw RbException( "Incorrect character type specified. Valid options are: AA, DNA, NaturalNumbers, Pomo, Protein, RNA, Standard, Restriction" );
    }
    
}


/** Get Rev type of object */
const std::string& Mntr_JointConditionalAncestralState::getClassType(void)
{
    
    static std::string revType = "Mntr_JointConditionalAncestralState";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Mntr_JointConditionalAncestralState::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Monitor::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Mntr_JointConditionalAncestralState::getMonitorName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "JointConditionalAncestralState";
    
    return c_name;
}


/** Return member rules (no members) */
const MemberRules& Mntr_JointConditionalAncestralState::getParameterRules(void) const
{
    
    static MemberRules asMonitorMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        asMonitorMemberRules.push_back( new ArgumentRule("tree"           , Tree::getClassTypeSpec() , "", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("ctmc"           , AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL ) );
        asMonitorMemberRules.push_back( new ArgumentRule("cdbdp"          , TimeTree::getClassTypeSpec(), "", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL) );
        asMonitorMemberRules.push_back( new ArgumentRule("filename"       , RlString::getClassTypeSpec() , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("type"           , RlString::getClassTypeSpec() , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        asMonitorMemberRules.push_back( new ArgumentRule("printgen"       , Natural::getClassTypeSpec()  , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        asMonitorMemberRules.push_back( new ArgumentRule("separator"      , RlString::getClassTypeSpec() , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        asMonitorMemberRules.push_back( new ArgumentRule("append"         , RlBoolean::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        asMonitorMemberRules.push_back( new ArgumentRule("withTips"       , RlBoolean::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        asMonitorMemberRules.push_back( new ArgumentRule("withStartStates", RlBoolean::getClassTypeSpec(), "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rules_set = true;
    }
    
    return asMonitorMemberRules;
}

/** Get type spec */
const TypeSpec& Mntr_JointConditionalAncestralState::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Mntr_JointConditionalAncestralState::printValue(std::ostream &o) const
{
    
    o << "Mntr_JointConditionalAncestralState";
}


/** Set a member variable */
void Mntr_JointConditionalAncestralState::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "" ) {
        vars.push_back( var );
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "separator" )
    {
        separator = var;
    }
    else if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "type" )
    {
        monitorType = var;
    }
    else if ( name == "ctmc" )
    {
        ctmc = var;
    }
    else if ( name == "cdbdp" )
    {
        cdbdp = var;
    }
    else if ( name == "printgen" )
    {
        printgen = var;
    }
    else if ( name == "append" )
    {
        append = var;
    }
    else if ( name == "withTips" )
    {
        withTips = var;
    }
    else if ( name == "withStartStates" )
    {
        withStartStates = var;
    }
    
    else
    {
        Monitor::setConstParameter(name, var);
    }
    
}
