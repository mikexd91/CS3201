#pragma once
#include <string>
#include <vector>
#include <utility>

using namespace std;

class SynNode
{
private:
	typedef vector<pair<int, SynNode*>> Edge;
	
	string synonym;
	int weight;
	Edge edgeList;

	void setOtherEdge(int, SynNode*);
	bool isSame(SynNode*, SynNode*);

public:
	// Constructor
	SynNode(string synonym);

	// Getters
	string getSynonym();		
	int getWeight();			// get Synonym weight
	Edge getEdges();

	// Setters
	void setWeight(int);		// set Synonym weight
	void setEdge(int, SynNode*); // set edge weight

};