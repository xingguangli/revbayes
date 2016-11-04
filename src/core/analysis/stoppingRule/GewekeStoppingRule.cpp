#include "GewekeTest.h"
#include "GewekeStoppingRule.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraceContinuousReader.h"


using namespace RevBayesCore;


GewekeStoppingRule::GewekeStoppingRule(double a, double f1, double f2, const std::string &fn, size_t f, BurninEstimatorContinuous *be) : AbstractConvergenceStoppingRule(fn, f, be),
    alpha( a ),
    frac1( f1 ),
    frac2( f2 )
{
    
}



GewekeStoppingRule::~GewekeStoppingRule()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
GewekeStoppingRule* GewekeStoppingRule::clone( void ) const
{
    
    return new GewekeStoppingRule( *this );
}


/**
 * Should we stop now?
 * Yes, if the minimum ESS is larger than the provided threshold.
 */
bool GewekeStoppingRule::stop( size_t g )
{
    
    bool passed = true;
    
    for ( size_t i = 1; i <= numReplicates; ++i)
    {
        std::string fn = filename;
        if ( numReplicates > 1 )
        {
            RbFileManager fm = RbFileManager(filename);
            fn = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + "_run_" + StringUtilities::to_string(i) + "." + fm.getFileExtension();
        }
        
        TraceContinuousReader reader = TraceContinuousReader( fn );
        
        // get the vector of traces from the reader
        std::vector<TraceNumeric> &data = reader.getTraces();
        
        size_t maxBurnin = 0;
        
        for ( size_t j = 0; j < data.size(); ++j)
        {
            TraceNumeric &t = data[j];
            const std::vector<double> &v = t.getValues();
            size_t b = burninEst->estimateBurnin( v );
            if ( maxBurnin < b )
            {
                maxBurnin = b;
            }
            
        }
        
        GewekeTest gTest = GewekeTest( alpha, frac1, frac2 );
        
        for ( size_t j = 0; j < data.size(); ++j)
        {
            RevBayesCore::TraceNumeric &t = data[j];
            const std::vector<double> &v = t.getValues();
            t.setBurnin( maxBurnin );
            t.computeStatistics();
            
            passed &= gTest.assessConvergenceSingleChain( v, maxBurnin );
        }
        
    }
    
    
    return passed;
}
