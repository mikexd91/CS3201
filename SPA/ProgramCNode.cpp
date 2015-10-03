#include "ProgramCNode.h"

ProgramCNode::ProgramCNode()
	: CNode(PROG_) {

}

ProgramCNode::ProgramCNode(const string& name)
	: CNode(PROG_) {
	this->nodeName = name;
}

ProgramCNode::~ProgramCNode() {

}

vector<CNode*>& ProgramCNode::getProcedures() {
	return this->children;
}

bool ProgramCNode::contains(const string& procName) {
	for(size_t i = 0; i < this->children.size(); i++) {
		CNode* child = children.at(i);
		if(child->getNodeType() == PROC_ && child->getName() == procName) {
			return true;
		}
	}

	return false;
}

ProcCNode* ProgramCNode::getProcNode(const string& procName) {
	for(size_t i = 0; i < this->children.size(); i++) {
		CNode* child = children.at(i);
		if(child->getNodeType() == PROC_ && child->getName() == procName) {
			return (ProcCNode*) child;
		}
	}

	return NULL;
}

void ProgramCNode::linkProcNode(ProcCNode* procNode) {
	this->addChild(procNode);
}
