#pragma once

#include "CNode.h"

class AssgCallCNode : public CNode {

public:

	// constructor
	AssgCallCNode(int);

	// getters
	CNode* getChild();

	// setters
	void setChild(CNode*);

	// validity checks
	bool hasMoreThanOneStmt(); 		// checks to see if have more than one stmt
};
