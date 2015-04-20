#pragma once

#include "TNode.h"
#include "VarNode.h"
#include "StmtLstNode.h"
#include "StmtNode.h"

class WhileNode : public StmtNode {

public:

	// constructor
	WhileNode(int stmtNum);

	// getters
	VarNode* getVarNode();
	StmtLstNode* getStmtLstNode();

	// setters
	void linkVarNode(VarNode *varNode);
	void linkStmtLstNode(StmtLstNode *stmtLst);

};

