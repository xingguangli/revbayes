#ifndef Move_BetaProbability_H
#define Move_BetaProbability_H

#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Rev Wrapper of a beta-Probability move on a single elements of Probability.
     *
     * This class is the RevLanguage wrapper of ElementScale.
     *
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @copyright GPL version 3
     * @since 2015-05-21, version 1.0
     */
    class Move_BetaProbability : public Move {
        
    public:
        
        Move_BetaProbability(void);                                                                                                             //!< Default constructor
        
        // Basic utility functions
        virtual Move_BetaProbability*               clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        std::vector<std::string>                    getMoveAliases(void) const;                                                             //!< Get the alternative names used for the constructor function in Rev.
        std::string                                 getMoveName(void) const;                                                                //!< Get the name used for the constructor function in Rev.
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);       //!< Set member variable
        
        RevPtr<const RevVariable>                   x;
        RevPtr<const RevVariable>                   delta;
        RevPtr<const RevVariable>                   offset;
        RevPtr<const RevVariable>                   tune;
        
    };
    
}

#endif
