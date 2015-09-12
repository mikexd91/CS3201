#include "CNode.h"

using namespace std;

CNode::CNode(CType type) {
	this->nodeType = type;
	this->firstChild = NULL;
	this->secondChild = NULL;
	this->firstParent = NULL;
	this->secondParent = NULL;
	this->startStmt = 0;
	this->endStmt = 0;
}

CNode::~CNode() {

}

CType CNode::getNodeType() {
	return this->nodeType;
}

int CNode::getStartStmt() {
	return this->startStmt;
}

int CNode::getEndStmt() {
	return this->endStmt;
}

CNode* CNode::getFirstChild() {
	return this->firstChild;
}

CNode* CNode::getSecondChild() {
	return this->secondChild;
}

CNode* CNode::getFirstParent() {
	return this->firstParent;
}

CNode* CNode::getSecondParent() {
	return this->secondParent;
}

void CNode::setNodeType(CType nodeType) {
	this->nodeType = nodeType;
}

void CNode::setStartStmt(int start) {
	this->startStmt = start;
}

void CNode::setEndStmt(int end) {
	this->endStmt = end;
}

void CNode::linkFirstChild(CNode* child) {
	this->firstChild = child;
}

void CNode::linkSecondChild(CNode* child) {
	this->secondChild = child;
}

void CNode::linkFirstParent(CNode* parent) {
	this->firstParent = parent;
}

void CNode::linkSecondParent(CNode* parent) {
	this->secondParent = parent;
}

bool CNode::isNodeType(CType type) {
	if(this->nodeType == type) {
		return true;
	}

	return false;
}
