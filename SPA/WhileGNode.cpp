#include "WhileGNode.h"

WhileGNode::WhileGNode(int stmtNum)
	: GNode(WHILE_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}


GNode* WhileGNode::getBeforeLoopChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

GNode* WhileGNode::getAfterLoopChild() {
	if(this->children.size() > 1) {
		return this->children.at(1);
	}

	return NULL;
}

int WhileGNode::getStmtNum() {
	return this->startStmt;
}

void WhileGNode::setBeforeLoopChild(GNode* child) {
	this->setFirstChild(child);
}

void WhileGNode::setAfterLoopChild(GNode* child) {
	this->setSecondChild(child);
}

void WhileGNode::setStmtNum(int stmtNum) {
	this->setStartStmt(stmtNum);
	this->setEndStmt(stmtNum);
}
