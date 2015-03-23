#pragma once

#include <string>

using namespace std;

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
	
	//TNode(string nodeType); // constructor

	//void setStmtNumber(int num); // for nodes that have stmt numbers
	//void setValue(string value); // the value for the node, example procnode should have the value = procname
	//void setParent(TNode *parent); // direct parent, not the relationship Parent
	//void setChild(TNode *child);
	//void setLeftSibling(TNode *left);
	//void setRightSibling(TNode *right);

	//string getNodeType();
	//int getStmtNumber();
	//TNode * getParent();
	//TNode * getChild();
	//TNode * getLeftSibling();
	//TNode * getRightSibling();

private:

	/*string _nodeType;
	int _stmtNumber;
	string _value;
	TNode * _parent;
	TNode * _child;
	TNode * _left;
	TNode * _right;*/

};