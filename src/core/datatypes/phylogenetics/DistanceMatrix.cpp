//
//  DistanceMatrix.cpp
//
//  Created by Bastien Boussau on 4/8/15.
//  Copyright (c) 2015 Bastien Boussau. All rights reserved.
//

#include "DistanceMatrix.h"
#include "DistanceMatrixReader.h"
#include <sstream>
#include <string>

using namespace RevBayesCore;

DistanceMatrix::DistanceMatrix( void ) :
    matrix( 2 ),
    names(),
    numTips( 2 )
{
    
}

DistanceMatrix::DistanceMatrix( size_t n ) :
    matrix( n ),
    names(),
    numTips( n )
{
    
}


DistanceMatrix::DistanceMatrix(DistanceMatrixReader* tadr) : filename(tadr->getFilename())
{
    names = tadr->getNames();
	matrix = tadr->getMatrix();
	numTips = names.size();
}

DistanceMatrix::DistanceMatrix(const DistanceMatrix& a)
{
    *this = a;
}

DistanceMatrix::DistanceMatrix(const MatrixReal& a, std::vector<std::string>& nam)
{
	names = nam;
	matrix = a;
	numTips = names.size();

}


DistanceMatrix& DistanceMatrix::operator=(const DistanceMatrix& a)
{
    if (this != &a)
    {
        names = a.names;
		matrix = a.matrix;
		filename = a.filename;
		numTips = a.numTips;
    }
    
    return *this;
}

DistanceMatrix* DistanceMatrix::clone(void) const
{
    return new DistanceMatrix(*this);
}

const std::vector<std::string>& DistanceMatrix::getNames(void) const
{
    return names;
}

const MatrixReal& DistanceMatrix::getMatrix(void) const
{
    return matrix;
}

size_t DistanceMatrix::getSize(void) const
{
	return numTips;
}


std::string DistanceMatrix::getFilename(void) const
{
    return filename;
}


size_t DistanceMatrix::size(void) const
{
	return matrix.size();
}


RbVector<double>& DistanceMatrix::operator[]( size_t index )
{
	
	return matrix[index];
}


const RbVector<double>& DistanceMatrix::operator[]( size_t index ) const
{
	return matrix[index];
}


double& DistanceMatrix::getElement( size_t i, size_t j )
{
	return matrix[i][j];
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const DistanceMatrix& x) {
	
    std::stringstream s;
    
    // Generate nice header
    o << std::endl;

    s << "DistanceMatrix with " << x.getSize() << " tips. " << std::endl;

    o << s.str();
	std::vector<std::string> names = x.getNames();

	for ( size_t i = 0; i < x.getSize(); ++i ) {
		o << names[i] ;
		for ( size_t j = 0; j < x.getSize(); ++j ) {
        	o << "\t" << x.getMatrix()[i][j] ;
		}
		o << std::endl;
	}
    o << std::endl;
    
    return o;
}
