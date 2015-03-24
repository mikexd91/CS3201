#pragma once

#include "tnode.h"
#include "StmtNode.h"

class CallNode : public StmtNode {
public:

	// constructor
	CallNode(int stmtNum, const string &nodeName);
};

