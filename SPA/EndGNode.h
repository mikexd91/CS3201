#pragma once

#include "GNode.h"

class EndGNode : public GNode {

public:

	EndGNode();

	// getters
	GNode* getParent();
	GNode* getProcNode();

	// setters
	void setParent(GNode*);
	void setProcNode(GNode*);

private:

	GNode* procNode;
};
