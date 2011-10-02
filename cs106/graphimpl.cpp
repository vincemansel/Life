/*
 * File: graphimpl.cpp
 * -------------------
 * This file provides the private implementation for the Graph class.
 * As with all template classes, the C++ compiler must have access to
 * this code when it processes the interface.  Putting this code in a
 * separate file means that the client is not forced to confront these
 * while reading the interface.
 */

/************************************************************************/
/*                            IMPORTANT NOTE                            */
/*                                                                      */
/* Next year, the Graph class defined by this interface will be part    */
/* the CS106 class library, which means that clients will have to use   */
/* this code "as is" and will not be able to extend or change it to     */
/* suit the requirements of a particular application.  In anticipation  */
/* of this change, you are not allowed to change the contents of this   */
/* file as you implement the Pathfinder application.                    */
/************************************************************************/

#ifdef _graph_h

#include "genlib.h"

/*
 * Implementation notes: NodeCompare, ArcCompare
 * ---------------------------------------------
 * These functions are the comparison functions used for nodes and arcs,
 * respectively.  Their purpose is to ensure that the sets containing
 * nodes and arcs process their elements in a predictable order.  Nodes
 * are processed in alphabetical order by node name; arcs are compared
 * in much the same way, looking first at the start node and then
 * continuing on to look at the finish node if the start nodes match.
 * These comparison functions, however, return 0 only if the arguments
 * are identical, in the sense that they are at the same address.  If
 * two distinct arcs, for example, connect the same pair of nodes (which
 * is perfectly legal in the graph abstraction and can be used to
 * represent multiple modes of travel between two nodes), those arcs
 * are not the same.  The comparison function must therefore return
 * a nonzero value to ensure that both arcs can coexist in the arc set.
 */

template <typename NodeType>
int NodeCompare(NodeType *n1, NodeType *n2) {
	if (n1 == n2) return 0;
	if (n1->name == n2->name) return (n1 < n2) ? -1 : +1;
	return (n1->name < n2->name) ? -1 : +1;
}

template <typename NodeType,typename ArcType>
int ArcCompare(ArcType *a1, ArcType *a2) {
	if (a1 == a2) return 0;
	NodeType *n1 = a1->start;
	NodeType *n2 = a2->start;
	int cmp = NodeCompare(n1, n2);
	if (cmp != 0) return cmp;
	n1 = a1->finish;
	n2 = a2->finish;
	cmp = NodeCompare(n1, n2);
	if (cmp != 0) return cmp;
	return (a1 < a2) ? -1 : +1;
}

/*
 * Implementation notes: Graph constructor
 * ---------------------------------------
 * Even though the body of the Graph constructor is empty, important
 * work is done by the initializers, which ensure that the nodes and
 * arcs set are given the correct comparison functions.
 */

template <typename NodeType,typename ArcType>
Graph<NodeType,ArcType>::Graph() : nodes(NodeCompare<NodeType>),
                                   arcs(ArcCompare<NodeType,ArcType>) {
	/* Empty body, although important work is done in the initializers */
}

/*
 * Implementation notes: Graph destructor
 * --------------------------------------
 * The destructor must free all heap storage used by this graph to
 * represent the nodes and arcs.  The clear metho must also reclaim
 * this memory, which means that the destructor can simply call
 * clear to do the work.
 */

template <typename NodeType,typename ArcType>
Graph<NodeType,ArcType>::~Graph() {
	clear();
}

/*
 * Implementation notes: clear
 * ---------------------------
 * The implementation of clear first frees the nodes and arcs in
 * their respective sets and then uses the Set class clear method
 * to ensure that these sets are empty.
 */

template <typename NodeType,typename ArcType>
void Graph<NodeType,ArcType>::clear() {
	foreach (NodeType *node in nodes) {
		delete node;
	}
	foreach (ArcType *arc in arcs) {
		delete arc;
	}
	arcs.clear();
	nodes.clear();
	nodeMap.clear();
}

/*
 * Implementation notes: addNode
 * -----------------------------
 * The addNode method appears in two forms: one that creates a node
 * from its name and one that assumes that the client has created
 * the new node.  In each case, the implementation must add the node
 * the set of nodes for the graph and add the name-to-node association
 * to the node map.
 */

template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::addNode(string name) {
	NodeType *node = new NodeType();
	node->arcs = Set<ArcType *>(ArcCompare<NodeType,ArcType>);
	node->name = name;
	return addNode(node);
}

template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::addNode(NodeType *node) {
	nodes.add(node);
	nodeMap[node->name] = node;
	return node;
}

/*
 * Implementation notes: addArc
 * ----------------------------
 * The addNode method appears in three forms, as described in the
 * interface.  The code for each form of the method, however, is
 * quite straightforward.
 */

template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(string s1, string s2) {
	return addArc(getNode(s1), getNode(s2));
}

template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(NodeType *n1, NodeType *n2) {
	ArcType *arc = new ArcType();
	arc->start = n1;
	arc->finish = n2;
	return addArc(arc);
}

template <typename NodeType,typename ArcType>
ArcType *Graph<NodeType,ArcType>::addArc(ArcType *arc) {
	arc->start->arcs.add(arc);
	arcs.add(arc);
	return arc;
}

/*
 * Implementation notes: isConnected
 * ---------------------------------
 * Node n1 is connected to n2 if any of the arcs leaving n1 finish at n2.
 * The two versions of this method allow nodes to be specified either as
 * node pointers or by name.
 */

template <typename NodeType,typename ArcType>
bool Graph<NodeType,ArcType>::isConnected(NodeType *n1, NodeType *n2) {
	foreach (ArcType *arc in n1->arcs) {
		if (arc->finish == n2) return true;
	}
	return false;
}

template <typename NodeType,typename ArcType>
bool Graph<NodeType,ArcType>::isConnected(string s1, string s2) {
	return isConnected(getNode(s1), getNode(s2));
}

/*
 * Implementation notes: getNode
 * -----------------------------
 * This code simply looks up the name of the node in the node map.
 * As described in the interface, getNode returns NULL if the name
 * does not appear.
 */

template <typename NodeType,typename ArcType>
NodeType *Graph<NodeType,ArcType>::getNode(string name) {
	if (nodeMap.containsKey(name)) return nodeMap[name];
	return NULL;
}

/*
 * Implementation notes: getNodeSet, getArcSet
 * -------------------------------------------
 * These methods simply return the set requested by the client.  The
 * sets are returned by reference for efficiency, because doing so
 * eliminates the need to copy the set.
 */

template <typename NodeType,typename ArcType>
Set<NodeType *> & Graph<NodeType,ArcType>::getNodeSet() {
	return nodes;
}

template <typename NodeType,typename ArcType>
Set<ArcType *> & Graph<NodeType,ArcType>::getArcSet() {
	return arcs;
}

template <typename NodeType,typename ArcType>
Set<ArcType *> & Graph<NodeType,ArcType>::getArcSet(NodeType *node) {
	return node->arcs;
}

/*
 * Implementation notes: operator=, copy constructor
 * -------------------------------------------------
 * These methods ensure that copying a graph creates an entirely new
 * parallel structure of nodes and arcs.
 */

template <typename NodeType,typename ArcType>
const Graph<NodeType,ArcType> &Graph<NodeType,ArcType>::operator=(const Graph & rhs) {
	if (this != &rhs) {
		clear();
		copyInternalData(rhs);
	}
	return *this;
}

template <typename NodeType,typename ArcType>
Graph<NodeType,ArcType>::Graph(const Graph & rhs) {
	copyInternalData(rhs);
}

/* Private method: copyInternalData
 * --------------------------------
 * Common code factored out of the copy constructor and operator= to
 * copy the contents from the other graph.
 */

template <typename NodeType,typename ArcType>
void Graph<NodeType,ArcType>::copyInternalData(const Graph & other) {
	Map<NodeType *> nodeTable;
	Set<NodeType *>nodes = other.nodes;
	foreach (NodeType *oldNode in nodes) {
		NodeType *newNode = new NodeType();
		*newNode = *oldNode;
		newNode->arcs.clear();
		addNode(newNode);
		nodeTable[nodeAddressToString(oldNode)] = newNode;
	}
	Set<ArcType *> arcs = other.arcs;
	foreach (ArcType *oldArc in arcs) {
		ArcType *newArc = new ArcType();
		*newArc = *oldArc;
		newArc->start = nodeTable[nodeAddressToString(oldArc->start)];
		newArc->finish = nodeTable[nodeAddressToString(oldArc->finish)];
		addArc(newArc);
	}
}

template <typename NodeType,typename ArcType>
string Graph<NodeType,ArcType>::nodeAddressToString(NodeType *node) {
	unsigned long addr = (unsigned long) node;
	string str;
	while (addr != 0) {
		str = "0123456789ABCDEF"[addr & 0xF] + str;
		addr >>= 4;
	}
	return str;
}

#endif
