#include "FixedNodeheightPruneAndRegraftProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
FixedNodeheightPruneAndRegraftProposal::FixedNodeheightPruneAndRegraftProposal( StochasticNode<Tree> *n ) : Proposal(),
    variable( n )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void FixedNodeheightPruneAndRegraftProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
FixedNodeheightPruneAndRegraftProposal* FixedNodeheightPruneAndRegraftProposal::clone( void ) const
{
    
    return new FixedNodeheightPruneAndRegraftProposal( *this );
}



void FixedNodeheightPruneAndRegraftProposal::findNewBrothers(std::vector<TopologyNode *> &b, TopologyNode &p, TopologyNode *n) {
    // security check that I'm not a tip
    if (!n->isTip() && &p != n)
    {
        // check the first child
        TopologyNode* child = &n->getChild( 0 );
        if ( child->getAge() < p.getAge() )
        {
            b.push_back( child );
        }
        else
        {
            findNewBrothers(b, p, child);
        }
        
        // check the second child
        child = &n->getChild( 1 );
        if ( child->getAge() < p.getAge() )
        {
            b.push_back( child );
        }
        else
        {
            findNewBrothers(b, p, child);
        }
    }
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& FixedNodeheightPruneAndRegraftProposal::getProposalName( void ) const
{
    static std::string name = "FNPR";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Beta-simplex proposal randomly changes some values of a simplex, although the other values
 * change too because of the renormalization.
 * First, some random indices are drawn. Then, the proposal draws a new somplex
 *   u ~ Beta(val[index] * alpha)
 * where alpha is the tuning parameter.The new value is set to u.
 * The simplex is then renormalized.
 *
 * \return The hastings ratio.
 */
double FixedNodeheightPruneAndRegraftProposal::doProposal( void )
{
    
    // reset flags
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    
    TopologyNode* parent        = &node->getParent();
    TopologyNode* grandparent   = &parent->getParent();
    TopologyNode* brother       = &parent->getChild( 0 );
    // check if we got the correct child
    if ( brother == node )
    {
        brother = &parent->getChild( 1 );
    }
    
    // collect the possible reattachement points
    std::vector<TopologyNode*> new_brothers;
    findNewBrothers(new_brothers, *parent, &tau.getRoot());
    
    // we only need to propose a new tree if there are any other re-attachement points
    if ( new_brothers.size() < 1)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    size_t index = size_t(rng->uniform01() * new_brothers.size());
    TopologyNode* newBro = new_brothers[index];
    
    
    // now we store all necessary values
    storedBrother       = brother;
    storedNewBrother    = newBro;
    
    // prune
    grandparent->removeChild( parent );
    parent->removeChild( brother );
    grandparent->addChild( brother );
    brother->setParent( grandparent );
    
    // regraft
    TopologyNode* newGrandParent = &newBro->getParent();
    newGrandParent->removeChild( newBro );
    newGrandParent->addChild( parent );
    parent->setParent( newGrandParent );
    parent->addChild( newBro );
    newBro->setParent( parent );
    
    return 0.0;
}


/**
 *
 */
void FixedNodeheightPruneAndRegraftProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void FixedNodeheightPruneAndRegraftProposal::printParameterSummary(std::ostream &o) const
{
    
    // no parameters
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void FixedNodeheightPruneAndRegraftProposal::undoProposal( void )
{
    
    // we undo the proposal only if it didn't fail
    if ( !failed )
    {
        // undo the proposal
        TopologyNode& parent = storedNewBrother->getParent();
        TopologyNode& newGrandparent = parent.getParent();
        TopologyNode& grandparent = storedBrother->getParent();
        
        // prune
        newGrandparent.removeChild( &parent );
        parent.removeChild( storedNewBrother );
        newGrandparent.addChild( storedNewBrother );
        storedNewBrother->setParent( &newGrandparent );
        
        
        // regraft
        grandparent.removeChild( storedBrother );
        parent.addChild( storedBrother );
        storedBrother->setParent( &parent );
        grandparent.addChild( &parent );
        parent.setParent( &grandparent );
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void FixedNodeheightPruneAndRegraftProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<Tree>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void FixedNodeheightPruneAndRegraftProposal::tune( double rate )
{
    
    // nothing to tune
    
}

