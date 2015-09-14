#pragma once

#include "CNode.h"

class ProcCNode : public CNode {

public:

	// constructor
	ProcCNode(const string&);

	// getters
	CNode* getChild();

	// setters
	void setChild(CNode*);
};
