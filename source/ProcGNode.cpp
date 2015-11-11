#include "ProcGNode.h"

ProcGNode::ProcGNode(const string& name)
	: GNode(PROC_) {
	this->nodeName = name;
}

// Procedure is only allowed to have one child
GNode* ProcGNode::getChild() {
	return this->children.at(0);
}

GNode* ProcGNode::getEndNode() {
	return this->endNode;
}

void ProcGNode::setChild(GNode* child) {
	this->setFirstChild(child);
}

void ProcGNode::setEndNode(GNode* end) {
	this->endNode = end;
}