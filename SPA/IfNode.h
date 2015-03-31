#pragma once

#include "TNode.h"
#include "VarNode.h"
#include "StmtLstNode.h"
#include "StmtNode.h"

class IfNode : public StmtNode {

public:
	// constructors
	IfNode(int stmtNum);

	// getters
	VarNode* getVarNode(); // children[0]
	StmtLstNode* getThenStmtLstNode(); // children[1]
	StmtLstNode* getElseStmtLstNode(); // children[2]

	// setters
	void linkVarNode(VarNode *varNode);
	void linkThenStmtLstNode(StmtLstNode *thenNode);
	void linkElseStmtLstNode(StmtLstNode *elseNode);

};

