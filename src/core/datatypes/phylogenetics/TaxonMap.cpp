#include "AbstractCharacterData.h"
#include "RbException.h"
#include "TaxonMap.h"
#include "Tree.h"

using namespace RevBayesCore;


/**
 * Default constructor.
 */
TaxonMap::TaxonMap( void ) :
    taxa(),
    ref_count( 0 )
{
    
}

/**
 * Constructor from tree object.
 */
TaxonMap::TaxonMap( const Tree &t ) :
    taxa(),
    ref_count( 0 )
{
    
    addTaxa( t );
    
}


/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
void TaxonMap::addTaxa(const std::vector<Taxon> &t)
{
    
    for (size_t i=0; i<t.size(); ++i)
    {
        addTaxon( t[i] );
    }
    
}


/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
void TaxonMap::addTaxa(const AbstractCharacterData &d)
{
    
    std::vector<Taxon> taxa = d.getTaxa();
    for (size_t i=0; i<taxa.size(); ++i)
    {
        addTaxon( taxa[i] );
    }
    
}


/**
  * Get the i-th taxon.
  *
  * \return    The taxon.
  */
void TaxonMap::addTaxa(const Tree &t)
{
    
    std::vector<Taxon> tree_taxa = t.getTaxa();
    for (size_t i=0; i<tree_taxa.size(); ++i)
    {
        addTaxon( tree_taxa[i] );
    }
    
}


/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
size_t TaxonMap::addTaxon( const Taxon &t )
{
    
    // check for empty taxa
    if ( t.getName() == "" )
    {
        return RbConstants::Size_t::nan;
    }
        
    const std::map< Taxon, size_t >::const_iterator& entry = taxa_map.find(t);
    if ( entry == taxa_map.end() )
    {
        size_t index = taxa.size();
        taxa_map.insert( std::pair<Taxon, size_t>( t, index ) );
        taxa.push_back( t );
        return index;
    }
    else
    {
        return entry->second;
    }
    
}


/**
 * Decrement the reference count and return it.
 */
size_t TaxonMap::decrementReferenceCount( void ) const
{
    
    --ref_count;
    
    return ref_count;
}



/**
 * Get the reference count.
 */
size_t TaxonMap::getReferenceCount( void ) const
{
    
    return ref_count;
}

/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
Taxon& TaxonMap::getTaxon(size_t i)
{
    if ( i >= taxa.size() )
    {
        throw RbException("Cannot get a taxon for this index.");
    }
    
    return taxa[i];
}

/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
const Taxon& TaxonMap::getTaxon(size_t i) const
{
    return taxa[i];
}


size_t TaxonMap::getTaxonIndex(const Taxon &t) const
{
    
    // check if the name makes sense
    if ( t.getName() == "" )
    {
        return RbConstants::Size_t::nan;
    }
    
    const std::map< Taxon, size_t >::const_iterator& entry = taxa_map.find(t);
    
    if ( entry == taxa_map.end() )
    {
//        throw RbException("Couldn't find a clade with name '" + t.getName() + "'.");
        return const_cast<TaxonMap*>(this)->addTaxon( t );
    }
    
    return entry->second;
}
/**
 * Get the i-th taxon.
 *
 * \return    The taxon.
 */
bool TaxonMap::hasTaxon(const Taxon &t) const
{
    const std::map< Taxon, size_t >::const_iterator& entry = taxa_map.find(t);
    
    return entry != taxa_map.end();
}


/**
 * Increment the reference count.
 */
void TaxonMap::incrementReferenceCount( void ) const
{
    
    ++ref_count;
    
}


/**
 * Get the size, i.e., the number of taxa in this map.
 *
 * \return    The size.
 */
size_t TaxonMap::size( void ) const
{
    return taxa.size();
}
