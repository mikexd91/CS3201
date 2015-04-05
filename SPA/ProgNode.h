#pragma once

#include "TNode.h"
#include "ProcNode.h"

class ProgNode : public TNode {
public:

	// constructors
	ProgNode();
	ProgNode(const string &name);

	// getters
	vector<TNode*>& getProcNodes();
	bool contains(const string& procName);
	ProcNode* getProcNode(const string& procName);

	// setters
	void linkProcNode(ProcNode *procNode); 
};

