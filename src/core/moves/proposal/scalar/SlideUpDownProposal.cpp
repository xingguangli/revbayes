#include "DistributionNormal.h"
#include "SlideUpDownProposal.h"
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
SlideUpDownProposal::SlideUpDownProposal( ContinuousStochasticNode *n1, ContinuousStochasticNode *n2, double l) : Proposal(),
variable1( n1 ),
variable2( n2 ),
storedValue1( 0.0 ),
storedValue2( 0.0 ),
lambda( l )
{
    // tell the base class to add the node
    addNode( variable1 );
    addNode( variable2 );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void SlideUpDownProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
SlideUpDownProposal* SlideUpDownProposal::clone( void ) const
{
    
    return new SlideUpDownProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& SlideUpDownProposal::getProposalName( void ) const
{
    static std::string name = "SlideUpDown";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A sliding proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and SlideUpDowns the current vale by
 * delta = lambda * u
 * where lambda is the tuning parameter of the proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double SlideUpDownProposal::doProposal( void )
{
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double sv1 = variable1->getValue();
    double sv2 = variable2->getValue();
    
    storedValue1 = sv1;
    storedValue2 = sv2;
    
    double min1 = variable1->getMin();
    double min2 = variable2->getMin();
    
    double max1 = variable1->getMax();
    double max2 = variable2->getMax();
    
    double size = max1 - min1;
    
    double u      = rng->uniform01();
    double delta  = ( lambda * ( u - 0.5 ) );
    
    if ( fabs(delta) > 2.0*size )
    {
        delta -= floor(delta / (2.0*size)) * (2.0*size);
    }
    double nv1 = sv1 + delta;
    double nv2 = sv2 - delta;
    
    /* reflect the new value */
    do {
        if ( nv1 < min1 )
        {
            nv1 = 2.0 * min1 - nv1;
            nv2 = sv2 - (nv1 - sv1);
        }
        else if ( nv1 > max1 )
        {
            nv1 = 2.0 * max1 - nv1;
            nv2 = sv2 - (nv1 - sv1);
        }
        
    } while ( nv1 < min1 || nv1 > max1 || nv2 < min2 || nv2 > max2);
    
    variable1->setValue( new double(nv1) );
    variable2->setValue( new double(nv2) );
    
    return 0.0;
}


/**
 *
 */
void SlideUpDownProposal::prepareProposal( void )
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
void SlideUpDownProposal::printParameterSummary(std::ostream &o) const
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
void SlideUpDownProposal::undoProposal( void )
{
    // swap current value and stored value
    variable1->setValue( new double(storedValue1) );
    variable2->setValue( new double(storedValue2) );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void SlideUpDownProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if(oldN == variable1)
    {
        variable1 = static_cast<ContinuousStochasticNode* >( newN );
    }
    
    if(oldN == variable2)
    {
        variable2 = static_cast<ContinuousStochasticNode* >( newN );
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void SlideUpDownProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
    
    lambda = fmin(10000, lambda);
    
}

