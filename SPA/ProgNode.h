#pragma once

#include "TNode.h"
#include "ProcNode.h"

class ProgNode : public TNode {
public:

	// constructors
	ProgNode(const string &name);

	// getters
	vector<TNode*>& getProcNodes(); // simply replace the methods for getchildren

	// setters
	void linkProcNode(ProcNode *procNode); 
};

