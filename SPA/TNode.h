#pragma once

#include <string>
#include <vector>

using namespace std;

enum NodeType {
    PROGRAM_, PROCEDURE_, STMTLST_, CALL_STMT_, WHILE_STMT_, 
    IF_STMT_, ASSIGN_STMT_, OPERATOR_, VARIABLE_, CONSTANT_
};

class TNode {

	/*

	proposed idea:
	- TNode is the superclass of all TNodes.
	- There will be specialised nodes that inherit from TNode
		* e.g. AssgNode, StmtLstNode, ProgramNode, ProcNode, VarNode
	- These nodes are known to public. Everyone can access or even create them.
	- It is the PDR's responsibility to create procedure-level nodes and below. 
		* this means everything except programNode, which is managed by the AST class.
	- Each node will have its own set of specialised properties
		* e.g. VarNode has the property _varName
	- All nodes will have the same basic abilities
		* e.g. linking parent, linking children
		* this will be fleshed out by tuesday

	*/

public:
	
	// constructors
	TNode(NodeType type); // properties
	TNode(NodeType type, const string& name); // properties

	// getters
	NodeType getNodeType(); // properties
	string getName(); // properties
	vector<TNode*>& getChildren(); // links
	TNode* getParent(); // links
	TNode* getRightSibling(); // links
	TNode* getLeftSibling(); // links

	// setters
	void setName(const string& name); // properties
	void addChild(TNode *child); // links (from linkchild)
	void setParent(TNode *parent); // links (from linkparent)
	void linkChild(TNode *child); // links
	void linkParent(TNode *parent); // links
	void linkRightSibling(TNode *right); // links
	void linkLeftSibling(TNode *left); // links

	// checks
	bool isNodeType(NodeType type); // properties
	bool hasChildren(); // links
	bool hasParent(); // links
	bool hasRightSibling(); // links
	bool hasLeftSibling(); // links

protected:
	NodeType _nodeType;
	string _nodeName;
	vector<TNode*> _children;
	TNode *_parent;
	TNode *_rightSibling;
	TNode *_leftSibling;

};