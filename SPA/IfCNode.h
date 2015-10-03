#pragma once

#include "CNode.h"

class IfCNode : public CNode {

public:

	// constructor
	IfCNode(int);

	// getters
	CNode* getThenChild();
	CNode* getElseChild();
	int getStmtNum();

	// setters
	void setThenChild(CNode*);
	void setElseChild(CNode*);
	void setStmtNum(int);
};
