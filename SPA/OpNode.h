#pragma once

#include "tnode.h"

class OpNode : public TNode {
public:

	// constructor
	OpNode(void);

	// getters
	TNode* getLeftNode(); 
	TNode* getRightNode();

	// setters
	void linkLeftNode(TNode *left);
	void linkRightNode(TNode *right);
};

