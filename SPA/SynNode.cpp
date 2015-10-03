#include "SynNode.h"

using namespace std;

SynNode::SynNode(string name) {
	edgeList = vector<pair<int, SynNode>>();
	this->synonym = name;
}

string SynNode::getSynonym() {
	return synonym;
}

int SynNode::getWeight() {
	return weight;
}

vector<pair<int, SynNode>> SynNode::getEdges() {
	return edgeList;
}

void SynNode::setWeight(int num) {
	this->weight = num;
}

void SynNode::setEdge(int edgeWeight, SynNode node) {
	pair<int, SynNode> edge (edgeWeight, node);
	edgeList.push_back(edge);
	if (node.getEdges().size() > 0) {
		pair<int, SynNode> othersEdge = node.getEdges().back();
		if (!isSame(*this, othersEdge.second)) {
			setOtherEdge(edgeWeight, node);
		}
	} else {
		setOtherEdge(edgeWeight, node);
	}
}

void SynNode::setOtherEdge(int edgeWeight, SynNode node) {
	pair<int, SynNode> edge (edgeWeight, *this);
	node.getEdges().push_back(edge);
}

bool SynNode::isSame(SynNode node1, SynNode node2) {
	string syn1 = node1.getSynonym();
	string syn2 = node2.getSynonym();
	return syn1 == syn2;
}
