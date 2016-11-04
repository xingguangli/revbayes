#include "BinarySwitchProposal.h"
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
BinarySwitchProposal::BinarySwitchProposal( StochasticNode<int> *n) : Proposal(),
    variable( n ),
    storedValue( 0 )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void BinarySwitchProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
BinarySwitchProposal* BinarySwitchProposal::clone( void ) const
{
    
    return new BinarySwitchProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& BinarySwitchProposal::getProposalName( void ) const
{
    static std::string name = "BinarySwitch";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A switching Proposal between the values 0 and 1.
 *
 * \return The hastings ratio.
 */
double BinarySwitchProposal::doProposal( void )
{
    
    int &val = variable->getValue();
    
    // copy value
    storedValue = val;
    
    // Generate new value
    if ( val == 1 )
    {
        val = 0;
    }
    else
    {
        val = 1;
    }
    
    return 0.0;
}


/**
 *
 */
void BinarySwitchProposal::prepareProposal( void )
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
void BinarySwitchProposal::printParameterSummary(std::ostream &o) const
{
    
    // no tuning parameter
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void BinarySwitchProposal::undoProposal( void )
{
    // swap current value and stored value
    variable->setValue( new int(storedValue) );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void BinarySwitchProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<int>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * There is no tuning parameter here.
 */
void BinarySwitchProposal::tune( double rate )
{
    
    // no tuning parameter
    
}

