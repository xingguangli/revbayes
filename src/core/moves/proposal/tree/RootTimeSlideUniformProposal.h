#ifndef RootTimeSlideUniformProposal_H
#define RootTimeSlideUniformProposal_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "Tree.h"

namespace RevBayesCore {
    
    /**
     * The node-age slide proposal operator using a Uniform distribution.
     *
     * This node-age proposal is a Uniform-sliding proposal on rooted subtrees without changing the topology.
     * That is, we pick a random node which is not the root.
     * Then, we pick an age between the parent and the oldest sampled descendant drawn from a Uniform distribution centered around the current age.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-07-12, version 1.0
     *
     */
    class RootTimeSlideUniformProposal : public Proposal {
        
    public:
        RootTimeSlideUniformProposal( StochasticNode<Tree> *n, StochasticNode<double> *o);                                               //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                        //!< Clean up proposal
        RootTimeSlideUniformProposal*           clone(void) const;                                          //!< Clone object
        double                                  doProposal(void);                                           //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                    prepareProposal(void);                                      //!< Prepare the proposal
        void                                    printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                    tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // parameters
        StochasticNode<Tree>*                   variable;                                                   //!< The variable the Proposal is working on
        StochasticNode<double>*                 origin;
        
        // stored objects to undo proposal
        double                                  storedAge;
        
    };
    
}

#endif

