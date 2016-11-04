#include "VectorSingleElementScaleProposal.h"
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
VectorSingleElementScaleProposal::VectorSingleElementScaleProposal( StochasticNode< RbVector<double> > *n, double l) : Proposal(),
    variable( n ),
    lambda( l ),
    index( 0 ),
    storedValue( 0.0 )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void VectorSingleElementScaleProposal::cleanProposal( void )
{
    variable->clearTouchedElementIndices();
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
VectorSingleElementScaleProposal* VectorSingleElementScaleProposal::clone( void ) const
{
    
    return new VectorSingleElementScaleProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& VectorSingleElementScaleProposal::getProposalName( void ) const
{
    static std::string name = "ElementScaling";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and scales the current vale by a scaling factor
 * sf = exp( lambda * u )
 * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double VectorSingleElementScaleProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    RbVector<double> &val = variable->getValue();

    // choose an index
    index = size_t(rng->uniform01() * val.size());
    
    // copy value
    storedValue = val[index];
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scaling_factor = std::exp( lambda * ( u - 0.5 ) );
    val[index] *= scaling_factor;
    
    variable->addTouchedElementIndex(index);
    
    // compute the Hastings ratio
    double lnHastingsratio = log( scaling_factor );
    
    return lnHastingsratio;
}


/**
 * Prepare the proposal, e.g., pick the element that we want to change.
 * Here we do not need to do any preparation.
 */
void VectorSingleElementScaleProposal::prepareProposal( void )
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
void VectorSingleElementScaleProposal::printParameterSummary(std::ostream &o) const
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
void VectorSingleElementScaleProposal::undoProposal( void )
{
    std::vector<double>& v = variable->getValue();
    v[index] = storedValue;
    variable->clearTouchedElementIndices();
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void VectorSingleElementScaleProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode< RbVector<double> >* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void VectorSingleElementScaleProposal::tune( double rate )
{
    
    if ( rate > 0.23 )
    {
        lambda *= (1.0 + ((rate-0.23)/0.77) );
    }
    else
    {
        lambda /= (2.0 - rate/0.23 );
    }
    
}

