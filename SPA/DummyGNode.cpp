#include "DummyGNode.h"

DummyGNode::DummyGNode()
	: GNode(DUMMY_) {
}

int DummyGNode::getIfParentStmt() {
	return this->startStmt;
}

int DummyGNode::getElseParentStmt() {
	return this->endStmt;
}

void DummyGNode::setIfParentStmt(int stmtNum) {
	this->startStmt = stmtNum;
}

void DummyGNode::setElseParentStmt(int stmtNum) {
	this->endStmt = stmtNum;
}
