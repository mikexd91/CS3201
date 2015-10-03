#include "WhileCNode.h"

WhileCNode::WhileCNode(int stmtNum)
	: CNode(WHILE_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}


CNode* WhileCNode::getBeforeLoopChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

CNode* WhileCNode::getAfterLoopChild() {
	if(this->children.size() > 1) {
		return this->children.at(1);
	}

	return NULL;
}

int WhileCNode::getStmtNum() {
	return this->startStmt;
}

void WhileCNode::setBeforeLoopChild(CNode* child) {
	this->setFirstChild(child);
}

void WhileCNode::setAfterLoopChild(CNode* child) {
	this->setSecondChild(child);
}

void WhileCNode::setStmtNum(int stmtNum) {
	this->setStartStmt(stmtNum);
	this->setEndStmt(stmtNum);
}
