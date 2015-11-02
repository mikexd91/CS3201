#include "EndGNode.h"

using boost::unordered_set;

EndGNode::EndGNode()
	: GNode(END_) {

}

GNode* EndGNode::getParent() {
	vector<GNode*> parents = this->getParents();
	return this->parents.at(0);
}

GNode* EndGNode::getProcNode() {
	return this->procNode;
}

unordered_set<int> EndGNode::getPrevStmts() {
	return this->prevStmts;
}

void EndGNode::setParent(GNode* parent) {
	this->setFirstParent(parent);
}

void EndGNode::setProcNode(GNode* proc) {
	this->procNode = proc;
}

void EndGNode::setPrevStmts(unordered_set<int> prevStmts) {
	this->prevStmts = prevStmts;
}