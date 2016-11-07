#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "RlClade.h"
#include "RlTraceTree.h"
#include "RlTree.h"
#include "RlUtils.h"


//TraceTree::TraceTree() : WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>(),
//    tree_summary()
//{
//    
//    // initialize the methods
//    initMethods();
//    
//}



TraceTree::TraceTree(const RevBayesCore::TraceTree &m) : WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>( new RevBayesCore::TraceTree( m ) ),
    tree_summary( *this->value )
{
    
    // initialize the methods
    initMethods();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */

TraceTree* TraceTree::clone(void) const
{
    
    return new TraceTree(*this);
}



void TraceTree::constructInternalObject( void )
{
    throw RbException("We do not support a constructor function for TraceTree.");
}


/* Map calls to member methods */

RevPtr<RevVariable> TraceTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    
    if ( name == "setBurnin" )
    {
        found = true;
        
        double f = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        
        
        int b = int( floor( this->value->size()*f ) );
        tree_summary.setBurnin( b );
        
    }
    else if ( name == "summarize" )
    {
        found = true;
        
        double treeCI       = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        double minCladeProb = static_cast<const Probability &>( args[1].getVariable()->getRevObject() ).getValue();
        
        tree_summary.summarizeTrees();
        tree_summary.printTreeSummary(std::cout, treeCI);
        tree_summary.summarizeClades( true );
        tree_summary.printCladeSummary(std::cout, minCladeProb);
        
        return NULL;
    }
    else if ( name == "cladeProbability" )
    {
        found = true;
        
        const RevBayesCore::Clade &c    = static_cast<const Clade &>( args[0].getVariable()->getRevObject() ).getValue();
        
        double p = tree_summary.cladeProbability( c );
        
        return new RevVariable( new Probability( p ) );

    }
    else if ( name == "getNumberSamples" )
    {
        found = true;
        
        int n = tree_summary.getNumberSamples();
        
        return new RevVariable( new Natural( n ) );
    }
    else if ( name == "getTree" )
    {
        found = true;
        
        // get the index which is the only argument for this method
        int i    = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        
        const RevBayesCore::Tree &current_tree = this->value->objectAt( i );
        
        return new RevVariable( new Tree( current_tree ) );
    }
    else if ( name == "getTopologyFrequency" )
    {
        found = true;
        
        // get the tree which is the only argument for this method
        const RevBayesCore::Tree &current_tree = static_cast<const Tree &>( args[0].getVariable()->getRevObject() ).getValue();
        tree_summary.summarizeTrees();
        double p = tree_summary.getTopologyFrequency( current_tree );
        
        return new RevVariable( new Probability( p ) );
    }
    else if ( name == "getUniqueTrees" )
    {
        found = true;
        
        double tree_CI       = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        
        tree_summary.summarizeTrees();
        std::vector<RevBayesCore::Tree> trees = tree_summary.getUniqueTrees(tree_CI);
        
        ModelVector<Tree> *rl_trees = new ModelVector<Tree>;
        for (size_t i=0; i<trees.size(); ++i)
        {
            rl_trees->push_back( Tree( trees[i] ) );
        }
        
        return new RevVariable( rl_trees );
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */

const std::string& TraceTree::getClassType(void)
{
    
    static std::string revType = "TraceTree";
    
    return revType;
}

/** Get class type spec describing type of object */

const TypeSpec& TraceTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::TraceTree>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}



/** Return member rules (no members) */

const MemberRules& TraceTree::getParameterRules(void) const
{
    
    static MemberRules modelMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        rules_set = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */

const TypeSpec& TraceTree::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}



void TraceTree::initMethods( void )
{
    
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), "The fraction of samples to disregard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    this->methods.addFunction( new MemberProcedure( "setBurnin", RlUtils::Void, burninArgRules) );
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), "The size of the credible set to print.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), "The minimum clade probability used when printing.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );
    
    ArgumentRules* cladeProbArgRules = new ArgumentRules();
    cladeProbArgRules->push_back( new ArgumentRule("clade", Clade::getClassTypeSpec(), "The (monophyletic) clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY) );
    this->methods.addFunction( new MemberProcedure( "cladeProbability", Probability::getClassTypeSpec(), cladeProbArgRules) );
    
    ArgumentRules* getNumberSamplesArgRules = new ArgumentRules();
    this->methods.addFunction( new MemberProcedure( "getNumberSamples", Natural::getClassTypeSpec(), getNumberSamplesArgRules) );
    
    ArgumentRules* getTreeArgRules = new ArgumentRules();
    getTreeArgRules->push_back( new ArgumentRule("index", Natural::getClassTypeSpec(), "The index of the tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY) );
    this->methods.addFunction( new MemberProcedure( "getTree", Tree::getClassTypeSpec(), getTreeArgRules) );
    
    ArgumentRules* getUniqueTreesArgRules = new ArgumentRules();
    getUniqueTreesArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), "The size of the credible set.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    this->methods.addFunction( new MemberProcedure( "getUniqueTrees", ModelVector<Tree>::getClassTypeSpec(), getUniqueTreesArgRules) );
    
    ArgumentRules* getTopologyFrequencyArgRules = new ArgumentRules();
    getTopologyFrequencyArgRules->push_back( new ArgumentRule("tree", Tree::getClassTypeSpec(), "The tree.", ArgumentRule::BY_VALUE, ArgumentRule::ANY) );
    this->methods.addFunction( new MemberProcedure( "getTopologyFrequency", Probability::getClassTypeSpec(), getTopologyFrequencyArgRules) );
    
    
    
}


/** Get type spec */

void TraceTree::printValue(std::ostream &o) const
{
    
    o << "TraceTree";
}


/** Set a member variable */

void TraceTree::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "xxx")
    {
        
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}
