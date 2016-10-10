#ifndef TaxonMap_H
#define TaxonMap_H

#include "Taxon.h"

#include <string>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    class AbstractCharacterData;
    class Tree;
    
    
    /**
     * @brief TaxonMap object representing an individual.
     *
     * This class represents a TaxonMap.
     * A TaxonMap simply consists of a TaxonMap name and a date when this TaxonMap has been observed.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-18, version 1.0
     */
    class TaxonMap {
        
    public:
        
        TaxonMap(void);                                //!< Default constructor
        TaxonMap(const Tree &t);                                                            //!< Constructor from tree object
        TaxonMap(const TaxonMap &tm);                                                            //!< Constructor from tree object
        virtual                             ~TaxonMap() {}

        TaxonMap                              operator=(const TaxonMap &t);                 //!< assingment operators

//        bool                                operator==(const TaxonMap &t) const;           //!< Equals operators
//        bool                                operator!=(const TaxonMap &t) const;           //!< Not-quals operators
//        bool                                operator<(const TaxonMap &t) const;            //!< Less-than operators
//        bool                                operator<=(const TaxonMap &t) const;           //!< Less-than operators
//        bool                                operator>(const TaxonMap &t) const;            //!< Less-than operators
//        bool                                operator>=(const TaxonMap &t) const;           //!< Less-than operators
        
        // public methods
        size_t                              addTaxon(const Taxon &t);                       //!< Get the age for this TaxonMap.
        void                                addTaxa(const std::vector<Taxon> &t);           //!< Get the age for this TaxonMap.
        void                                addTaxa(const AbstractCharacterData &d);        //!< Get the age for this TaxonMap.
        void                                addTaxa(const Tree &t);                         //!< Get the age for this TaxonMap.
        size_t                              decrementReferenceCount(void) const;                                                        //!< Decrement the reference count for reference counting in smart pointers
        size_t                              getReferenceCount(void) const;                                                              //!< Get the reference count for reference counting in smart pointers
        Taxon&                              getTaxon(size_t i);                             //!< Get the i-th Taxon
        const Taxon&                        getTaxon(size_t i) const;                       //!< Get the i-th Taxon
        size_t                              getTaxonIndex(const Taxon &t) const;            //!< Get the i-th Taxon
        bool                                hasTaxon(const Taxon &t) const;            //!< Get the i-th Taxon
        void                                incrementReferenceCount(void) const;                                                        //!< Increment the reference count for reference counting in smart pointers
        size_t                              size(void) const;                               //!< Get the i-th Taxon
        
    private:
        
        std::vector<Taxon>                  taxa;
        std::map<Taxon, size_t>             taxa_map;

        mutable size_t                                              ref_count;

    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const TaxonMap& x);                                         //!< Overloaded output operator
}

#endif
