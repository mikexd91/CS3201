#include "SynNode.h"

using namespace std;

SynNode::SynNode(void) {

}

SynNode::~SynNode(void) {

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

bool SynNode::isSame(SynNode node1) {
	string syn1 = node1.getSynonym();
	string syn2 = synonym;
	return syn1 == syn2;
}
