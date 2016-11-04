#ifndef Clade_H
#define Clade_H

#include <vector>
#include <string>

#include "Taxon.h"

namespace RevBayesCore {
    
    /**
     * Object describing clades.
     *
     * A clade is simply a container of the taxon names.
     * Hence, this class just provides some convenience methods but could be considered as
     * a string-vector.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-03-10, version 1.0
     */
    class Clade  {
        
    public:
                                                    Clade(void);                                                //! Default constructor: empty clade of age 0.0
                                                    Clade(const Taxon &t);                                      //!< Default constructor with optional index
                                                    Clade(const std::vector<Taxon> &n);                         //!< Default constructor with optional index
        
        virtual                                    ~Clade() {}
        
        std::vector<Taxon>::const_iterator          begin(void) const;
        std::vector<Taxon>::iterator                begin(void);
        std::vector<Taxon>::const_iterator          end(void) const;
        std::vector<Taxon>::iterator                end(void);
        // overloaded operators
        bool                                        operator==(const Clade &t) const;
        bool                                        operator!=(const Clade &t) const;
        bool                                        operator<(const Clade &t) const;
        bool                                        operator<=(const Clade &t) const;
        bool                                        operator>(const Clade &t) const;
        bool                                        operator>=(const Clade &t) const;

        
        // Basic utility functions
        Clade*                                      clone(void) const;                                          //!< Clone object
        
        // public methods
        void                                        addTaxon(const Taxon &t);                                   //!< Add a taxon to our list.
        double                                      getAge(void) const;                                         //!< Get the age of this clade.
        int                                         getNumberMissingTaxa(void) const;                           //!< Get the number of missing taxa.
        std::vector<Taxon>&                         getTaxa(void);                                              //!< Get the taxon names.
        const std::vector<Taxon>&                   getTaxa(void) const;                                        //!< Get the taxon names.
        const Taxon&                                getTaxon(size_t i) const;                                   //!< Get a single taxon name.
        const std::string&                          getTaxonName(size_t i) const;                               //!< Get a single taxon name.
        void                                        setAge(double a);                                           //!< Set the age of the clade.
        void                                        setNumberMissingTaxa(int n);                                //!< Set the number of missing taxa in this clade.
        void                                        setTaxonAge(size_t i, double age);                          //!< Set a single taxon's age.
        size_t                                      size(void) const;                                           //!< Get the number of taxa.
        std::string                                 toString(void) const;                                       //!< Convert this value into a string.
        
        // public TopologyNode functions
        
    private: 
        
        // members
        double                                      age;
        int                                         num_missing;
        std::vector<Taxon>                          taxa;
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Clade& x);                                         //!< Overloaded output operator

    
}

#endif
