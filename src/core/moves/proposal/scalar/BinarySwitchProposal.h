#ifndef BinarySwitchProposal_H
#define BinarySwitchProposal_H

#include "Proposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The random-binary-walk operator.
     *
     * This is a very simple move on 0-1 numbers that proposes to change 
     * the value of a binary variable. 
     * Thus, it is a random walk but guided by the acceptance ratio.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Bastien Boussau)
     * @since 2009-09-08, version 1.0
     *
     */
    class BinarySwitchProposal : public Proposal {
        
    public:
        BinarySwitchProposal( StochasticNode<int> *n);                                                                    //!<  constructor
        
        // Basic utility functions
        void                                cleanProposal(void);                                                                //!< Clean up proposal
        BinarySwitchProposal*          clone(void) const;                                                                  //!< Clone object
        double                              doProposal(void);                                                                   //!< Perform proposal
        const std::string&                  getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                prepareProposal(void);                                                              //!< Prepare the proposal
        void                                printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        StochasticNode<int>*                variable;                                                                           //!< The variable the Proposal is working on
        int                                 storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
    };
    
}

#endif

