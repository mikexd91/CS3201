/*
 * CallGNode.cpp
 *
 *  Created on: 5 Oct, 2015
 *      Author: Leon
 */

#include "CallGNode.h"

CallGNode::CallGNode(int stmtNum)
	: GNode(CALL_) {
	this->startStmt = stmtNum;
	this->endStmt = stmtNum;
}

ProcGNode* CallGNode::getProcChild() {
	if(this->children.size() > 1) {
		return (ProcGNode*)this->children.at(1);
	}

	return NULL;
}

GNode* CallGNode::getChild() {
	if(this->children.size() > 0) {
		return this->children.at(0);
	}

	return NULL;
}

GNode* CallGNode::getParent() {
	if(this->parents.size() > 0) {
		return this->parents.at(0);
	}

	return NULL;
}

void CallGNode::setCallChild(ProcGNode* node) {
	this->setSecondChild(node);
}

void CallGNode::setChild(GNode* node) {
	this->setFirstChild(node);
}

void CallGNode::setParent(GNode* node) {
	this->setFirstParent(node);
}
