#pragma once

#include "GNode.h"

class WhileGNode : public GNode {

public:

	// constructor
	WhileGNode(int);

	// getters
	GNode* getBeforeLoopChild();
	GNode* getAfterLoopChild();
	int getStmtNum();

	// setters
	void setBeforeLoopChild(GNode*);
	void setAfterLoopChild(GNode*);
	void setStmtNum(int);
};
