#ifndef NexusWriter_H
#define NexusWriter_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "ContinuousCharacterData.h"
#include "Tree.h"

#include <fstream>
#include <string>

namespace RevBayesCore {
    
    /**
     * This class represents the writer object of character data objects into files in Nexus format.
     *
     * This class currently has only one functionality,
     * to write character data objects into a file in Nexus format.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-15, version 1.0
     */
    class NexusWriter {
        
    public:
        NexusWriter(const std::string& fn);
        
        // public methods
        void                    closeStream(void);                                                          //!< Close the file stream
        void                    openStream(void);                                                           //!< Open the file stream
        void                    writeNexusBlock(const AbstractHomologousDiscreteCharacterData &d);          //!< Write a nexus block with a discrete character data
        void                    writeNexusBlock(const ContinuousCharacterData &data);                       //!< Write a nexus block with a continuous character data
        void                    writeNexusBlock(const Clade &c);                                            //!< Write a nexus block with tree(s)
        void                    writeNexusBlock(const Tree &t);                                             //!< Write a nexus block with tree(s)
        void                    writeNexusBlock(const std::vector<Tree> &t);                                //!< Write a nexus block with tree(s)
        
    private:
    
        // members
        std::string             fileName;                                                                   //!< The file name
        std::fstream            outStream;                                                                  //!< the filestream object
        
    };
    
}


#endif
