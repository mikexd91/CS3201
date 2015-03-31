#pragma once

#include "tnode.h"
#include "ProcNode.h"

class ProgNode : public TNode {
public:

	// constructors
	ProgNode(const string &name);

	// getters
	vector<TNode*>& getProcNodes(); 

	// setters
	void linkProcNode(ProcNode *procNode); 
};

