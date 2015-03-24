#pragma once

#include "tnode.h"
#include "StmtLstNode.h"

class ProcNode : public TNode {

public:

	// constructor
	ProcNode(void);

	// getters
	StmtLstNode& getStmtLst(); // children[0]

	// setters
	void linkStmtLstNode(StmtLstNode stmtlst);
	
	
private:

	

};

