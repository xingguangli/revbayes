#ifndef TaxonMapFactory_H
#define TaxonMapFactory_H

#include <set>
#include <vector>

namespace RevBayesCore {
    
#define GLOBAL_TAXON_MAP TaxonMapFactory::taxonMapFactoryInstance().getGlobalTaxonMap()
    
    class TaxonMap;
    class TaxonMapFactory {
        
    public:
        static TaxonMapFactory&                 taxonMapFactoryInstance(void)                                                      //!< Return a reference to the singleton factory
        {
            static TaxonMapFactory single_taxon_map_factory;
            return single_taxon_map_factory;
        }
        void                                    deleteTaxonMap(TaxonMap* r);                                 //!< Return a random number object to the pool
        TaxonMap*                               getGlobalTaxonMap(void) { return map; }                         //!< Return a pointer to the global random number object
        
    private:
        TaxonMapFactory(void);                                                              //!< Default constructor
        TaxonMapFactory(const TaxonMapFactory&);                                            //!< Copy constructor
        TaxonMapFactory& operator=(const TaxonMapFactory&);                                 //!< Assignment operator
        ~TaxonMapFactory(void);                                                             //!< Destructor
        TaxonMap*                               map;                                                                         //!< A random number object that generates seeds
        std::set<TaxonMap*>                     allocated_taxon_maps;                                                                //!< The pool of random number objects
    };
}

#endif


