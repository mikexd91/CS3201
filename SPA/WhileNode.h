#pragma once

#include "tnode.h"
#include "VarNode.h"
#include "StmtLstNode.h"

class WhileNode :
	public TNode
{
public:

	// constructor
	WhileNode();

	// getters
	VarNode& getVarNode();
	StmtLstNode& getStmtLstNode();

	// setters
	void linkVarNode(VarNode varNode);
	void linkStmtLstNode(StmtLstNode stmtLst);

};

