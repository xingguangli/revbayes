

#include "DagNode.h"
#include "Monitor.h"
#include "RbException.h"
#include "StringUtilities.h"

#include <algorithm>

using namespace RevBayesCore;



Monitor::Monitor(unsigned long g) :
    enabled( true ),
    printgen( g ),
    model( NULL )
{
    
}

Monitor::Monitor(unsigned long g, DagNode *n) :
    enabled( true ),
    printgen( g ),
    model( NULL )
{
    
    nodes.push_back( n );
    
    // add myself to the set of monitors
    n->addMonitor( this );
    
    // tell the node that we have a reference to it (avoids deletion)
    n->incrementReferenceCount();
}


Monitor::Monitor(unsigned long g, const std::vector<DagNode *> &n) :
    enabled( true ),
    printgen( g ),
    nodes( n ),
    model( NULL )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *the_node = *it;
        
        // add myself to the set of monitors
        the_node->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        the_node->incrementReferenceCount();
    }
    
    sortNodesByName();
    
}

Monitor::Monitor(const Monitor &m) :
    enabled( m.enabled ),
    nodes( m.nodes ),
    model( m.model )
{
    
    // set the print gen
    printgen = m.printgen;
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *the_node = *it;
        
        // add myself to the set of monitors
        the_node->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        the_node->incrementReferenceCount();
    }
    
}


Monitor::~Monitor( void )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *the_node = *it;
        
        // remove myself to the set of monitors
        the_node->removeMonitor( this );
        
        
        // tell the node that we have a reference to it (avoids deletion)
        if ( the_node->decrementReferenceCount() == 0 )
        {
            delete *it;
        }
    }

}


Monitor& Monitor::operator=(const Monitor &m)
{
    
    // check for self-assignment
    if ( &m != this )
    {
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            DagNode *the_node = *it;
            
            // remove myself to the set of monitors
            the_node->removeMonitor( this );

            // tell the node that we have a reference to it (avoids deletion)
            if ( the_node->decrementReferenceCount() == 0 )
            {
                delete *it;
            }
        }
        
        // set the nodes (we don't own them)
        nodes = m.nodes;
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            
            DagNode *the_node = *it;
            
            // add myself to the set of monitors
            the_node->addMonitor( this );

            // tell the node that we have a reference to it (avoids deletion)
            the_node->incrementReferenceCount();
        }
        
        // set the printgen
        printgen = m.printgen;
        
        model = m.model;
        
        enabled = m.enabled;
    }
    
    return *this;
}


/**
 * This is the dummy method for the setting of a filename extension.
 * Overwrite this method if necessary.
 */
void Monitor::addFileExtension(const std::string &s, bool dir)
{
    // nothing to do here
}


void Monitor::addVariable(DagNode *n)
{
        
    nodes.push_back( n );
    
    // add myself to the set of monitors
    n->addMonitor( this );
        
    // tell the node that we have a reference to it (avoids deletion)
    n->incrementReferenceCount();
    
    // now we need to sort again
    sortNodesByName();
    
}


/**
 * Close the stream for the monitor.
 * Overwrite this method for specialized behavior.
 */
void Monitor::closeStream( void )
{
    ; // dummy fn
}


/**
 * Combine output for the monitor.
 * Overwrite this method for specialized behavior.
 */
void Monitor::combineReplicates( size_t n_reps )
{
    // dummy implementation
}


/**
 * Disable this monitor.
 * This may be especially useful for screen monitors.
 */
void Monitor::disable( void )
{
    enabled = false;
}


/**
 * Enable this monitor.
 */
void Monitor::enable( void )
{
    enabled = true;
}


/**
 * Is this monitor currently enabled?
 */
bool Monitor::isEnabled( void ) const
{
    return enabled;
}


/**
 * Is this a file monitor?
 * Overwrite this method if necessary.
 */
bool Monitor::isFileMonitor( void ) const
{
    return false;
}


/**
 * Is this a screen monitor?
 * Overwrite this method if necessary.
 */
bool Monitor::isScreenMonitor( void ) const
{
    return false;
}


/**
 * Open the stream for the monitor.
 * Overwrite this method for specialized behavior.
 */
void Monitor::openStream( bool reopen )
{
    ; // dummy fn
}


/**
 * Print header information for the monitor.
 * Overwrite this method for specialized behavior.
 */
void Monitor::printHeader( void )
{
    ; // dummy fn
}



const std::vector<DagNode*>& Monitor::getDagNodes( void ) const
{
    
    return nodes;
}


void Monitor::removeVariable(DagNode *n)
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if ( *it == n )
        {
            // move myself to the set of monitors
            n->removeMonitor( this );
            
            // tell the node that we have a reference to it (avoids deletion)
            if ( n->decrementReferenceCount() == 0 )
            {
                delete *it;
            }
            
            nodes.erase( it );
            
            // we assume that nodes are unique
            break;
        }
    }
    
    // now we need to sort again
    sortNodesByName();
    
}


void Monitor::setDagNodes( const std::vector<DagNode *> &args)
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *the_node = *it;
        
        // remove myself to the set of monitors
        the_node->removeMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        if ( the_node->decrementReferenceCount() == 0 )
        {
            delete *it;
        }
    }
    
    // set the nodes (we don't own them)
    nodes = args;
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        DagNode *the_node = *it;
        
        // add myself to the set of monitors
        the_node->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        the_node->incrementReferenceCount();
    }
    
    sortNodesByName();

}


void Monitor::setModel(Model *m)
{
    model = m;
    
}


void Monitor::setMcmc(Mcmc *m)
{
    mcmc = m;
}


/**
 * Sort the nodes by name so that the order is guaranteed of replicated runs.
 */
void Monitor::sortNodesByName( void )
{
    
    std::vector<std::string> names;
    std::map<std::string,DagNode*> nodesMap;
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        DagNode *node = *it;
        std::string name = node->getName();
        StringUtilities::toLower(name);
        nodesMap.insert( std::pair<std::string,DagNode*>(name,node) );
        names.push_back(name);
    }
    
    nodes.clear();
    std::sort (names.begin(), names.end());
    
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
    {
        DagNode *node = nodesMap[*it];
        nodes.push_back( node );
    }
    
}


void Monitor::swapNode(DagNode *oldN, DagNode *newN) 
{
    // error catching
    std::vector<DagNode*>::iterator it = find(nodes.begin(), nodes.end(), oldN);

    if (it == nodes.end())
    {
        throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this monitor because the monitor doesn't hold this DAG node.");
    }
    
    // remove myself from the old node and add myself to the new node
    oldN->removeMonitor( this );
    newN->addMonitor( this );
    
    // increment and decrement the reference counts
    newN->incrementReferenceCount();
    if ( oldN->decrementReferenceCount() == 0 )
    {
        throw RbException("Memory leak in monitor. Please report this bug to Sebastian.");
    }
    
    it = nodes.insert( it, newN );
    nodes.erase( it + 1 );
}


/**
 * Reset the variables for the monitor.
 * Overwrite this method for specialized behavior.
 */
void Monitor::reset(size_t numCycles)
{
    // dummy implementation
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Monitor& x)
{
    o << "Monitor";
    
    return o;
}

