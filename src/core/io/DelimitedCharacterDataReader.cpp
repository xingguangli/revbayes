#include "DelimitedCharacterDataReader.h"
#include "RbFileManager.h"

#include "RbConstants.h"


using namespace RevBayesCore;


DelimitedCharacterDataReader::DelimitedCharacterDataReader(const std::string &fn, char d, size_t num_skipped) : DelimitedDataReader(fn, d, num_skipped)
{
    
    filename = fn;

    data = std::vector<std::vector<std::string> >( chars.size(), std::vector<std::string>() );
    for (size_t i = 0; i < chars.size(); ++i)
    {
        
        std::string name = chars[i][0];
        
        names.push_back( name );
        
        std::vector<std::string>& characters = data[i];
        for (size_t j = 1; j < chars[i].size(); ++j)
        {
            characters.push_back( chars[i][j] );
        }
        
    }
    
}


const std::vector<std::string>& DelimitedCharacterDataReader::getNames(void)
{
    return names;
}


const std::vector<std::vector<std::string> >& DelimitedCharacterDataReader::getData(void)
{
    return data;
}