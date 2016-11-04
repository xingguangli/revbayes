#include "DagNode.h"
#include "MetropolisHastingsMove.h"
#include "Proposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathLogic.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace RevBayesCore;


/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
MetropolisHastingsMove::MetropolisHastingsMove( Proposal *p, double w, bool t ) : AbstractMove(p->getNodes(), w, t),
    numAccepted( 0 ),
    proposal( p )
{
    
    proposal->setMove( this );
    
}


/**
 * Copy constructor.
 * We need to create a deep copy of the proposal here.
 *
 * \param[in]   m   The object to copy.
 *
 */
MetropolisHastingsMove::MetropolisHastingsMove(const MetropolisHastingsMove &m) : AbstractMove(m),
    numAccepted( m.numAccepted ),
    proposal( m.proposal->clone() )
{
    
    proposal->setMove( this );
    
}


/**
 * Basic destructor doing nothing.
 */
MetropolisHastingsMove::~MetropolisHastingsMove( void )
{
    
    delete proposal;
}


/** 
 * Overloaded assignment operator.
 * We need a deep copy of the operator.
 */
MetropolisHastingsMove& MetropolisHastingsMove::operator=(const RevBayesCore::MetropolisHastingsMove &m)
{
    
    if ( this != &m )
    {
        // delegate
        AbstractMove::operator=( m );
        
        // free memory
        delete proposal;
        
        numAccepted     = m.numAccepted;
        proposal        = m.proposal->clone();
        
        proposal->setMove( this );
        
    }
    
    return *this;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the MetropolisHastingsMove. 
 */
MetropolisHastingsMove* MetropolisHastingsMove::clone( void ) const 
{
    
    return new MetropolisHastingsMove( *this );
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& MetropolisHastingsMove::getMoveName( void ) const 
{
    
    return proposal->getProposalName();
}


/**
 * How often was the move accepted
 */
size_t MetropolisHastingsMove::getNumberAccepted( void ) const
{
    
    return numAccepted;
}


/**
 * Get the proposal of the move
 *
 * \return The proposal object.
 */
Proposal& MetropolisHastingsMove::getProposal( void )
{
    
    return *proposal;
}


void MetropolisHastingsMove::performHillClimbingMove( double lHeat, double pHeat )
{
    
    // Propose a new value
    proposal->prepareProposal();
    double ln_hastings_ratio = proposal->doProposal();
    
    
    const RbOrderedSet<DagNode*> &affectedNodes = getAffectedNodes();
    const std::vector<DagNode*> nodes = getDagNodes();
    
    // first we touch all the nodes
    // that will set the flags for recomputation
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* the_node = nodes[i];
        the_node->touch();
    }
    
    double lnPriorRatio = 0.0;
    double lnLikelihoodRatio = 0.0;
    
    
    // compute the probability of the current value for each node
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* the_node = nodes[i];
        
        if ( RbMath::isAComputableNumber(lnPriorRatio) && RbMath::isAComputableNumber(lnLikelihoodRatio) && RbMath::isAComputableNumber(ln_hastings_ratio) )
        {
            if ( the_node->isClamped() )
            {
                lnLikelihoodRatio += the_node->getLnProbabilityRatio();
            }
            else
            {
                lnPriorRatio += the_node->getLnProbabilityRatio();
            }
            
        }
        
    }
    
    // then we recompute the probability for all the affected nodes
    for (RbOrderedSet<DagNode*>::const_iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it)
    {
        DagNode *the_node = *it;
        
        if ( RbMath::isAComputableNumber(lnPriorRatio) && RbMath::isAComputableNumber(lnLikelihoodRatio) && RbMath::isAComputableNumber(ln_hastings_ratio) )
        {
            if ( the_node->isClamped() )
            {
                lnLikelihoodRatio += the_node->getLnProbabilityRatio();
            }
            else
            {
                lnPriorRatio += the_node->getLnProbabilityRatio();
            }
        }
        
    }
    
    // exponentiate with the chain heat
    double ln_posterior_ratio = pHeat * (lHeat * lnLikelihoodRatio + lnPriorRatio);
    
    if ( RbMath::isAComputableNumber(ln_posterior_ratio) == false || ln_posterior_ratio < 0.0 )
    {
        //        std::cerr << "Reject.\n";
        
        
        proposal->undoProposal();
        
        // call restore for each node
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            // get the pointer to the current node
            DagNode* the_node = nodes[i];
            the_node->restore();
        }
    }
    else
    {
        
        numAccepted++;
            
        // call accept for each node
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            // get the pointer to the current node
            DagNode* the_node = nodes[i];
            the_node->keep();
        }
        
    }
    
}




void MetropolisHastingsMove::performMcmcMove( double lHeat, double pHeat )
{
    
    // Propose a new value
    proposal->prepareProposal();
    double ln_hastings_ratio = proposal->doProposal();
    
//    if ( fabs(ln_hastings_ratio) > 100.0 )
//    {
//        std::cerr << proposal->getProposalName() << ":\t\t" << ln_hastings_ratio << std::endl;
//    }
    
    
    const RbOrderedSet<DagNode*> &affectedNodes = getAffectedNodes();
    const std::vector<DagNode*> nodes = getDagNodes();
    
    // first we touch all the nodes
    // that will set the flags for recomputation
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* the_node = nodes[i];
        the_node->touch();
    }
    
    double lnPriorRatio = 0.0;
    double lnLikelihoodRatio = 0.0;

    
    // compute the probability of the current value for each node
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // get the pointer to the current node
        DagNode* the_node = nodes[i];
        
        if ( RbMath::isAComputableNumber(lnPriorRatio) && RbMath::isAComputableNumber(lnLikelihoodRatio) && RbMath::isAComputableNumber(ln_hastings_ratio) )
        {
            if ( the_node->isClamped() )
            {
                lnLikelihoodRatio += the_node->getLnProbabilityRatio();
            }
            else
            {
                lnPriorRatio += the_node->getLnProbabilityRatio();
            }
            
        }
        
    }
    
    // then we recompute the probability for all the affected nodes
    for (RbOrderedSet<DagNode*>::const_iterator it = affectedNodes.begin(); it != affectedNodes.end(); ++it)
    {
        DagNode *the_node = *it;

        if ( RbMath::isAComputableNumber(lnPriorRatio) && RbMath::isAComputableNumber(lnLikelihoodRatio) && RbMath::isAComputableNumber(ln_hastings_ratio) )
        {
            if ( the_node->isClamped() )
            {
                lnLikelihoodRatio += the_node->getLnProbabilityRatio();
            }
            else
            {
                lnPriorRatio += the_node->getLnProbabilityRatio();
            }
        }

    }
    
    // exponentiate with the chain heat
    double ln_posterior_ratio;
    ln_posterior_ratio = pHeat * (lHeat * lnLikelihoodRatio + lnPriorRatio);
	
	if ( RbMath::isAComputableNumber(ln_posterior_ratio) == false )
    {
        
        proposal->undoProposal();
            
        // call restore for each node
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            // get the pointer to the current node
            DagNode* the_node = nodes[i];
            the_node->restore();
        }
	}
    else
    {
    
        // finally add the Hastings ratio
        double ln_acceptance_ratio = ln_posterior_ratio + ln_hastings_ratio;

        if (ln_acceptance_ratio >= 0.0)
        {

            
            numAccepted++;
        
            // call accept for each node
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                // get the pointer to the current node
                DagNode* the_node = nodes[i];
                the_node->keep();
            }
        
        }
        else if (ln_acceptance_ratio < -300.0)
        {
            
            proposal->undoProposal();
        
            // call restore for each node
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                // get the pointer to the current node
                DagNode* the_node = nodes[i];
                the_node->restore();
            }
        }
        else
        {
            double r = exp(ln_acceptance_ratio);
            // Accept or reject the move
            double u = GLOBAL_RNG->uniform01();
            if (u < r)
            {
                
                numAccepted++;
            
                // call accept for each node
                for (size_t i = 0; i < nodes.size(); ++i)
                {
                    // get the pointer to the current node
                    DagNode* the_node = nodes[i];
                    the_node->keep();
                }
            
                proposal->cleanProposal();
            }
            else
            {
                
                proposal->undoProposal();
            
                // call restore for each node
                for (size_t i = 0; i < nodes.size(); ++i)
                {
                    // get the pointer to the current node
                    DagNode* the_node = nodes[i];
                    the_node->restore();
                }
                
            }
            
        }

    }

}


/**
 * Print the summary of the move.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void MetropolisHastingsMove::printSummary(std::ostream &o) const 
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i)
    {
        o << " ";
    }
    o << " ";
    
    // print the DagNode name
    const std::string &dn_name = (*nodes.begin())->getName();
    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
    o << dn_name;
    for (size_t i = 0; i < spaces; ++i)
    {
        o << " ";
    }
    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i)
    {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i)
    {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted);
    
    for (int i = 0; i < a_length; ++i)
    {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i)
    {
        o << " ";
    }
    o << ratio;
    o << " ";
    
    proposal->printParameterSummary( o );
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}

/**
 * Reset the move counters. Here we only reset the counter for the number of accepted moves.
 *
 */
void MetropolisHastingsMove::resetMoveCounters( void )
{
    numAccepted = 0;
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new variable.
 */
void MetropolisHastingsMove::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    proposal->swapNode(oldN, newN);
    
}


/**
 * Tune the move to accept the desired acceptance ratio.
 * We only compute the acceptance ratio here and delegate the call to the proposal.
 */
void MetropolisHastingsMove::tune( void )
{
    
    if ( numTried > 2 )
    {
        double rate = numAccepted / double(numTried);
    
        proposal->tune( rate );
    }
    
}

