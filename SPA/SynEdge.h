#pragma once

class SynEdge
{
private:
	int edgeWeight;
	int clauseIndex;
	bool visited;

public:
	// Constructor
	SynEdge(int, int);
	
	// Getters
	int getWeight();
	int getClauseIndex();
	bool isVisited();

	// Setters
	void setWeight(int);
	void setClauseIndex(int);
	void addEdgeWeight(int);
	void setVisited();
};

