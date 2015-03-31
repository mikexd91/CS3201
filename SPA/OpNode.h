#pragma once

#include "tnode.h"

class OpNode : public TNode {
public:

	// constructor
	OpNode(const string &varName);

	// getters
	TNode* getLeftNode(); 
	TNode* getRightNode();

	// setters
	void linkLeftNode(TNode *left);
	void linkRightNode(TNode *right);
};

