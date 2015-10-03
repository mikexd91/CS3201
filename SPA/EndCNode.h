#pragma once

#include "CNode.h"

class EndCNode : public CNode {

public:

	EndCNode();

	// getters
	CNode* getParent();

	// setters
	void setParent(CNode*);
};
