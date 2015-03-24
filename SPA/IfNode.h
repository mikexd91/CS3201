#pragma once

#include "tnode.h"
#include "VarNode.h"
#include "StmtLstNode.h"

class IfNode : public TNode {

public:
	// constructors
	IfNode(void);

	// getters
	VarNode& getVarNode(); // children[0]
	StmtLstNode& getThenStmtLst(); // children[1]
	StmtLstNode& getElseStmtLst(); // children[2]

	// setters
	void linkVarNode(VarNode &varNode);
	void linkThenStmtLst(StmtLstNode &thenNode);
	void linkElseStmtLst(StmtLstNode &elseNode);

};

