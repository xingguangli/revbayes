#include "AbstractHomologousDiscreteCharacterData.h"
#include "NexusWriter.h"
#include "RbFileManager.h"

#include <sstream>

using namespace RevBayesCore;

void AbstractHomologousDiscreteCharacterData::writeToFile(const std::string &dir, const std::string &fn) const
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    fm.createDirectoryForFile();
    
    NexusWriter nw( fm.getFullFileName() );
    nw.openStream();
    
    nw.writeNexusBlock( *this );
    
    nw.closeStream();
    
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractHomologousDiscreteCharacterData& x) {
    
    std::stringstream s;
    
    // Generate nice header
    o << std::endl;
    s << x.getDataType() << " character matrix with " << x.getNumberOfTaxa() << " taxa and " << x.getNumberOfCharacters() << " characters" << std::endl;
    o << s.str();
    
    for ( size_t i = 0; i < s.str().length() - 1; ++i )
        o << "=";
    o << std::endl;
    
    o << "Origination:                   " << x.getFileName() << std::endl;
    o << "Number of taxa:                " << x.getNumberOfTaxa() << std::endl;
    o << "Number of included taxa:       " << x.getNumberOfIncludedTaxa() << std::endl;
    o << "Number of characters:          " << x.getNumberOfCharacters() << std::endl;
    o << "Number of included characters: " << x.getNumberOfIncludedCharacters() << std::endl;
    o << "Datatype:                      " << x.getDataType() << std::endl;
    o << std::endl;
    
    return o;
}
