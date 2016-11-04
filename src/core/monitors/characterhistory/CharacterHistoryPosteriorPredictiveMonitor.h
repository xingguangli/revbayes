//
//  CharacterHistoryPosteriorPredictiveMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 12/23/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__CharacterHistoryPosteriorPredictiveMonitor__
#define __rb_mlandis__CharacterHistoryPosteriorPredictiveMonitor__


#include "Monitor.h"
#include "BranchHistory.h"
#include "StochasticNode.h"
#include "TypedDagNode.h"
#include "Tree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class CharacterHistoryPosteriorPredictiveMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        CharacterHistoryPosteriorPredictiveMonitor(TypedDagNode<Tree> *t, std::vector< StochasticNode< BranchHistory >* > bh, unsigned long g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool sm=true, bool sr=true);
        
        // new CharacterHistoryPosteriorPredictiveMonitor( tau, bh_vector_stochastic, 10, filepath + "rb.tree_chars.txt", "\t"));
        
        CharacterHistoryPosteriorPredictiveMonitor(const CharacterHistoryPosteriorPredictiveMonitor& f);
        
        // basic methods
        CharacterHistoryPosteriorPredictiveMonitor*          clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(unsigned long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        std::string                         buildExtendedNewick();
        std::string                         buildExtendedNewick(TopologyNode* n);
        std::string                         buildCharacterHistoryString(TopologyNode* n, std::string brEnd="child");
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode<Tree>*                 tree;
        std::vector<StochasticNode<BranchHistory>* > branchHistories;
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        bool                                showMetadata;
        bool                                showRates;
        
    };
    
}


#endif /* defined(__rb_mlandis__CharacterHistoryPosteriorPredictiveMonitor__) */
