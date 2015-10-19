#pragma once

class SynEdge
{
private:
	int edgeWeight;
	int clauseIndex;
	
public:
	// Constructor
	SynEdge(int, int);
	
	// Getters
	int getWeight();
	int getClauseIndex();

	// Setters
	void setWeight(int);
	void setClauseIndex(int);
	void addEdgeWeight(int);
};

