#include "ProcCNode.h"

ProcCNode::ProcCNode(const string& name)
	: CNode(PROC_) {
	this->nodeName = name;
}

// Procedure is only allowed to have one child
CNode* ProcCNode::getChild() {
	return this->children.at(0);
}

void ProcCNode::setChild(CNode* child) {
	this->setFirstChild(child);
}
