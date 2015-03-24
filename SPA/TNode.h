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
	TNode();
	TNode(NodeType type);
	TNode(NodeType type, const string& name);

	// getters
	NodeType getNodeType();
	string getNodeName(); // the name that comes before the colon e.g. else:stmtlst
	vector<TNode>& getChildren();
	TNode& getParent();
	TNode& getRightSibling();
	TNode& getLeftSibling();

	// setters
	void setName(const string& name);
	void linkChild(TNode& child);
	void linkParent(TNode& parent);
	void linkRightSibling(TNode& right);
	void linkLeftSibling(TNode& left);

	// checks
	bool isNodeType(NodeType type);
	bool hasChildren();
	bool hasParent();
	bool hasRightSibling();
	bool hasLeftSibling();

private:

	NodeType _nodeType;
	string _nodeName;
	vector<TNode> &_children;
	TNode &parent;

	/*string _nodeType;
	int _stmtNumber;
	string _value;
	TNode * _parent;
	TNode * _child;
	TNode * _left;
	TNode * _right;*/

};