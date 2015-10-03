#include "IfCNode.h"

IfCNode::IfCNode(int stmtNum)
	: CNode(IF_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}

CNode* IfCNode::getThenChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

CNode* IfCNode::getElseChild() {
	if(this->children.size() > 1) {
		return this->children.at(1);
	}

	return NULL;
}

int IfCNode::getStmtNum() {
	return this->getStartStmt();
}

void IfCNode::setThenChild(CNode* child) {
	this->setFirstChild(child);
}

void IfCNode::setElseChild(CNode* child) {
	this->setSecondChild(child);
}

void IfCNode::setStmtNum(int stmtNum) {
	this->setStartStmt(stmtNum);
	this->setEndStmt(stmtNum);
}
