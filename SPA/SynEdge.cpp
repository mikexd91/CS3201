#include "SynEdge.h"


SynEdge::SynEdge(int edge, int clause) {
	edgeWeight = edge;
	clauseIndex = clause;
}

int SynEdge::getWeight() {
	return edgeWeight;
}

int SynEdge::getClauseIndex() {
	return clauseIndex;
}

void SynEdge::setWeight(int weight) {
	edgeWeight = weight;
}

void SynEdge::setClauseIndex(int index) {
	clauseIndex = index;
}

void SynEdge::addEdgeWeight(int weight) {
	edgeWeight += weight;
}