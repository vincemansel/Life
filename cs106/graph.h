/*
 * File: graph.h
 * -------------
 * Extended interface for the graph package.  This interface exports
 * both a low-level graphT type and a parameterized Graph class.
 */

#ifndef _graph_h
#define _graph_h

#include "set.h"
#include "map.h"

struct nodeT;     /* Forward references to these two types so  */
struct arcT;      /* that the C++ compiler can recognize them. */

/*
 * Type: graphT
 * ------------
 * This type represents a graph and consists of two sets -- a set
 * of nodes and a set of arcs -- along with a map that creates an
 * association between names and nodes.
 */

struct graphT {
	Set<nodeT *> nodes;
	Set<arcT *> arcs;
	Map<nodeT *> nodeMap;
};

/*
 * Type: nodeT
 * -----------
 * This type is the supplied type for a node in a graph.  Clients
 * may substitute their own type, as described in the notes for
 * the Graph class.
 */

struct nodeT {
	string name;
	Set<arcT *> arcs;
};

/*
 * Type: arcT
 * ----------
 * This type is the supplied type for an arc in a graph.  Clients
 * may substitute their own type, as described in the notes for
 * the Graph class.
 */

struct arcT {
	nodeT *start;
	nodeT *finish;
	double cost;
};

/*
 * Class: Graph<NodeType,ArcType>
 * ------------------------------
 * This class represents a graph with the specified node and arc
 * types.  The NodeType and ArcType parameters indicate the record
 * or object types used for nodes and arcs, respectively.  These
 * types can contain any fields or methods required by the client,
 * but must also contain the following public fields required by
 * the Graph package itself:
 *
 * The NodeType definition must include:
 *   - A string field called name
 *   - A Set<ArcType *> field called arcs
 *
 * The ArcType definition must include:
 *   - A NodeType * field called start
 *   - A NodeType * field called finish
 */

template <typename NodeType,typename ArcType>
class Graph {

public:

/*
 * Constructor: Graph
 * Usage: Graph<NodeType,ArcType> g;
 * ---------------------------------
 * Declares a new Graph object named g.
 */
	Graph();

/*
 * Destructor: ~Graph
 * Usually called implicitly
 * -------------------------
 * Frees the internal storage allocated for the nodes and
 * arcs in the graph.
 */
	~Graph();

/*
 * Method: clear
 * Usage: g.clear();
 * -----------------
 * Frees the storage for all nodes and arcs in the graph and
 * reinitializes the graph to be empty.
 */
	void clear();

/*
 * Method: addNode
 * Usage: g.addNode(name);
 *        g.addNode(node);
 * -----------------------
 * Adds a node to the graph.  The first version of this method
 * creates a new node of the appropriate type and initializes its
 * fields; the second assumes that the client has already created
 * the node and simply adds it to the graph.  Both versions of this
 * method return a pointer to the node in case the client needs to
 * capture this value.
 */
	NodeType *addNode(string name);
	NodeType *addNode(NodeType *node);

/*
 * Method: addArc
 * Usage: g.addArc(s1, s2);
 *        g.addArc(n1, n2);
 *        g.addArc(arc);
 * ------------------------
 * Adds an arc to the graph.  The endpoints of the arc can be
 * specified either as strings indicating the names of the nodes
 * or as pointers to the node structures.  Alternatively, the
 * client can create the arc structure explicitly and pass that
 * pointer to the addArc method.  All three of these versions
 * return a pointer to the arc in case the client needs to
 * capture this value.
 */
	ArcType *addArc(string s1, string s2);
	ArcType *addArc(NodeType *n1, NodeType *n2);
	ArcType *addArc(ArcType *arc);

/*
 * Method: isConnected
 * Usage: if (g.isConnected(n1, n2)) . . .
 *        if (g.isConnected(s1, s2)) . . .
 * ---------------------------------------
 * Returns true if the graph contains an arc from n1 to n2.  As
 * in the addArc method, nodes can be specified either as node
 * pointers or by name.
 */
	bool isConnected(NodeType *n1, NodeType *n2);
	bool isConnected(string s1, string s2);

/*
 * Method: getNode
 * Usage: NodeType *node = g.getNode(name);
 * ----------------------------------------
 * Looks up a node in the name table attached to the graph and
 * returns a pointer to that node.  If no node with the specified
 * name exists, getNode returns NULL.
 */
	NodeType *getNode(string name);

/*
 * Method: getNodeSet
 * Usage: foreach (NodeType *node in g.getNodeSet()) . . .
 * -------------------------------------------------------
 * Returns the set of all nodes in the graph.  This method returns
 * a reference to the set rather than a copy of the set, which makes
 * it possible to iterate over its elements.
 */
	Set<NodeType *> & getNodeSet();

/*
 * Method: getArcSet
 * Usage: foreach (ArcType *arc in g.getArcSet()) . . .
 *        foreach (ArcType *arc in g.getArcSet(node)) . . .
 * --------------------------------------------------------
 * Returns the set of all arcs in the graph or, in the second form,
 * the arcs that start at the specified node.  This method returns
 * a reference to the set rather than a copy of the set, which makes
 * it possible to iterate over its elements.
 */
	Set<ArcType *> & getArcSet();
	Set<ArcType *> & getArcSet(NodeType *node);

private:

#include "graphpriv.h"

};

#include "graphpriv.cpp"

#endif
