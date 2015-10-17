#include "IfGNode.h"

IfGNode::IfGNode(int stmtNum)
	: GNode(IF_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}

GNode* IfGNode::getThenChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

GNode* IfGNode::getElseChild() {
	if(this->children.size() > 1) {
		return this->children.at(1);
	}

	return NULL;
}

GNode* IfGNode::getExit() {
	return this->exit;
}

int IfGNode::getStmtNum() {
	return this->getStartStmt();
}

void IfGNode::setThenChild(GNode* child) {
	this->setFirstChild(child);
}

void IfGNode::setElseChild(GNode* child) {
	this->setSecondChild(child);
}

void IfGNode::setExit(GNode* exit) {
	this->exit = exit;
}

void IfGNode::setStmtNum(int stmtNum) {
	this->setStartStmt(stmtNum);
	this->setEndStmt(stmtNum);
}
