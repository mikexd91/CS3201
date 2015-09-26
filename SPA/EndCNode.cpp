#include "EndCNode.h"

EndCNode::EndCNode()
	: CNode(END_) {

}

CNode* EndCNode::getParent() {
	vector<CNode*> parents = this->getParents();
	return this->parents.at(0);
}

void EndCNode::setParent(CNode* parent) {
	this->setFirstParent(parent);
}
