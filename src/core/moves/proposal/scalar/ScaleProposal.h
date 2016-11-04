#ifndef ScaleProposal_H
#define ScaleProposal_H

#include <set>
#include <string>

#include "SimpleProposal.h"
#include "StochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The scaling operator. 
     *
     * A scaling proposal draws a random uniform number u ~ unif(-0.5,0.5)
     * and scales the current vale by a scaling factor
     * sf = exp( lambda * u )
     * where lambda is the tuning parameter of the Proposal to influence the size of the proposals.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2009-09-08, version 1.0
     *
     */
    class ScaleProposal : public SimpleProposal<double> {
        
    public:
        ScaleProposal( StochasticNode<double> *n, double l);                                                                    //!<  constructor
        
        // Basic utility functions
        void                                cleanProposal(void);                                                                //!< Clean up proposal
        ScaleProposal*                      clone(void) const;                                                                  //!< Clone object
        const std::string&                  getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                prepareProposal(void);                                                              //!< Prepare the proposal
        void                                printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        double                              propose(double &v);                                                                   //!< Perform proposal
        void                                tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        
        void                                swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        // parameters
        
        double                              storedValue;                                                                        //!< The stored value of the Proposal used for rejections.
        double                              lambda;                                                                             //!< The scaling parameter of the Proposal

    };
    
}

#endif

