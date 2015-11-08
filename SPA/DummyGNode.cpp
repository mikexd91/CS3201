#include "DummyGNode.h"

using boost::unordered_set;

DummyGNode::DummyGNode()
	: GNode(DUMMY_) {
}

int DummyGNode::getIfParentStmt() {
	return this->getStartStmt();
}

int DummyGNode::getElseParentStmt() {
	return this->getEndStmt();
}

GNode* DummyGNode::getIfParent() {
	return this->getParents().at(0);
}

GNode* DummyGNode::getElseParent() {
	return this->getParents().at(1);
}

GNode* DummyGNode::getEntrance() {
	return this->entrance;
}

GNode* DummyGNode::getChild() {
	return this->getChildren().at(0);
}

unordered_set<int> DummyGNode::getPrevStmts() {
	return this->prevStmts;
}

void DummyGNode::setIfParent(GNode* parent) {
	this->setFirstParent(parent);
}

void DummyGNode::setElseParent(GNode* parent) {
	this->setSecondParent(parent);
}

void DummyGNode::setEntrance(GNode* entrance) {
	this->entrance = entrance;
}

void DummyGNode::setPrevStmts(unordered_set<int> prevStmts) {
	this->prevStmts = prevStmts;
}