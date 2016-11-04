#include "Clade.h"
#include "RbVectorUtilities.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace RevBayesCore;


/**
 * Default constructor required by the revlanguage code.
 * We use an empty string and an age of 0.0 for
 * this default object.
 */
Clade::Clade( void ) :
    age( 0.0 ),
    num_missing( 0 ),
    taxa()
{
    
}


/**
 * Constructor with a single taxon.
 */
Clade::Clade( const Taxon &t ) :
    age( 0.0 ),
    num_missing( 0 ),
    taxa()
{
    
    taxa.push_back( t );
}


/**
 * Default constructor that instantiates the object.
 * Additionally, we sort the vector of taxon names.
 *
 * \param[in]   n    The vector containing the taxon names.
 */
Clade::Clade(const std::vector<Taxon> &n) :
    age( 0.0 ),
    num_missing( 0 ),
    taxa( n )
{
    
    // for identifiability we always keep the taxon names sorted
//    std::sort(taxa.begin(), taxa.end());
    VectorUtilities::sort( taxa );
}


/**
 * Overloaded equals operator.
 * Only if we have the extact same taxon names then these two clades are equal.
 */
bool Clade::operator==(const Clade &c) const 
{
    
    if ( c.size() != taxa.size() )
    {
        return false;
    }
    
    // Sebastian (10/19/2015): We cannot use the clade age for comparison because
    //                         otherwise we cannot find the same clade in different trees.
//    if ( c.getAge() != age )
//    {
//        return false;
//    }
    
    for (size_t i = 0; i < taxa.size(); ++i)
    {
        if ( taxa[i] != c.getTaxon(i) )
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
    
    if ( taxa.size() < c.size() )
    {
        return true;
    }
    else if ( taxa.size() > c.size() )
    {
        return false;
    }
    
    for (size_t i = 0; i < taxa.size(); ++i)
    {
        
        if ( taxa[i] < c.getTaxon(i) )
        {
            return true;
        }
        else if ( taxa[i] > c.getTaxon(i) )
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
 * Get the const-iterator to the first taxon name.
 */
std::vector<Taxon>::const_iterator Clade::begin(void) const
{
    return taxa.begin();
}


/**
 * Get the iterator to the first taxon name.
 */
std::vector<Taxon>::iterator Clade::begin(void)
{
    return taxa.begin();
}


/**
 * Get the const-iterator after the last taxon name.
 */
std::vector<Taxon>::const_iterator Clade::end(void) const
{
    return taxa.end();
}


/**
 * Get the iterator after the last taxon name.
 */
std::vector<Taxon>::iterator Clade::end(void)
{
    return taxa.end();
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
    return taxa.push_back( t );
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
std::vector<Taxon>& Clade::getTaxa( void )
{
    return taxa;
}


/**
 * Get all taxon names.
 *
 * \return       The vector of taxon names.
 */
const std::vector<Taxon>& Clade::getTaxa( void ) const
{
    return taxa;
}


/**
 * Get the taxon name at position i.
 *
 * \param[in]    i    The index for the taxon name we are interested in.
 *
 * \return       The name of the taxon.
 */
const Taxon& Clade::getTaxon(size_t i) const
{
    return taxa[i];
}


/**
 * Get the taxon name at position i.
 *
 * \param[in]    i    The index for the taxon name we are interested in.
 *
 * \return       The name of the taxon.
 */
const std::string& Clade::getTaxonName(size_t i) const
{
    return taxa[i].getName();
}


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


/**
 * Set the taxon age at position i.
 *
 * \param[in]    i    The index for the taxon we are interested in.
 * \param[in]    age  The age of the taxon to set.
 *
 */
void Clade::setTaxonAge(size_t i, double age)
{
    taxa[i].setAge(age);
}

/**
 * Get the number of taxa contained in this clade.
 *
 * \return       Size of the taxon name vector.
 */
size_t Clade::size(void) const 
{
    return taxa.size();
}


/**
 * Write the value of this clade as a string.
 *
 * \return    A single string containing the entire clade.
 */
std::string Clade::toString( void ) const
{
    std::string s = "{";
    
    for (size_t i = 0; i < taxa.size(); ++i)
    {
        if ( i > 0 )
        {
            s += ",";
        }
        s += taxa[i].getName();
    }
    s += "}";
    
    return s;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Clade& x) {
   
    o << x.toString();
   
    return o;
}

