//
//  ChromosomesCladogenicBirthDeathFunction.h
//
//  Created by Will Freyman on 6/22/16.
//

#ifndef ChromosomesCladogenicBirthDeathFunction__
#define ChromosomesCladogenicBirthDeathFunction__

#include "AbstractCladogenicStateFunction.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"

#include <sstream>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class ChromosomesCladogenicBirthDeathFunction : public AbstractCladogenicStateFunction, public TypedFunction<MatrixReal> {
        
    public:
        
        ChromosomesCladogenicBirthDeathFunction( const TypedDagNode< RbVector< double > >* sr, unsigned mc );
        virtual                                            ~ChromosomesCladogenicBirthDeathFunction(void);
        
        const static unsigned NO_CHANGE                     = 0;         // N -> N
        const static unsigned FISSION                       = 1;         // N -> N + 1
        const static unsigned FUSION                        = 2;         // N -> N - 1
        const static unsigned POLYPLOIDIZATION              = 3;         // N -> N * 2
        const static unsigned DEMIPOLYPLOIDIZATION          = 4;         // N -> N * 1.5
        
        // public member functions
        ChromosomesCladogenicBirthDeathFunction*            clone(void) const;
        const std::map< std::vector<unsigned>, double >&    getEventMap(void) const;
        void                                                update(void);
        
    protected:
        
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);
        
    private:
        
        void                                                buildEventMap(void);
        void                                                updateSpeciationRates(void);
        
        // members
        const TypedDagNode< RbVector<double> >*             speciationRates;
        unsigned                                            maxChromo;
        unsigned                                            numEventTypes;
        std::map< std::vector<unsigned>, unsigned >         eventMapTypes;
        std::map< std::vector<unsigned>, double >           eventMap;
        std::vector< std::vector<unsigned> >                eventMapCounts;
        
    };
    
}

#endif