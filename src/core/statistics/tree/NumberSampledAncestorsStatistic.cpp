#include "NumberSampledAncestorsStatistic.h"
#include "RbConstants.h"
#include "RbException.h"

using namespace RevBayesCore;

NumberSampledAncestorsStatistic::NumberSampledAncestorsStatistic(const TypedDagNode<Tree> *t) : TypedFunction<size_t>( new size_t(0) ),
    tree( t )
{

    // add the tree parameter as a parent
    addParameter( tree );
    update();
}


NumberSampledAncestorsStatistic::~NumberSampledAncestorsStatistic( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

}



NumberSampledAncestorsStatistic* NumberSampledAncestorsStatistic::clone( void ) const
{
    
    return new NumberSampledAncestorsStatistic( *this );
}



void NumberSampledAncestorsStatistic::update( void )
{
    
    size_t totalSA = 0;
    Tree t = tree->getValue();
    size_t ntax = t.getNumberOfTips();
    
    for(size_t i=0; i<ntax; i++){
        TopologyNode &node = t.getNode(i);
        if(node.isSampledAncestor())
            totalSA++;
    }
    *value = totalSA;
}



void NumberSampledAncestorsStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tree){
        tree = static_cast<const TypedDagNode<Tree>* >( newP );
    }
}


