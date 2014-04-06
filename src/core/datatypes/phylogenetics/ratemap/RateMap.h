//
//  RateMap.h
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__RateMap__
#define __rb_mlandis__RateMap__

#include "CharacterEvent.h"
#include "CharacterState.h"
#include "Cloneable.h"
#include <vector>

namespace RevBayesCore {
    
    class RateMap : public Cloneable {
        
    public:
        virtual                            ~RateMap(void);                                                                 //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                      //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                  //!< Return the number of characters

        // pure virtual methods to overwrite
        virtual RateMap*                    clone(void) const = 0;
        virtual double                      getRate(CharacterState* from, CharacterState* to) const = 0;
        virtual double                      getSumOfRates(CharacterState* from) const = 0;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to) const = 0;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from) const = 0;

        virtual double                      getTransitionProbability(CharacterState* from, CharacterState* to, double t) const = 0;
        virtual double                      getLnTransitionProbability(CharacterState* from, CharacterState* to, double t) const = 0;
        virtual double                      getLnTransitionProbability(std::vector<CharacterEvent*> from, CharacterEvent* to, double t) const = 0;
        virtual void                        updateMap(void) = 0;
        
    protected:
        // prevent instantiation
        RateMap(const RateMap& m);                                                              //!< Copy constructor
        RateMap(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        RateMap&                            operator=(const RateMap& r);
        
        // protected members available for derived classes
        size_t                              numStates;                                          //!< The number of character states
        size_t                              numCharacters;                                      //!< The number of characters
        bool                                needsUpdate;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMap& x);                                           //!< Overloaded output operator
    
}

#endif /* defined(__rb_mlandis__RateMap__) */