#include "RbSettings.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"

#include <cstdlib> //includes std::atof
#include <iostream>
#include <string>
#include <sys/stat.h>

#include <dirent.h>
#include <unistd.h>



/** Default constructor: The default settings are first read, and 
 * then potentially overwritten by values contained in a file.  */
RbSettings::RbSettings(void)
{

	initializeUserSettings();
}


const std::string& RbSettings::getModuleDir( void ) const
{
    
    return moduleDir;
}


size_t RbSettings::getLineWidth( void ) const
{
    // return the internal value
    return lineWidth;
}

size_t RbSettings::getScalingDensity( void ) const
{
    // return the internal value
    return scalingDensity;
}


std::string RbSettings::getOption(const std::string &key) const
{
    if ( key == "moduledir" )
    {
        return moduleDir;
    }
    else if ( key == "printNodeIndex" )
    {
        return printNodeIndex ? "TRUE" : "FALSE";
    }
    else if ( key == "tolerance" )
    {
        return StringUtilities::to_string(tolerance);
    }
    else if ( key == "linewidth" )
    {
        return StringUtilities::to_string(lineWidth);
    }
    else if ( key == "scalingDensity" )
    {
        return StringUtilities::to_string(scalingDensity);
    }
    else
    {
        std::cout << "Unknown user setting with key '" << key << "'." << std::endl;
    }
    
    return "";
}


bool RbSettings::getPrintNodeIndex( void ) const
{
    // return the internal value
    return printNodeIndex;
}


double RbSettings::getTolerance( void ) const
{
    
    return tolerance;
}


const std::string& RbSettings::getWorkingDirectory( void ) const
{
    
    return workingDirectory;
}


/** Initialize the user settings */
#define	MAX_DIR_PATH	2048
void RbSettings::initializeUserSettings(void)
{
    moduleDir = "modules";      // the default module directory
    scalingDensity = 4;         // the default scaling density
    lineWidth = 160;            // the default line width
    tolerance = 10E-10;         // set default value for tolerance comparing doubles
    printNodeIndex = true;      // print node indices of tree nodes as comments
    
    std::string user_dir = RevBayesCore::RbFileManager::expandUserDir("~");
    
    // read the ini file, override defaults if applicable
    std::string settings_file_name = ".RevBayes.ini";
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(user_dir, settings_file_name);

 	
    //    bool failed = false; //unused
    if ( fm.isFile() )
    {
        std::ifstream readStream;
        fm.openFile( readStream );
        std::string readLine = "";
        while ( std::getline(readStream,readLine) )
        {
            std::vector<std::string> tokens = std::vector<std::string>();
            StringUtilities::stringSplit(readLine, "=", tokens);
            if (tokens.size() > 1)
            {
                setOption(tokens[0], tokens[1], false);
            }
        }
        
        fm.closeFile(readStream);
    }

    // initialize the current directory to be the directory the binary is sitting in
    char cwd[MAX_DIR_PATH+1];
	if ( getcwd(cwd, MAX_DIR_PATH+1) )
    {
#	ifdef RB_WIN
        std::string pathSeparator = "\\";
#	else
        std::string pathSeparator = "/";
#   endif
        
        std::string curdir = cwd;
        
        if ( curdir.at( curdir.length()-1 ) == pathSeparator[0] )
        {
            curdir.erase( curdir.length()-1 );
        }
        
        workingDirectory = curdir;
	}
    else
    {
        workingDirectory = "";
    }
    
    // save the current settings for the future.
//    writeUserSettings();
}


void RbSettings::setModuleDir(const std::string &md)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(md);
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the help directory to '" + md + "'.");
    }
    
    moduleDir = fm.getFullFilePath();
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setLineWidth(size_t w)
{
    // replace the internal value with this new value
    lineWidth = w;
    
    // save the current settings for the future.
    writeUserSettings();
}

void RbSettings::setScalingDensity(size_t w)
{
    if(w < 1)
        throw(RbException("scalingDensity must be an integer greater than 0"));
    
    // replace the internal value with this new value
    scalingDensity = w;
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setOption(const std::string &key, const std::string &value, bool write)
{
    
    if ( key == "moduledir" )
    {
        moduleDir = value;
    }
    else if ( key == "printNodeIndex" )
    {
        printNodeIndex = value == "TRUE";
    }
    else if ( key == "tolerance" )
    {
        //std::string::size_type sz;     // alias of size_t
        //tolerance = std::stod (value,&sz);
        tolerance = (double)atof(value.c_str());
    }
    else if ( key == "linewidth" )
    {
        //std::string::size_type sz;     // alias of size_t
        //lineWidth = std::stoi (value,&sz);
        lineWidth = atoi(value.c_str());
    }
    else if ( key == "scalingDensity" )
    {
        size_t w = atoi(value.c_str());
        if(w < 1)
            throw(RbException("scalingDensity must be an integer greater than 0"));
        
        scalingDensity = atoi(value.c_str());
    }
    else
    {
        std::cout << "Unknown user setting with key '" << key << "'." << std::endl;
    }
    
    if ( write == true )
    {
        writeUserSettings();
    }
    
}


void RbSettings::setPrintNodeIndex(bool tf)
{
    // replace the internal value with this new value
    printNodeIndex = tf;
}


void RbSettings::setTolerance(double t)
{
    // replace the internal value with this new value
    tolerance = t;
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::setWorkingDirectory(const std::string &wd)
{
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager( wd );
    
    if ( !fm.isDirectory() )
    {
        throw RbException("Cannot set the current directory to '" + wd + "'.");
    }
    
    workingDirectory = fm.getFullFilePath();
    
    // save the current settings for the future.
    writeUserSettings();
}


void RbSettings::writeUserSettings( void )
{
    std::string user_dir = RevBayesCore::RbFileManager::expandUserDir("~");
    
    // open the ini file
    std::string settings_file_name = ".RevBayes.ini";
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(user_dir, settings_file_name);

    std::ofstream writeStream;
    fm.openFile( writeStream );
    writeStream << "moduledir=" << moduleDir << std::endl;
    writeStream << "printNodeIndex=" << (printNodeIndex ? "TRUE" : "FALSE") << std::endl;
    writeStream << "tolerance=" << tolerance << std::endl;
    writeStream << "linewidth=" << lineWidth << std::endl;
    writeStream << "scalingDensity=" << scalingDensity << std::endl;
    fm.closeFile( writeStream );

}
