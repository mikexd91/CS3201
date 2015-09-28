#include "DummyCNode.h"

DummyCNode::DummyCNode()
	: CNode(DUMMY_) {
}

int DummyCNode::getIfParentStmt() {
	return this->startStmt;
}

int DummyCNode::getElseParentStmt() {
	return this->endStmt;
}

void DummyCNode::setIfParentStmt(int stmtNum) {
	this->startStmt = stmtNum;
}

void DummyCNode::setElseParentStmt(int stmtNum) {
	this->endStmt = stmtNum;
}
