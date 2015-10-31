#include "SynEdge.h"


SynEdge::SynEdge(int edge, int clause) {
	edgeWeight = edge;
	clauseIndex = clause;
	visited = false;
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

void SynEdge::setVisited() {
	visited = true;
}

bool SynEdge::isVisited() {
	return visited;
}