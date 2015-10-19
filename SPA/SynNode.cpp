#include "SynNode.h"

using namespace std;

SynNode::SynNode(string name) {
	this->synonym = name;
}

string SynNode::getSynonym() {
	return synonym;
}

int SynNode::getWeight() {
	return weight;
}

int SynNode::getComponentNum() {
	return componentNum;
}

void SynNode::setSynonym(string name) {
	this->synonym = name;
}

void SynNode::setWeight(int num) {
	this->weight = num;
}

void SynNode::setComponentNum(int num) {
	this->componentNum = num;
}
/*
void SynNode::setEdge(int edgeWeight, SynNode* node) {
	pair<int, SynNode*> edge (edgeWeight, node);
	edgeList.push_back(edge);
	if (node->getEdges().size() > 0) {
		pair<int, SynNode*> othersEdge = node->getEdges().back();
		if (!isSame(this, othersEdge.second)) {
			setOtherEdge(edgeWeight, node);
		}
	} else {
		setOtherEdge(edgeWeight, node);
	}
}

void SynNode::setOtherEdge(int edgeWeight, SynNode* node) {
	node->setEdge(edgeWeight, this);
}

bool SynNode::isSame(SynNode* node1, SynNode* node2) {
	string syn1 = node1->getSynonym();
	string syn2 = node2->getSynonym();
	return syn1 == syn2;
}
*/