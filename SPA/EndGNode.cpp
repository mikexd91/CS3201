#include "EndGNode.h"

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

void EndGNode::setParent(GNode* parent) {
	this->setFirstParent(parent);
}

void EndGNode::setProcNode(GNode* proc) {
	this->procNode = proc;
}