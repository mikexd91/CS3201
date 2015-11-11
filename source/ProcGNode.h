#pragma once

#include "GNode.h"

class ProcGNode : public GNode {

public:

	// constructor
	ProcGNode(const string&);

	// getters
	GNode* getChild();
	GNode* getEndNode();

	// setters
	void setChild(GNode*);
	void setEndNode(GNode*);

private:

	GNode* endNode;
};
