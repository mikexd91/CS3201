#pragma once

#include "GNode.h"
#include "DummyGNode.h"

class IfGNode : public GNode {

public:

	// constructor
	IfGNode(int);

	// getters
	GNode* getThenChild();
	GNode* getElseChild();
	DummyGNode* getExit();
	int getStmtNum();

	// setters
	void setThenChild(GNode*);
	void setElseChild(GNode*);
	void setExit(DummyGNode*);
	void setStmtNum(int);

private:

	DummyGNode* exit;
};
