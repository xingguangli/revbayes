#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraceReader.h"

#include <map>

using namespace RevBayesCore;


/** Read Model Trace */
std::vector<ModelTrace> TraceReader::readStochasticVariableTrace( const std::string &fn, const std::string &delimiter )
{
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn );
    if ( !myFileManager.testFile() || !myFileManager.testDirectory() )
    {        
        std::string errorStr = "";
        myFileManager.formatError( errorStr );
        throw( RbException(errorStr) );
    }
    
    std::vector<ModelTrace> data;
    
    bool hasHeaderBeenRead = false;
    
    // Open file
    std::ifstream inFile( fn.c_str() );
        
    if ( !inFile )
    {
        throw RbException( "Could not open file \"" + fn + "\"" );
    }
        
    // Initialize
    std::string commandLine;
//        RBOUT("Processing file \"" + *p + "\"");
        
    // Command-processing loop
    while ( inFile.good() )
    {
            
        // Read a line
        std::string line;
        getline( inFile, line );
        
        // skip empty lines
        //line = stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        if (line.length() == 0)
        {
            continue;
        }
            
            
        // removing comments
        if (line[0] == '#')
        {
            continue;
        }
            
        // splitting every line into its columns
        std::vector<std::string> columns;
        StringUtilities::stringSplit(line, delimiter, columns);
            
        // we assume a header at the first line of the file
        if ( hasHeaderBeenRead == false )
        {
                
            for (size_t j=0; j<columns.size(); ++j)
            {
                ModelTrace t;
                    
                std::string parmName = columns[j];
                t.setParameterName(parmName);
                t.setFileName( fn );
                    
                data.push_back( t );
            }
                
            hasHeaderBeenRead = true;
                
            continue;
        }
            
        // adding values to the Tracess
        for (size_t j=0; j<columns.size(); ++j)
        {
            ModelTrace& t = data[j];
            std::string tmp = columns[j];
            t.addObject( tmp );
        }
            
    }
    
    // return the vector of traces
    return data;
}
