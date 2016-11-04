/**
 * @file
 * This file contains the declaration of a TopologyNode. Tree nodes are member objects and therefore can hold
 * all the variables one might to associate to the tree. The tree nodes are used to create the structure
 * of a tree. They provide access to their parent and children.
 *
 * The usage of tree nodes is to create and give easy access to parts of the dag, namely variables hold
 * by the nodes which need access to ancestral variables.
 *
 * We do not distinguish between branch parameter and node parameters. A branch parameter is simply set as
 * a node parameter of the descending node.
 *
 * A tree node can have a distribution associated with it, for instance in the situation when we condition
 * on a group of taxa being monophyletic.
 *
 *
 * NOTE: This class might be a temporary solution being the unified solution for all tree nodes. In the future
 * we might make this class abstract and implement at least the two types of tree nodes: bifurcating tree nodes
 * which are restricted two have exactly two descendants and multifurcating tree nodes which can have any number
 * of tree nodes. Perhaps there might be also tip nodes as a derived class and a root node, which is the
 * correct OO design approach but comes with a lot of overhead. A fast conversion method would be needed.
 *
 * @brief Declaration of TopologyNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-17 10:31:20 +0200 (Tue, 17 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends MemberObject
 *
 * $Id: TopologyNode.h 1682 2012-07-17 08:31:20Z hoehna $
 */

#ifndef TopologyNode_H
#define TopologyNode_H

#include "Clade.h"
#include "Taxon.h"

#include <vector>
#include <map>
#include <string>

namespace RevBayesCore {
    
    class Tree;
    
    class TopologyNode  {
        
    public:
        TopologyNode(size_t indx=0);                                                                                                       //!< Default constructor with optional index
        TopologyNode(const std::string& n, size_t indx=0);                                                                                 //!< Constructor with name and optional index
        TopologyNode(const Taxon& t, size_t indx=0);                                                                                 //!< Constructor with taxon and optional index
        TopologyNode(const TopologyNode &n);                                                                                            //!< Copy constructor
        virtual                                    ~TopologyNode(void);                                                                 //!< Destructor
        TopologyNode&                               operator=(const TopologyNode& n);
        
        
        // Basic utility functions
        TopologyNode*                               clone(void) const;                                                                  //!< Clone object
        bool                                        equals(const TopologyNode& node) const;                                             //!< Test whether this is the same node
        
        // public methods
        void                                        addBranchParameter(const std::string &n, double p);
        void                                        addBranchParameter(const std::string &n, const std::string &p);
        void                                        addBranchParameters(const std::string &n, const std::vector<double> &p, bool io);
        void                                        addBranchParameters(const std::string &n, const std::vector<std::string> &p, bool io);
        void                                        addChild(TopologyNode* c);                                                          //!< Adds a child node
        void                                        addNodeParameter(const std::string &n, double p);
        void                                        addNodeParameter(const std::string &n, const std::string &p);
        void                                        addNodeParameters(const std::string &n, const std::vector<double> &p, bool io);
		void                                        addNodeParameters(const std::string &n, const std::vector<std::string*> &p, bool io);
        void                                        clearParameters(void);                                                              //!< Clear the node and branch parameters
        void                                        clearBranchParameters(void);
		void                                        clearNodeParameters(void);
        virtual std::string                         computeNewick(void);                                                                //!< Compute the newick string for this clade
        std::string                                 computePlainNewick(void) const;                                                     //!< Compute the newick string for this clade as a plain string without branch length
        bool                                        containsClade(const TopologyNode* c, bool strict) const;
        bool                                        containsClade(const Clade &c, bool strict) const;
        double                                      getAge(void) const;                                                                 //!< Get the age (time ago from present) for this node
        const std::vector<std::string>&             getBranchParameters(void) const;                                                        //!< Get the branch length leading towards this node
        double                                      getBranchLength(void) const;                                                        //!< Get the branch length leading towards this node
        size_t                                      getCladeIndex(const TopologyNode* c) const;
        const TopologyNode&                         getChild(size_t i) const;                                                           //!< Returns the i-th child
        TopologyNode&                               getChild(size_t i);                                                                 //!< Returns the i-th child (non-const to return non-const node)
        const std::vector<TopologyNode*>&           getChildren(void) const;
        std::vector<int>                            getChildrenIndices(void) const;                                                     //!< Return children indices
        Clade                                       getClade(void) const;                                                               //!< Get the clade this node represents
        size_t                                      getIndex(void) const;                                                               //!< Get index of node
        double                                      getMaxDepth(void) const;                                                            //!< Get the maximum depth from this node (time between this node and most recent tip)
        const std::string&                          getName() const;                                                                    //!< Get name of node
        const std::vector<std::string>&             getNodeParameters(void) const;                                                      //!< Get the branch length leading towards this node
        size_t                                      getNumberOfChildren(void) const;                                                    //!< Returns the number of children
        size_t                                      getNumberOfNodesInSubtree(bool tips) const;   
        
     // std::string                                 getNodeField(std::string key) const;
     // size_t                                      getNodeFieldNumber() const;
        
        //!< Get the number of nodes contained in this subtree.
        TopologyNode&                               getParent(void);                                                                    //!< Returns the node's parent
        const TopologyNode&                         getParent(void) const;                                                              //!< Returns the node's parent
        std::string                                 getSpeciesName() const;                                                             //!< Get the species name for the node
        void                                        getTaxa(std::vector<Taxon> &taxa) const;                                            //!< Fill the vector of taxa
        const Taxon&                                getTaxon() const;                                                                   //!< Fill the vector of taxa
        double                                      getTmrca(const Clade &c) const;
        double                                      getTmrca(const TopologyNode &n) const;
        double                                      getTmrca(const std::vector<Taxon> &t) const;
        bool                                        isFossil(void) const;                                                               //!< Is node a fossil?
        bool                                        isInternal(void) const;                                                             //!< Is node internal?
        bool                                        isRoot(void) const;                                                                 //!< Is node root?
        bool                                        isSampledAncestor(void) const;                                                      //!< Is node a sampled ancestor?
        bool                                        isTip(void) const;                                                                  //!< Is node tip?
        void                                        makeBifurcating(void);                                                              //!< Make this and all its descendants bifurcating.
        void                                        removeAllChildren(void);                                                            //!< Removes all of the children of the node
        void                                        removeChild(TopologyNode* c);                                                       //!< Removes a specific child
        void                                        removeTree(Tree *t);                                                                //!< Removes the tree pointer
        void                                        setAge(double a);                                                                   //!< Set the age of this node (should only be done for tips).
        void                                        setBranchLength(double b);                                                          //!< Set the length of the branch leading to this node.
        void                                        setFossil(bool tf);                                                                 //!< Set if the node is a fossil node
        void                                        setIndex(size_t idx);                                                               //!< Set the index of the node
        void                                        setName(const std::string& n);                                                      //!< Set the name of this node
  		void										setNodeType(bool tip, bool root, bool interior); //SK
        void                                        setSampledAncestor(bool tf);                                                        //!< Set if the node is a sampled ancestor
        void                                        setSpeciesName(std::string const &n);                                               //!< Set the species name of this node
        void                                        setTaxon(Taxon const &t);                                                           //!< Set the taxon of this node
        void                                        setTree(Tree *t);                                                                   //!< Sets the tree pointer
        void                                        setParent(TopologyNode* p);                                                         //!< Sets the node's parent
        
        // internal helper functions
        void                                        recomputeBranchLength(void);                                                        //!< Recompute the length of this branch based on the ages.
        
    protected:
        
        // helper methods
        virtual std::string                         buildNewickString(void);                                                            //!< compute the newick string for a tree rooting at this node
        
        // protected members
        double                                      age;
        double                                      branch_length;
        std::vector<TopologyNode*>                  children;                                                                           //!< Vector holding the node's children. Note that the parent owns the children but not the other way around.
        TopologyNode*                               parent;                                                                             //!< Pointer to the parent of the node. It is a regular pointer instead of a super smart pointer to avoid loops in the reference counting.
        Tree*                                       tree;                                                                               //!< A pointer to the tree for convinience access
        Taxon                                       taxon;                                                                              //!< Taxon of the node, i.e. identifier/taxon name, plus species it comes from
        size_t                                      index;                                                                              //!< Node index
        bool                                        interior_node;
        bool                                        root_node;
        bool                                        tip_node;
        bool                                        fossil;
        bool                                        sampled_ancestor;
        
        // information for newick representation
        std::vector<std::string>                    node_comments;
        std::vector<std::string>                    branch_comments;
        
     // std::map<std::string,std::string>           nodeFields;
     // std::map<std::string,std::string>           branchFields;
    };
}

#endif
