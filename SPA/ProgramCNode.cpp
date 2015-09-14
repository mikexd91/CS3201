#include "ProgramCNode.h"

ProgramCNode::ProgramCNode()
	: CNode(PROGRAM_) {

}

ProgramCNode::ProgramCNode(const string& name)
	: CNode(PROGRAM_) {
	this->nodeName = name;
}

ProgramCNode::~ProgramCNode() {

}

vector<CNode*>& ProgramCNode::getProcedures() {
	return this->children;
}

bool ProgramCNode::contains(const string& procName) {
	for(int i = 0; i < this->children.size(); i++) {
		CNode* child = children.at(i);
		if(child->nodeType == PROCEDURE_ && child->nodeName == procName) {
			return true;
		}
	}

	return false;
}

ProcCNode* ProgramCNode::getProcNode(const string& procName) {
	for(int i = 0; i < this->children.size(); i++) {
		CNode* child = children.at(i);
		if(child->nodeType == PROCEDURE_ && child->nodeName == procName) {
			return (ProcCNode*) child;
		}
	}

	return NULL;
}

void ProgramCNode::linkProcNode(ProcCNode* procNode) {
	this->addChild(procNode);
}
