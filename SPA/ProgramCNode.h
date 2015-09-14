#pragma once

#include "CNode.h"
#include "ProcCNode.h"

class ProgramCNode : public CNode {
public:

	ProgramCNode();
	ProgramCNode(const string&);
	~ProgramCNode();

	// getters
	vector<CNode*>& getProcedures();
	bool contains(const string&);
	ProcCNode* getProcNode(const string&);

	// setters
	void linkProcNode(ProcCNode*);

};
