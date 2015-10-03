#include "CNode.h"

using namespace std;

CNode::CNode(CType type) {
	this->nodeType = type;
	this->startStmt = 0;
	this->endStmt = 0;
	this->nodeName = "";
}

CNode::~CNode() {

}

CType CNode::getNodeType() {
	return this->nodeType;
}

string CNode::getName() {
	return this->nodeName;
}

int CNode::getStartStmt() {
	return this->startStmt;
}

int CNode::getEndStmt() {
	return this->endStmt;
}

vector<CNode*>& CNode::getParents() {
	return this->parents;
}

vector<CNode*>& CNode::getChildren() {
	return this->children;
}

void CNode::setNodeType(CType nodeType) {
	this->nodeType = nodeType;
}

void CNode::setName(string name) {
	this->nodeName = name;
}

void CNode::setStartStmt(int start) {
	this->startStmt = start;
}

void CNode::setEndStmt(int end) {
	this->endStmt = end;
}

void CNode::addChild(CNode* child) {
	this->children.push_back(child);
}

void CNode::setFirstChild(CNode* child) {
	if(this->children.size() < 1) {
		this->children.push_back(child);
	} else {
		this->children.at(0) = child;
	}
}

void CNode::setSecondChild(CNode* child) {
	if(this->children.size() < 1) {
		this->children.push_back(NULL);
		this->children.push_back(child);
	} else if(this->children.size() < 2) {
		this->children.push_back(child);
	} else {
		this->children.at(1) = child;
	}
}

void CNode::setFirstParent(CNode* parent) {
	if(this->parents.size() < 1) {
		this->parents.push_back(parent);
	} else {
		this->parents.at(0) = parent;
	}
}

void CNode::setSecondParent(CNode* parent) {
	if(this->parents.size() < 1) {
		this->parents.push_back(NULL);
		this->parents.push_back(parent);
	} else if(this->parents.size() < 2) {
		this->parents.push_back(parent);
	} else {
		this->parents.at(1) = parent;
	}
}

bool CNode::isNodeType(CType type) {
	if(this->nodeType == type) {
		return true;
	}

	return false;
}
