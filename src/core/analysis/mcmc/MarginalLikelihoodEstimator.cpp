#include "MarginalLikelihoodEstimator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"

using namespace RevBayesCore;


MarginalLikelihoodEstimator::MarginalLikelihoodEstimator(const std::string &fn, const std::string &pn, const std::string &ln, const std::string &del) :
    powers(),
    likelihoodSamples()
{
    
    setActivePID( 0, 1 );
    
    
    if ( process_active == true )
    {
        /********************/
        /* read in the file */
        /********************/
    
        // check that the file/path name has been correctly specified
        RbFileManager myFileManager( fn );
    
        if ( !myFileManager.isFile() )
        {
            throw RbException("We currently only support reading of single files for marginal likelihood estimation.");
        }
    
    
        bool hasHeaderBeenRead = false;
        
        // Open file
        std::ifstream inFile( fn.c_str() );
        
        if ( !inFile )
        {
            throw RbException( "Could not open file \"" + fn + "\"" );
        }
    
        // Initialize
        std::string commandLine;
    
        //    RBOUT("Processing file \"" + fn + "\"");
    
        size_t powerColumnIndex = RbConstants::Size_t::nan;
        size_t likelihoodColumnIndex = RbConstants::Size_t::nan;
        size_t index = 0;
    
        double previousPower = -1.0;
        // loop over file content
        while ( inFile.good() )
        {
            
            // Read a line
            std::string line;
            getline( inFile, line );
            
            // skip empty lines
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
            StringUtilities::stringSplit(line, del, columns);
            
            // we assume a header at the first line of the file
            if ( hasHeaderBeenRead == false )
            {
            
                for (size_t j=0; j<columns.size(); j++)
                {
                
                    if ( columns[j] == pn )
                    {
                        powerColumnIndex = j;
                    }
                    else if ( columns[j] == ln )
                    {
                        likelihoodColumnIndex = j;
                    }
                
                }
            
                hasHeaderBeenRead = (powerColumnIndex != RbConstants::Size_t::nan && likelihoodColumnIndex != RbConstants::Size_t::nan);
            
                continue;
            }
        
            // get the power entry
            std::string tmp = columns[powerColumnIndex];
            double p = atof( tmp.c_str() );
            if ( p != previousPower )
            {
                previousPower = p;
                powers.push_back( p );
                likelihoodSamples.push_back( std::vector<double>() );
                index++;
            }
        
            // get the likelihood entry
            tmp = columns[likelihoodColumnIndex];
            double l = atof( tmp.c_str() );
            likelihoodSamples[index-1].push_back( l );

        }
    
        inFile.close();
    }
    
}




MarginalLikelihoodEstimator::~MarginalLikelihoodEstimator()
{
    
}

