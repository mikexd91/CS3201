#pragma once

#include "tnode.h"
#include "StmtLstNode.h"

class ProcNode : public TNode {

public:

	// constructor
	ProcNode(const string &name);

	// getters
	StmtLstNode* getStmtLstNode(); // children[0]

	// setters
	void linkStmtLstNode(StmtLstNode *stmtlst);
	
	
private:

	

};

