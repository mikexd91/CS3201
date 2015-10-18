#include "DummyGNode.h"

using boost::unordered_set;

DummyGNode::DummyGNode()
	: GNode(DUMMY_) {
}

int DummyGNode::getIfParentStmt() {
	return this->startStmt;
}

int DummyGNode::getElseParentStmt() {
	return this->endStmt;
}

GNode* DummyGNode::getEntrance() {
	return this->entrance;
}

unordered_set<int> DummyGNode::getPrevStmts() {
	return this->prevStmts;
}

void DummyGNode::setIfParentStmt(int stmtNum) {
	this->startStmt = stmtNum;
}

void DummyGNode::setElseParentStmt(int stmtNum) {
	this->endStmt = stmtNum;
}

void DummyGNode::setEntrance(GNode* entrance) {
	this->entrance = entrance;
}

void DummyGNode::setPrevStmts(unordered_set<int> prevStmts) {
	this->prevStmts = prevStmts;
}