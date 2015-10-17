#pragma once

#include "GNode.h"

class AssgGNode : public GNode {

public:

	// constructor
	AssgGNode(int);

	// getters
	GNode* getChild();

	// setters
	void setChild(GNode*);

	// validity checks
	bool hasMoreThanOneStmt(); 		// checks to see if have more than one stmt
};
