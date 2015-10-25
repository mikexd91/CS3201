#pragma once
#include "AssgGNode.h"

AssgGNode::AssgGNode(int stmtNum)
	: GNode(ASSIGN_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}

GNode* AssgGNode::getChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

void AssgGNode::setChild(GNode* child) {
	this->setFirstChild(child);
}

bool AssgGNode::hasMoreThanOneStmt() {
	if(this->startStmt == this->endStmt) {
		return false;
	}

	return true;
}
