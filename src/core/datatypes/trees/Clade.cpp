#include "Clade.h"
#include "RbException.h"
#include "RbVectorUtilities.h"
#include "TaxonMapFactory.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace RevBayesCore;


/**
 * Default constructor required by the revlanguage code.
 * We use an empty string and an age of 0.0 for this default object.
 */
Clade::Clade( void ) :
    age( 0.0 ),
    num_missing( 0 )
{
    
}


/**
  * Constructor with a single taxon.
  */
Clade::Clade( const RbBitSet &b) :
    age( 0.0 ),
    num_missing( 0 ),
    bitset( b )
{
    
}


/**
 * Constructor with a single taxon.
 */
Clade::Clade( const Taxon &t ) :
    age( 0.0 ),
    num_missing( 0 ),
    bitset( GLOBAL_TAXON_MAP->size() )
{
    
    bitset.set( GLOBAL_TAXON_MAP->getTaxonIndex( t ) );
    
}


/**
 * Default constructor that instantiates the object.
 * Additionally, we sort the vector of taxon names.
 *
 * \param[in]   n    The vector containing the taxon names.
 */
Clade::Clade(const std::vector<Taxon> &n ) :
    age( 0.0 ),
    num_missing( 0 ),
    bitset( GLOBAL_TAXON_MAP->size() )
{
    
    RbPtr<TaxonMap> tm = GLOBAL_TAXON_MAP;
    // for identifiability we always keep the taxon names sorted
//    std::sort(taxa.begin(), taxa.end());
    for (size_t i=0; i<n.size(); ++i)
    {
        bitset.set( tm->getTaxonIndex( n[i] ) );
    }
    
}


/**
 * Overloaded equals operator.
 * Only if we have the extact same taxon names then these two clades are equal.
 */
bool Clade::operator==(const Clade &c) const 
{
    
    if ( c.getBitRepresentation().size() != bitset.size() || c.getBitRepresentation().getNumberSetBits() != bitset.getNumberSetBits() )
    {
        return false;
    }
    
    // Sebastian (10/19/2015): We cannot use the clade age for comparison because
    //                         otherwise we cannot find the same clade in different trees.
//    if ( c.getAge() != age )
//    {
//        return false;
//    }
    
    const RbBitSet &your_bits = c.getBitRepresentation();
    for (size_t i = 0; i < bitset.size(); ++i)
    {
        if ( bitset[i] != your_bits[i] )
        {
            return false;
        }
    }
    
    return true;
}


/**
 * Not equals operator that uses the equals operator.
 */
bool Clade::operator!=(const Clade &c) const 
{
    return !operator==( c );
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator<(const Clade &c) const 
{
    
    const RbBitSet &your_bits = c.getBitRepresentation();
    if ( bitset.getNumberSetBits() < your_bits.getNumberSetBits() )
    {
        return true;
    }
    else if ( bitset.getNumberSetBits() > your_bits.getNumberSetBits() )
    {
        return false;
    }
    
    for (size_t i = 0; i < bitset.size(); ++i)
    {
        
        if ( bitset[i] == true && your_bits[i] == false )
        {
            return true;
        }
        else if ( bitset[i] == false && your_bits[i] == true )
        {
            return false;
        }
        
    }
    
    return false;
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator<=(const Clade &c) const
{
    return operator<( c ) || operator==( c );
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator>(const Clade &c) const
{
    return operator<( c ) == false && operator==( c ) == false;
}


/**
 * Less than operator so that we can sort the clades.
 */
bool Clade::operator>=(const Clade &c) const
{
    return operator>( c ) == false;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Clade* Clade::clone(void) const 
{
    return new Clade(*this);
}


/**
 * Add a taxon to the list.
 *
 * \param[in]    t    The new taxon.
 */
void Clade::addTaxon(const Taxon &t)
{
    size_t index = GLOBAL_TAXON_MAP->getTaxonIndex( t );
    bitset.set( index );
}


/**
 * Get the clade age.
 *
 * \return    The stored age.
 */
double Clade::getAge( void ) const 
{
    
    return age;
}


/**
  * Get all taxa as a bitset.
  *
  * \return       The bitset.
  */
const RbBitSet& Clade::getBitRepresentation( void ) const
{
    return bitset;
}


/**
 * Get number of missing taxa.
 *
 * \return       The number of missing taxa.
 */
int Clade::getNumberMissingTaxa( void ) const
{
    return num_missing;
}


/**
 * Get all taxon names.
 *
 * \return       The vector of taxon names.
 */
std::vector<Taxon> Clade::getTaxa( void ) const
{
    std::vector<Taxon> taxa;
    RbPtr<TaxonMap> tm = GLOBAL_TAXON_MAP;
    for (size_t i=0; i<bitset.size(); ++i)
    {
        if ( bitset.isSet(i) == true )
        {
            taxa.push_back( tm->getTaxon(i) );
        }
    }
    
    return taxa;
}


/**
 * Get the taxon name at position i.
 *
 * \param[in]    i    The index for the taxon name we are interested in.
 *
 * \return       The name of the taxon.
 */
const Taxon& Clade::getTaxon(size_t index) const
{
    RbPtr<TaxonMap> tm = GLOBAL_TAXON_MAP;
    size_t k=0;
    for (size_t i=0; i<bitset.size(); ++i)
    {
        
        if ( bitset.isSet(i) == true )
        {
            if ( k == index )
            {
                return tm->getTaxon(i);
            }
            ++k;
        }
        
    }
    
    throw RbException("Could not find taxon with index in the clade.");
    
    return Taxon("");
}


/**
 * Get the taxon name at position i.
 *
 * \param[in]    i    The index for the taxon name we are interested in.
 *
 * \return       The name of the taxon.
 */
Taxon& Clade::getTaxon(size_t index)
{
    RbPtr<TaxonMap> tm = GLOBAL_TAXON_MAP;
    size_t k=0;
    for (size_t i=0; i<bitset.size(); ++i)
    {
        
        if ( bitset.isSet(i) == true )
        {
            if ( k == index )
            {
                return tm->getTaxon(i);
            }
            ++k;
        }
        
    }
    
    throw RbException("Could not find taxon with index in the clade.");
    
//    return Taxon("");
}


///**
// * Get the taxon name at position i.
// *
// * \param[in]    i    The index for the taxon name we are interested in.
// *
// * \return       The name of the taxon.
// */
//const std::string& Clade::getTaxonName(size_t i) const
//{
//    return taxa[i].getName();
//}


/**
 * Set the age of the clade.
 *
 * \param[in]    age  The age of the clade.
 *
 */
void Clade::setAge(double a)
{
    age = a;
}


/**
 * Set the number of missing taxa.
 *
 * \param[in]    n      The number of missing taxa.
 *
 */
void Clade::setNumberMissingTaxa(int n)
{
    num_missing = n;
}


///**
// * Set the taxon age at position i.
// *
// * \param[in]    i    The index for the taxon we are interested in.
// * \param[in]    age  The age of the taxon to set.
// *
// */
//void Clade::setTaxonAge(size_t i, double age)
//{
//    taxa[i].setAge(age);
//}

/**
 * Get the number of taxa contained in this clade.
 *
 * \return       Size of the taxon name vector.
 */
size_t Clade::size(void) const 
{
    return bitset.getNumberSetBits();
}


/**
 * Write the value of this clade as a string.
 *
 * \return    A single string containing the entire clade.
 */
std::string Clade::toString( void ) const
{
    std::string s = "{";
    
    RbPtr<TaxonMap> taxon_map = GLOBAL_TAXON_MAP;
    
    size_t j = 0;
    for (size_t i = 0; i < bitset.size(); ++i)
    {
        if ( bitset.isSet(i) == true )
        {
            if ( j > 0 )
            {
                s += ",";
            }
            s += taxon_map->getTaxon(i).getName();
            ++j;
        }
    }
    s += "}";
    
    return s;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Clade& x)
{
   
    o << x.toString();
   
    return o;
}

