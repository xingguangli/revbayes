//
//  BitsetCharacterDataConverter.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/1/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "BitsetCharacterDataConverter.h"
#include "DiscreteTaxonData.h"
#include "StandardState.h"
#include "NaturalNumbersState.h"

#include <algorithm>


using namespace RevBayesCore;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
//BitsetCharacterDataConverter::BitsetCharacterDataConverter(void)
BitsetCharacterDataConverter::BitsetCharacterDataConverter(const HomologousDiscreteCharacterData<StandardState>& d, std::string f, size_t ns) : data(d), format(f), numStates(ns), numAllStates(0)
{
    // get dimensions
    numTaxa = data.getNumberOfTaxa();
    numChars = data.getNumberOfCharacters();
    numAllStates = (size_t)(std::pow(double(2),int(numChars)));
    if (numStates == 0) {
        numStates = numAllStates;
    }
    
    
    // create bit containers
    initializeBits((size_t)numChars);
}


/**
 * Data converter from DNA into NaturalNumbersState.
 *
 * This function concverts a DNA matrix into a NaturalNumbersState matrix of given virtualPopulationSize,
 * using the given mapping between sequence name and species name.
 */
HomologousDiscreteCharacterData<NaturalNumbersState>* BitsetCharacterDataConverter::convertData(void)
{
    // create empty character data to be returned
    HomologousDiscreteCharacterData<NaturalNumbersState>* dataConverted = new HomologousDiscreteCharacterData<NaturalNumbersState> ();
    
    for (size_t i = 0; i < numTaxa; i++)
    {
        DiscreteTaxonData<StandardState> taxon = data.getTaxonData(i);
        
        // get bit vector from taxon data
        std::vector<size_t> taxonChars;
        for (size_t j = 0; j < taxon.getNumberOfCharacters(); j++)
        {
            StandardState s = taxon[j];
            size_t k = (size_t)s.getStateIndex();
            taxonChars.push_back(k);
        }
        
        // get natural number value from bitset
        size_t numberState = bitsToStatesByNumOn[taxonChars];
        
//        for (size_t j = 0; j < taxonChars.size(); j++)
//        {
//            std::cout << taxonChars[j];
//        }
//        std::cout << " = " << numberState << "\n";
        
        // create NaturalNumberState character
        DiscreteTaxonData<NaturalNumbersState> taxonNN(taxon.getTaxonName());
        std::stringstream ss;
        ss << numberState;
        taxonNN.addCharacter(NaturalNumbersState(ss.str(), (int)numStates));
//        std::cout << numberState << " " << ss.str() << "\n";
        
        // add converted taxon to character data
        dataConverted->addTaxonData(taxonNN);
    }
  
    return dataConverted;
}

void BitsetCharacterDataConverter::initializeBits(size_t n)
{
    std::vector<size_t> v(numChars, 0);
    bitsByNumOn.resize(numChars+1);
    statesToBitsByNumOn.resize(numAllStates);
    
    // fill out bitsByNumOn
    statesToBits = std::vector<std::vector<size_t> >(numAllStates, std::vector<size_t>(numChars, 0));
    bitsByNumOn[0].push_back(statesToBits[0]);
    for (size_t i = 1; i < numAllStates; i++)
    {
        size_t m = i;
        for (size_t j = 0; j < numChars; j++)
        {
            statesToBits[i][j] = m % 2;
            m /= 2;
            if (m == 0)
                break;
        }
        size_t j = numBitsOn(statesToBits[i]);
        
        bitsByNumOn[j].push_back(statesToBits[i]);

    }

    
    // assign state to each bit vector, sorted by numOn
    size_t k = 0;
    for (size_t i = 0; i < bitsByNumOn.size(); i++)
    {
        for (size_t j = 0; j < bitsByNumOn[i].size(); j++)
        {
            statesToBitsByNumOn[k++] = bitsByNumOn[i][j];
        }
    }
    
    for (size_t i = 0; i < statesToBitsByNumOn.size(); i++)
    {
        bitsToStatesByNumOn[ statesToBitsByNumOn[i] ] = (size_t)i;
//        std::cout << i << " : ";
//        for (size_t j = 0; j < statesToBitsByNumOn[i].size(); j++)
//            std::cout << statesToBitsByNumOn[i][j];
//        std::cout << "\n";
    }
    
    return;
}

size_t BitsetCharacterDataConverter::numBitsOn(std::vector<size_t> v)
{
    size_t n = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        n += v[i];
    }
    return n;
}



