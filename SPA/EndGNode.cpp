#include "EndGNode.h"

EndGNode::EndGNode()
	: GNode(END_) {

}

GNode* EndGNode::getParent() {
	vector<GNode*> parents = this->getParents();
	return this->parents.at(0);
}

void EndGNode::setParent(GNode* parent) {
	this->setFirstParent(parent);
}
