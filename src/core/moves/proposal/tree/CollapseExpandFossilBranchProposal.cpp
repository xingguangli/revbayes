#include "DistributionUniform.h"
#include "CollapseExpandFossilBranchProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
CollapseExpandFossilBranchProposal::CollapseExpandFossilBranchProposal( StochasticNode<Tree> *n, TypedDagNode<double> *o ) : Proposal(),
    tau( n ),
    origin( o )
{
    // tell the base class to add the node
    addNode( tau );
    addNode( origin );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void CollapseExpandFossilBranchProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
CollapseExpandFossilBranchProposal* CollapseExpandFossilBranchProposal::clone( void ) const
{
    
    return new CollapseExpandFossilBranchProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& CollapseExpandFossilBranchProposal::getProposalName( void ) const
{
    static std::string name = "CollapseExpandFossilBranch";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Uniform-simplex proposal randomly changes some values of a simplex, although the other values
 * change too because of the renormalization.
 * First, some random indices are drawn. Then, the proposal draws a new somplex
 *   u ~ Uniform(val[index] * alpha)
 * where alpha is the tuning parameter.The new value is set to u.
 * The simplex is then renormalized.
 *
 * \return The hastings ratio.
 */
double CollapseExpandFossilBranchProposal::doProposal( void )
{
    
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree &t = tau->getValue();
    
    size_t num_fossils = 0;
    for (size_t i = 0; i < t.getNumberOfNodes(); ++i)
    {
        if ( t.getNode(i).isFossil() == true )
        {
            ++num_fossils;
        }
        
    }
    
    if ( num_fossils == 0)
    {
        throw RbException("Cannot perform collapse-expand-branch move on tree without fossils.");
    }
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(t.getNumberOfNodes() * u) );
        node = &t.getNode(index);
    } while ( node->isFossil() == false );
    
    // store the node which we selected
    storedNode = node;
    
    double hr = 0;
    if ( node->isSampledAncestor() == true )
    {
        hr += expandBranch( *node );
    }
    else
    {
        hr += collapseBranch( *node );
    }
    
    return hr;
    
}



/* Move a fossil tip (brl > 0) to be ancestral (brl =0)
 
 __|__              __|__
|     |            |     |
     q|___     -->       |
      |   |    -->       |
      |   |p            q|___p
     r|                 r|
 
 1. Pich a fossil among those with brl > 0 (prob = 1/m)
 2. Set brl = 0
 */
double CollapseExpandFossilBranchProposal::collapseBranch(TopologyNode &n)
{
    
    // Get the parent and sibling of the chosen node
    TopologyNode &parent = n.getParent();
    TopologyNode *sibling = &parent.getChild( 0 );
    if ( sibling == &n )
    {
        sibling = &parent.getChild( 1 );
    }
    
    // determine lower and upper bound of backward move
    double min_age = ( n.getAge() > sibling->getAge() ? n.getAge() : sibling->getAge() );
    double max_age = parent.getAge();
    if ( parent.isRoot() )
    {
        if (origin != NULL)
        {
            max_age = origin->getValue();
        }
    }
    else
    {
        max_age = parent.getParent().getAge();
    }
    
    // test that the max age is larger than the min age
    if ( max_age <= min_age && n.getAge() <= min_age )
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // store the old age of the parent
    storedAge = parent.getAge();
    
    // set the age of the parent node equal to my age
    parent.setAge( n.getAge() );
    n.setSampledAncestor( true );
    
    // compute the Jacobian term
    double lnJacobian = - log(max_age - min_age);
    
    return lnJacobian;
}



/* Move an ancestral fossil (brl = 0) to fossil tip (brl > 0)
 
    __|__              __|__
   |     |            |     |
         |       -->       q|___
         |       -->        |   |
        q|___p              |   |p
        r|                 r|
 
 1. Pich a fossil among those with brl = 0 (prob = 1/k)
 2. Propose brl from a uniform(0, ?) distribution
 */
double CollapseExpandFossilBranchProposal::expandBranch(TopologyNode &n)
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // Get the parent and sibling of the chosen node
    TopologyNode &parent = n.getParent();
    TopologyNode *sibling = &parent.getChild( 0 );
    if ( sibling == &n)
    {
        sibling = &parent.getChild( 1 );
    }
    
    // determine lower and upper bound of backward move
    double min_age = ( n.getAge() > sibling->getAge() ? n.getAge() : sibling->getAge() );
    double max_age = parent.getAge();
    if ( parent.isRoot() )
    {
        if (origin != NULL)
        {
            max_age = origin->getValue();
        }
    }
    else
    {
        max_age = parent.getParent().getAge();
    }
    
    // test that the max age is larger than the min age
    if ( max_age <= min_age )
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // store the old age of the parent
    storedAge = parent.getAge();
    
    // draw the new age for the parent node
    double new_age = (max_age-min_age) * rng->uniform01() + min_age;
    
    // set the age of the parent node equal to the new age
    parent.setAge( new_age );
    n.setSampledAncestor( false );
    
    // compute the Jacobian term
    double lnJacobian = log(max_age - min_age);
    
    return lnJacobian;
}



/**
 *
 */
void CollapseExpandFossilBranchProposal::prepareProposal( void )
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
void CollapseExpandFossilBranchProposal::printParameterSummary(std::ostream &o) const
{
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void CollapseExpandFossilBranchProposal::undoProposal( void )
{
    
    // undo the proposal (only if succeeded)
    if ( failed == false)
    {
        if ( storedNode->isSampledAncestor() == true )
        {
            storedNode->setSampledAncestor( false );
            storedNode->getParent().setAge( storedAge );
        }
        else
        {
            storedNode->setSampledAncestor( true );
            storedNode->getParent().setAge( storedAge );
        }
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void CollapseExpandFossilBranchProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if ( oldN == tau )
    {
        tau = static_cast<StochasticNode<Tree>* >(newN) ;
    }
    else if ( oldN == origin )
    {
        origin = static_cast<TypedDagNode<double>* >(newN);
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void CollapseExpandFossilBranchProposal::tune( double rate )
{
    
}

