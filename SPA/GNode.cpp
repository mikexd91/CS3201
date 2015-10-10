#include "GNode.h"

using namespace std;

GNode::GNode(GType type) {
	this->nodeType = type;
	this->startStmt = 0;
	this->endStmt = 0;
	this->nodeName = "";
}

GNode::~GNode() {

}

GType GNode::getNodeType() {
	return this->nodeType;
}

string GNode::getName() {
	return this->nodeName;
}

int GNode::getStartStmt() {
	return this->startStmt;
}

int GNode::getEndStmt() {
	return this->endStmt;
}

vector<GNode*>& GNode::getParents() {
	return this->parents;
}

vector<GNode*>& GNode::getChildren() {
	return this->children;
}

void GNode::setNodeType(GType nodeType) {
	this->nodeType = nodeType;
}

void GNode::setName(string name) {
	this->nodeName = name;
}

void GNode::setStartStmt(int start) {
	this->startStmt = start;
}

void GNode::setEndStmt(int end) {
	this->endStmt = end;
}

void GNode::addChild(GNode* child) {
	this->children.push_back(child);
}

void GNode::setFirstChild(GNode* child) {
	if(this->children.size() < 1) {
		this->children.push_back(child);
	} else {
		this->children.at(0) = child;
	}
}

void GNode::setSecondChild(GNode* child) {
	if(this->children.size() < 1) {
		this->children.push_back(NULL);
		this->children.push_back(child);
	} else if(this->children.size() < 2) {
		this->children.push_back(child);
	} else {
		this->children.at(1) = child;
	}
}

void GNode::setFirstParent(GNode* parent) {
	if(this->parents.size() < 1) {
		this->parents.push_back(parent);
	} else {
		this->parents.at(0) = parent;
	}
}

void GNode::setSecondParent(GNode* parent) {
	if(this->parents.size() < 1) {
		this->parents.push_back(NULL);
		this->parents.push_back(parent);
	} else if(this->parents.size() < 2) {
		this->parents.push_back(parent);
	} else {
		this->parents.at(1) = parent;
	}
}

bool GNode::isNodeType(GType type) {
	if(this->nodeType == type) {
		return true;
	}

	return false;
}
