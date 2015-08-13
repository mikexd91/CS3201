#pragma once

#include "TNode.h"

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

