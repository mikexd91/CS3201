#pragma once

#include "tnode.h"
#include "StmtNode.h"

class StmtLstNode : public TNode {

public:

	// constructor
	StmtLstNode();
	StmtLstNode(const string &nodeName);

	// setters
	void linkStmtNode(StmtNode *stmtNode);
	
};

