#pragma once

#include "GNode.h"

class ProcGNode : public GNode {

public:

	// constructor
	ProcGNode(const string&);

	// getters
	GNode* getChild();

	// setters
	void setChild(GNode*);

};
