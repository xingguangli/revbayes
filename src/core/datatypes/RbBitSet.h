#ifndef RbRbBitSet_H
#define RbRbBitSet_H

#include <ostream>
#include <vector>

namespace RevBayesCore {
    
    /**
     * RevBayes class for bit sets.
     *
     * This class is essentially a wrapper of a std::vector<bool> with some additional functional.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2016-09-06, version 1.0
     */
    class RbBitSet {
        
    public:
        RbBitSet(void);                                                                                         //!< Constructor requires character type
        RbBitSet(size_t n);                                                                                         //!< Constructor requires character type
        virtual                        ~RbBitSet(void) {}
        
        bool                            operator[](size_t i) const;

        bool                            operator==(const RbBitSet &bs) const;
        bool                            operator!=(const RbBitSet &bs) const;
        bool                            operator<(const RbBitSet &bs) const;
        

        void                            clear(void);
        void                            flip(size_t i);
        size_t                          getNumberSetBits(void) const;                   //!< Get the number of bits set.
        bool                            isSet(size_t i) const;
        void                            set(size_t i);
        size_t                          size(void) const;
        void                            unset(size_t i);

        
    private:

        std::vector<bool>               value;
        size_t                          num_set_bits;

        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const RbBitSet& x);                    //!< Overloaded output operator
    
}

#endif
