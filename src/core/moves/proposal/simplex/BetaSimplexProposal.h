#ifndef BetaSimplexProposal_H
#define BetaSimplexProposal_H

#include <set>
#include <string>

#include "SimpleProposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The beta-simplex operator.
     *
     * A beta-simplex proposal randomly changes a single value of a simplex, although the other values 
     * change too because of the renormalization.
     * First, a random index is drawn. Then, draws a random uniform number u ~ beta(a,b)
     * and sets the current vale to u.
     * The simplex is then renormalized.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class BetaSimplexProposal : public SimpleProposal<RbVector<double> > {
        
    public:
        BetaSimplexProposal( StochasticNode<RbVector<double> > *n, double a);                                                                    //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                                                //!< Clean up proposal
        BetaSimplexProposal*                    clone(void) const;                                                                  //!< Clone object
        double                                  propose(RbVector<double> &v);                                                                   //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                                              //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                    tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        RbVector<double>                        storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
        double                                  alpha;                                                                             //!< The scaling parameter of the Proposal
//        double                              proposedValue;                                                                      //!< The value we propose.
    };
    
}

#endif

