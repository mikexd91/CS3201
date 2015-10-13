#pragma once

#include "GNode.h"

class IfGNode : public GNode {

public:

	// constructor
	IfGNode(int);

	// getters
	GNode* getThenChild();
	GNode* getElseChild();
	int getStmtNum();

	// setters
	void setThenChild(GNode*);
	void setElseChild(GNode*);
	void setStmtNum(int);
};
