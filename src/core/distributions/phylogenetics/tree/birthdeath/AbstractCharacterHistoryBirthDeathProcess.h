//
//  AbstractCharacterHistoryBirthDeathProcess.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/10/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#ifndef AbstractCharacterHistoryBirthDeathProcess_h
#define AbstractCharacterHistoryBirthDeathProcess_h


#include "CharacterHistory.h"
#include "MemberObject.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
    class AbstractCharacterHistoryBirthDeathProcess : public TypedDistribution<Tree> {
        
    public:
        AbstractCharacterHistoryBirthDeathProcess(void);                                             //!< Constructor
        
        virtual                                            ~AbstractCharacterHistoryBirthDeathProcess(void);                //!< Virtual destructor
        
        // public member functions
        AbstractCharacterHistoryBirthDeathProcess*          clone(void) const = 0;                                          //!< Create an independent clone
        virtual double                                      computeLnProbability(void) = 0;                                 //!< Compute ln prob of current value
        virtual CharacterHistory&                           getCharacterHistory(void) = 0;                                      //!< Get the character histories
        virtual CharacterHistory                            getCharacterHistory(void) const = 0;                                      //!< Get the character histories
        virtual void                                        redrawValue(void) = 0;                                          //!< Draw a new random value from distribution
        virtual void                                        setValue(Tree *v, bool force) = 0;
        
    protected:
        // Parameter management functions
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter) = 0;                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter) = 0;
        virtual void                                        restoreSpecialization(DagNode *restorer) = 0;
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll) = 0;
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP) = 0;            //!< Swap a
        
    private:
        
    };
    
}


#endif /* AbstractCharacterHistoryBirthDeathProcess_h */
