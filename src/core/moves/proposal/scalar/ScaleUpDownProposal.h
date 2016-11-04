#ifndef ScaleUpDownProposal_H
#define ScaleUpDownProposal_H

#include <set>
#include <string>

#include "Proposal.h"
#include "ContinuousStochasticNode.h"

namespace RevBayesCore {
    
    /**
     * The sliding operator.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Michael Landis)
     * @since 2009-09-08, version 1.0
     *
     */
    class ScaleUpDownProposal : public Proposal {
        
    public:
        ScaleUpDownProposal( ContinuousStochasticNode *n1, ContinuousStochasticNode *n2, double l);                                                                      //!<  constructor
        
        // Basic utility functions
        void                                    cleanProposal(void);                                                                //!< Clean up proposal
        ScaleUpDownProposal*                    clone(void) const;                                                                  //!< Clone object
        double                                  doProposal(void);                                                                   //!< Perform proposal
        const std::string&                      getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                    printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                    prepareProposal(void);                                                              //!< Prepare the proposal
        void                                    tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                    undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        void                                    swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
    private:
        // parameters
        
        ContinuousStochasticNode*               variable1;                                                                           //!< The variable the Proposal is working on
        ContinuousStochasticNode*               variable2;
        double                                  storedValue1;                                                                        //!< The stored value of the Proposal used for rejections.
        double                                  storedValue2;                                                                        //!< The stored value of the Proposal used for rejections.
        double                                  lambda;                                                                             //!< The value we propose.
    };
    
}

#endif

