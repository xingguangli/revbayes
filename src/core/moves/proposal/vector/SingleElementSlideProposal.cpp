#include "SingleElementSlideProposal.h"
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
SingleElementSlideProposal::SingleElementSlideProposal( std::vector<StochasticNode<double> *> n, double l) : Proposal(),
variables( n ),
lambda( l ),
index( 0 ),
storedValue( 0.0 )
{
    // tell the base class to add the node
    for (std::vector< StochasticNode<double> *>::const_iterator it = variables.begin(); it != variables.end(); it++)
    {
        addNode( *it );
    }
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void SingleElementSlideProposal::cleanProposal( void )
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
SingleElementSlideProposal* SingleElementSlideProposal::clone( void ) const
{
    
    return new SingleElementSlideProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& SingleElementSlideProposal::getProposalName( void ) const
{
    static std::string name = "ElementSlide";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and Slides the current vale by a scaling factor
 * sf = exp( lambda * u )
 * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double SingleElementSlideProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // choose an index
    index = size_t(rng->uniform01() * variables.size());
    
    double &val = variables[index]->getValue();
    
    // copy value
    storedValue = val;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double delta  = ( lambda * ( u - 0.5 ) );
    val += delta;
    
    return 0.0;
}


/**
 * Prepare the proposal, e.g., pick the element that we want to change.
 * Here we do not need to do any preparation.
 */
void SingleElementSlideProposal::prepareProposal( void )
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
void SingleElementSlideProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "lambda = " << lambda;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void SingleElementSlideProposal::undoProposal( void )
{
    double &v = variables[index]->getValue();
    v = storedValue;
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void SingleElementSlideProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    for (size_t i = 0; i < variables.size(); ++i)
    {
        if ( variables[i] == oldN )
        {
            variables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void SingleElementSlideProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
    
}

