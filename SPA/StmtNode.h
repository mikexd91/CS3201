#pragma once

#include "tnode.h"

class StmtNode : public TNode {

public:

	// constructors
	StmtNode(NodeType type, int stmtNum);
	StmtNode(NodeType type, int stmtNum, const string &name);

	// getters
	int getStmtNum();
	
private:
	int _stmtNum;

};

