/**
 * @file
 * This file contains the declaration of our tree interface, a light-weight class 
 * that holds the pointer to the root node of a tree and provides some convinience functions.
 *
 * The derived classes differ mainly in which type of topology nodes they store.
 *
 * @todo: If the derived classes only differ in the topology nodes, then we could use templates for the derived classes instead!
 *
 * @brief Declaration of the Tree
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: Tree.h 1651 2012-07-05 14:47:08Z hoehna $
 */

#ifndef Tree_H
#define Tree_H

#include "RbBoolean.h"
#include "Cloneable.h"
#include "MemberObject.h"
#include "Serializable.h"
#include "TreeChangeEventHandler.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class TopologyNode;

    class Tree : public Cloneable, public MemberObject<double>, public MemberObject<int>, public MemberObject<Boolean>, public Serializable {
    
    public:
        Tree(void);                                                                                                                                             //!< Default constructor
        Tree(const Tree& t);                                                                                                                                    //!< Copy constructor

        virtual                                            ~Tree(void);                                                                                         //!< Destructor

        Tree&                                               operator=(const Tree& t);
        
        // overloaded operators
        bool                                                operator==(const Tree &t) const;
        bool                                                operator!=(const Tree &t) const;
        bool                                                operator<(const Tree &t) const;
        bool                                                operator<=(const Tree &t) const;
        
        // virtual basic utility functions
        virtual Tree*                                       clone(void) const;                                                                                  //!< Clone object
        virtual void                                        initFromString(const std::string &s);                                                               //!< Serialize the object from a string

        // public Tree methods
        void                                                addBranchParameter(const std::string &n, const std::vector<double> &p, bool io);
        void                                                addNodeParameter(const std::string &n, const std::vector<double> &p, bool io);
		void                                                addNodeParameter(const std::string &n, const std::vector<std::string*> &p, bool io);
        void                                                clearParameters(void);                                                                              //!< Clear both the current node and branch parameters
        void                                                clearBranchParameters(void);
		void                                                clearNodeParameters(void);
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, int &rv) const;        //!< Map the member methods to internal function calls
        void                                                executeMethod(const std::string &n, const std::vector<const DagNode*> &args, Boolean &rv) const;    //!< Map the member methods to internal function calls
        std::vector<Taxon>                                  getFossilTaxa() const;                                                                                    //!< Get all the taxa in the tree
        std::string                                         getNewickRepresentation() const;                                                                    //!< Get the newick representation of this Tree
        TopologyNode&                                       getNode(size_t idx);                                                                                //!< Get the node at index
        const TopologyNode&                                 getNode(size_t idx) const;                                                                          //!< Get the node at index
        const std::vector<TopologyNode*>&                   getNodes(void) const;                                                                               //!< Get a pointer to the nodes in the Tree
        size_t                                              getNumberOfInteriorNodes(void) const;                                                               //!< Get the number of nodes in the Tree
        size_t                                              getNumberOfNodes(void) const;                                                                       //!< Get the number of nodes in the Tree
        size_t                                              getNumberOfTips(void) const;                                                                        //!< Get the number of tip nodes in the Tree
        const TopologyNode&                                 getInteriorNode(size_t indx) const;                                                                 //!< Get a pointer to interior node i
        std::string                                         getPlainNewickRepresentation() const;                                                                    //!< Get the newick representation of this Tree
        TopologyNode&                                       getRoot(void);                                                                                      //!< Get a pointer to the root node of the Tree
        const TopologyNode&                                 getRoot(void) const;                                                                                //!< Get a pointer to the root node of the Tree
        std::vector<std::string>                            getSpeciesNames() const;                                                                            //!< Get all the species represented in the tree
        std::vector<Taxon>                                  getTaxa() const;                                                                                    //!< Get all the taxa in the tree
        size_t                                              getTipIndex(const std::string &name) const;
        std::vector<std::string>                            getTipNames() const;
        TopologyNode&                                       getTipNode(size_t indx);                                                                            //!< Get a pointer to tip node i
        const TopologyNode&                                 getTipNode(size_t indx) const;                                                                      //!< Get a pointer to tip node i
        TopologyNode&                                       getTipNodeWithName(const std::string &n);                                                           //!< Get a pointer to tip node i
        const TopologyNode&                                 getTipNodeWithName(const std::string &n) const;                                                     //!< Get a pointer to tip node i
        std::vector<TopologyNode*>                          getTipNodesWithSpeciesName(const std::string &n);                                                   //!< Get a pointer to tip node i
        double                                              getTmrca(const TopologyNode &n);
        double                                              getTmrca(const Clade &c);
        double                                              getTmrca(const std::vector<Taxon> &t);
        TreeChangeEventHandler&                             getTreeChangeEventHandler(void) const;                                                              //!< Get the change-event handler for this tree
        double                                              getTreeLength(void) const;
        bool                                                hasSameTopology( const Tree &t ) const;                                                             //!< Has this tree the same topology?
        bool                                                isBinary(void) const;                                                                               //!< Is the Tree rooted
        bool                                                isBroken(void) const;                                                                               //!< Is this tree ultrametric?
        bool                                                isRooted(void) const;                                                                               //!< Is the Tree rooted
        bool                                                isUltrametric(void) const;                                                                          //!< Is this tree ultrametric?
        void                                                makeInternalNodesBifurcating(void);                                                                 //!< Make all the internal nodes bifurcating.
        void                                                orderNodesByIndex();
        void                                                reroot(const std::string &outgroup);                                                                //!< Re-root the tree with the given outgroup
        void                                                reroot(TopologyNode &n);
        void                                                setRoot(TopologyNode* r, bool resetIndex=true);                                                     //!< Set the root and bootstrap the Tree from it
        void                                                setRooted(bool tf);
        
        TopologyNode&                                       reverseParentChild(TopologyNode &n);                                    //!< Reverse the parent child relationship.

    protected:
        

        // protected members
        mutable TreeChangeEventHandler                      changeEventHandler;

//    private:
        
        void                                                fillNodesByPhylogeneticTraversal(TopologyNode* node);               //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        
        

        // private members
        TopologyNode*                                       root;
        std::vector<TopologyNode*>                          nodes;                                                                  //!< Vector of pointers to all nodes
        bool                                                binary;                                                                 //!< Is the BranchLengthTree binary?
        bool                                                rooted;
        size_t                                              numTips;
        size_t                                              num_nodes;

    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Tree& x);                                         //!< Overloaded output operator

}

#endif

