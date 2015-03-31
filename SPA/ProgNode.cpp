#include "ProgNode.h"

// constructors
ProgNode::ProgNode(const string &name)
	: TNode(PROGRAM_, name) {
}

// getters
vector<TNode*>& ProgNode::getProcNodes() {
	// simply replace the methods for getchildren
	return this->getChildren();
}

// setters
void ProgNode::linkProcNode(ProcNode *procNode) {
	this->linkChild(procNode);
}