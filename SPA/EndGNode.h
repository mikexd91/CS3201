#pragma once

#include "GNode.h"

class EndGNode : public GNode {

public:

	EndGNode();

	// getters
	GNode* getParent();

	// setters
	void setParent(GNode*);
};
