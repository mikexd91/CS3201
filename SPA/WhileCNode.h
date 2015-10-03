#pragma once

#include "CNode.h"

class WhileCNode : public CNode {

public:

	// constructor
	WhileCNode(int);

	// getters
	CNode* getBeforeLoopChild();
	CNode* getAfterLoopChild();
	int getStmtNum();

	// setters
	void setBeforeLoopChild(CNode*);
	void setAfterLoopChild(CNode*);
	void setStmtNum(int);
};
