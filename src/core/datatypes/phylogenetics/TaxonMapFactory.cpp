#include "TaxonMapFactory.h"
#include "TaxonMap.h"
#include "RbException.h"
#include "RbOptions.h"

using namespace RevBayesCore;

/** Default constructor */
TaxonMapFactory::TaxonMapFactory(void)
{
    
    map = new TaxonMap();
    map->incrementReferenceCount();
}


/** Destructor */
TaxonMapFactory::~TaxonMapFactory(void)
{
    
    delete map;
}


/** Delete a random number object (remove it from the pool too) */
void TaxonMapFactory::deleteTaxonMap(TaxonMap* m)
{
    
    allocated_taxon_maps.erase( m );
    
    delete m;
}
