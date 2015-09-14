#include "AssgCallCNode.h"

AssgCallCNode::AssgCallCNode(int stmtNum)
	: CNode(ASSIGN_CALL_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}

CNode* AssgCallCNode::getChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

void AssgCallCNode::setChild(CNode* child) {
	this->setFirstChild(child);
}

bool AssgCallCNode::hasMoreThanOneStmt() {
	if(this->startStmt == this->endStmt) {
		return false;
	}

	return true;
}
