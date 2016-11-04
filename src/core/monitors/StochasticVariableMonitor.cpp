#include "StochasticVariableMonitor.h"
#include "DagNode.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "StochasticNode.h"

using namespace RevBayesCore;

/* Constructor */
StochasticVariableMonitor::StochasticVariableMonitor(unsigned long g, const std::string &fname, const std::string &del) : AbstractFileMonitor(std::vector<DagNode *>(),g,fname,del)
{
    posterior  = false;
    prior      = false;
    likelihood = false;
    flatten    = false;
}


/**
 * Destructor.
 */
StochasticVariableMonitor::~StochasticVariableMonitor()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
StochasticVariableMonitor* StochasticVariableMonitor::clone(void) const
{
    
    return new StochasticVariableMonitor(*this);
}



/**
 * Reset the currently monitored DAG nodes by extracting the DAG nodes from the StochasticVariable again
 * and store this in the set of DAG nodes.
 */
void StochasticVariableMonitor::resetDagNodes( void )
{
    
    // for savety we empty our dag nodes
    while ( nodes.empty() == false )
    {
        removeVariable( *nodes.begin() );
    }
    
    if ( model != NULL )
    {
        // we only want to have each nodes once
        // this should by default happen by here we check again
        std::set<std::string> var_names;
        
        const std::vector<DagNode*> &n = model->getDagNodes();
        for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
        {
            
            DagNode *the_node = *it;
            
            // only non clamped variables
            if ( !the_node->isClamped())
            {
                if ( the_node->isStochastic() && the_node->isHidden() == false )
                {
                    const std::string &name = the_node->getName();
                    if ( var_names.find( name ) == var_names.end() )
                    {
                        addVariable( the_node );
                        var_names.insert( name );
                    }
                    else
                    {
#ifdef DEBUG_SEBASTIAN
                        std::cerr << "Trying to add variable with name '" << name << "' twice." << std::endl;
#endif
                    }
                    
                }
                
            }
            
        }
        
    }
    
}


/**
 * Set the StochasticVariable from which this monitor will extract the variables.
 * This will automatically result into a reseting of the currently monitored variables.
 *
 * \param[in]   m    The new model.
 */
void StochasticVariableMonitor::setModel(Model *m)
{
    
    // delegate call to super class
    Monitor::setModel(m);
    
    // reset the DAG nodes that should be monitored
    resetDagNodes();
    
    sortNodesByName();
}

