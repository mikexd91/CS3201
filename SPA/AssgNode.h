#pragma once

#include "tnode.h"
#include "VarNode.h"

class AssgNode : public TNode {

public:

	// constructor
	AssgNode();
	AssgNode(const string& name, int stmtNum);

	// getters
	int getStmtNum();
	VarNode getVarNode(); // just children[0] and children[1]
	TNode getExprNode();

	// setters
	void linkVarNode(VarNode& varNode);
	void linkExprNode(TNode& exprNode);

private:
	int _stmtNum;
};

