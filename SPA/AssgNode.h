#pragma once

#include "StmtNode.h"
#include "VarNode.h"

class AssgNode : public StmtNode {

public:

	// constructors
	AssgNode(int stmtNum);

	// getters
	VarNode* getVarNode(); // just children[0] and children[1]
	TNode* getExprNode();

	// setters
	void linkVarNode(VarNode *varNode);
	void linkExprNode(TNode *exprNode);

};

