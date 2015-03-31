#pragma once

#include "TNode.h"
#include "StmtLstNode.h"

class ProcNode : public TNode {

public:

	// constructor
	ProcNode(const string &name);

	// getters
	StmtLstNode* getStmtLst(); // children[0]

	// setters
	void linkStmtLstNode(StmtLstNode *stmtlst);
	
	
private:

	

};

