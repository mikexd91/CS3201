#pragma once

#include "GNode.h"

class IfGNode : public GNode {

public:

	// constructor
	IfGNode(int);

	// getters
	GNode* getThenChild();
	GNode* getElseChild();
	GNode* getExit();
	int getStmtNum();

	// setters
	void setThenChild(GNode*);
	void setElseChild(GNode*);
	void setExit(GNode*);
	void setStmtNum(int);

private:

	GNode* exit;
};
