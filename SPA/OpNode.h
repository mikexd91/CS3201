#pragma once

#include "tnode.h"

class OpNode : public TNode {
public:

	// constructor
	OpNode(void);

	// getters
	TNode& getTermNode(); // i forgot which side is term and expr
	TNode& getExprNode();

	// setters
	void linkTermNode(TNode termNode);
	void linkExprNode(TNode exprNode);
};

