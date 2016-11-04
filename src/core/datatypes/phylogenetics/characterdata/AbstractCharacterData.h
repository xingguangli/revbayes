#ifndef AbstractCharacterData_H
#define AbstractCharacterData_H

#include "CharacterState.h"
#include "Cloneable.h"
#include "AbstractTaxonData.h"
#include "Serializable.h"
#include "Taxon.h"

#include <map>
#include <string>
#include <vector>

namespace RevBayesCore {

    /**
     * Abstract class for all character data objects.
     *
     * This abstract class provides the base class for all character data objects.
     * A character data object contains a vector of TaxonData objects and defines additional
     * convenience functions to access the data stored in these TaxonData objects.
     * Note that this class is a pure interface and thus contains only pure virtual functions!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    class AbstractCharacterData : public Cloneable, public Serializable {
    
    public:    
        virtual                                    ~AbstractCharacterData(void);
        
        // Overloaded operators
        virtual const AbstractTaxonData&            operator[](size_t i) const = 0;                                             //!< Subscript operator (const)
    
        virtual bool                                operator==(const AbstractCharacterData &rm) const { return this == &rm; }
        virtual bool                                operator!=(const AbstractCharacterData &rm) const { return !operator==(rm); }
        virtual bool                                operator<(const AbstractCharacterData &rm) const { return this < &rm; }
        virtual bool                                operator<=(const AbstractCharacterData &rm) const { return operator<(rm) || operator==(rm); }

        // methods of the Cloneable interface
        virtual AbstractCharacterData*              clone(void) const = 0;
        
        // methods of the Serializable interface
        virtual void                                initFromString( const std::string &s ) = 0;                                 //!< Serialize (resurrect) the object from a string value
        virtual void                                writeToFile(const std::string &dir, const std::string &fn) const = 0;

        // Container functions
        void                                        clear(void);
        void                                        addMissingTaxon(const std::string &n);                                      //!< Add taxon data
        void                                        addTaxonData(const AbstractTaxonData &obs);                                 //!< Add taxon data
        void                                        excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                        excludeTaxon(const std::string& s);                                         //!< Exclude taxon
        const std::string&                          getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        const std::string&                          getFilePath(void) const;                                                    //!< Returns the name of the file path
        size_t                                      getIndexOfTaxon(const std::string &n) const;                                //!< Get the index of the taxon with name 'n'.
        size_t                                      getNumberOfTaxa(void) const;                                                //!< Number of taxa
        size_t                                      getNumberOfIncludedTaxa(void) const;                                        //!< Number of included taxa
        double                                      getPercentageMissing(const std::string &n) const;                           //!< Returns the percentage of missing data for this sequence
        const std::vector<Taxon>&                   getTaxa(void) const;                                                        //!< Get the names of the taxa
        const Taxon&                                getTaxon(size_t idx) const;                                                 //!< Returns the i-th taxon
        AbstractTaxonData&                          getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const AbstractTaxonData&                    getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        AbstractTaxonData&                          getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const AbstractTaxonData&                    getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        const std::string&                          getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        std::string                                 getStateLabels(void);                                                       //!< Get the possible state labels
        std::string                                 getStateLabels(void) const;                                                 //!< Get the possible state labels
        void                                        includeTaxon(const std::string& s);                                         //!< Include taxon
        bool                                        isSequenceMissing(const std::string &n) const;                              //!< Returns whether the contains only missing data or has some actual observations
        bool                                        isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                        isTaxonExcluded(const std::string& s) const;                                //!< Is the taxon excluded
        void                                        restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                        restoreTaxon(const std::string& s);                                         //!< Restore taxon
        void                                        setFileName(const std::string &fn);                                         //!< Set the file name
        void                                        setFilePath(const std::string &fn);                                         //!< Set the file path
        void                                        setTaxonName(const std::string& currentName, const std::string& newName);   //!< Change the name of a taxon
        void                                        show(std::ostream &out) const;                                              //!< Show the entire content
        
        
        // CharacterData functions
//      virtual AbstractCharacterData&              concatenate(const AbstractCharacterData &d) = 0;                            //!< Concatenate two sequences
        virtual std::string                         getDataType(void) const = 0;                                                //!< Return the data type of this character data matrix
        virtual bool                                isHomologyEstablished(void) const = 0;                                      //!< Returns whether the homology of the characters has been established
        
    protected:
                                                    AbstractCharacterData(void);                                                //!< Constructor requires character type
                                                    AbstractCharacterData(const AbstractCharacterData &d);                      //!< Constructor requires character type

        AbstractCharacterData&                      operator=(const AbstractCharacterData &d);                                                                                      //!< Constructor requires character type


        // Utility functions
        size_t                                      indexOfTaxonWithName(const std::string& s) const;                           //!< Get the index of the taxon
        
        // Member variables
        std::set<size_t>                            deletedTaxa;                                                                //!< Set of deleted taxa
        std::string                                 fileName;                                                                   //!< The path/filename from where this matrix originated
        std::string                                 filePath;                                                                   //!< The path/filename from where this matrix originated
        std::vector<Taxon>                          taxa;                                                                       //!< names of the sequences
        std::map<std::string, AbstractTaxonData* >  taxonMap;
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const AbstractCharacterData& x);                    //!< Overloaded output operator
}

#endif
