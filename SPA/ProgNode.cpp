#include "ProgNode.h"

using namespace std;

// constructors
ProgNode::ProgNode()
	: TNode(PROGRAM_) {
}

ProgNode::ProgNode(const string &name)
	: TNode(PROGRAM_, name) {
}

// getters
vector<TNode*>& ProgNode::getProcNodes() {
	// simply replace the methods for getchildren
	return this->getChildren();
}

bool ProgNode::contains(const string& procName) {
	int numChildren = this->_children.size();
	for (int i = 0; i < numChildren; i++) {
		TNode* child = this->_children.at(i);
		string childName = child->getName();
		NodeType childType = child->getNodeType();
		if (procName == childName && childType == PROCEDURE_) {
			return true;
		}
	}
	return false;
}

ProcNode* ProgNode::getProcNode(const string& procName) {
	int numChildren = this->_children.size();
	for (int i = 0; i < numChildren; i++) {
		TNode* child = this->_children.at(i);
		string childName = child->getName();
		NodeType childType = child->getNodeType();
		if (procName == childName && childType == PROCEDURE_) {
			return (ProcNode*)child;
		}
	}
	return NULL;
}

// setters
void ProgNode::linkProcNode(ProcNode *procNode) {
	this->linkChild(procNode);
}