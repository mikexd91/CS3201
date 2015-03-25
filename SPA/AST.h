#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ProgNode.h"

using namespace std;

class AST {

	/*
	
	seems like the AST class is not useful for actually building the branches of the tree.
	all the linking can actually be done at node level.
	there is no need for the AST class to be involved while linking nodes,
	since both nodes are actually fully specified.
	the tree structure still holds based on the node-level linking alone.
	the pdr knows the state of the tree (e.g. which stmtlst to insert stmts into).

	seems like even in query, the only time we need to use the tree is when we do pattern,
	and even then we can directly access the node concerned by referencing the tables,
	so the only traversing is done at the subtree level, which is in fact node-level.

	proposed idea:
	- initialise AST = AST creates the root node and holds on to it.
		* root node is not accessible to outsiders.
	- pdr only works on procedure level. it creates procnodes by itself, and all the necessary other nodes.
		* after parsing finish a procedure, it then adds the procnode (basically a subtree) to the AST.
	- upon receiving the complete procnode, AST will add it to the childList of the root node.
		* this means that AST only eye-power and do the final connection of procedures to program.
		* all hard work building the procedures is done by the pdr.
	- when querying, we access the nodes directly via the tables. 
		* is the AST not needed at all?

	*/

public:


	// getters
	static AST* getInstance(const string &progName);

	// setters
	void addProcNode(ProcNode *procNode); // adds the procedure to the program

private:
	// constructor: creates tree rooted at programNode (named programName)
	AST(const string &progName);
	
	ProgNode *_rootNode;
	static AST *_instance;

};