#pragma once
#include <string>
#include <vector>
#include <utility>

using namespace std;

class SynNode
{
private:
	string synonym;
	int weight;
	int componentNum;

	bool isSame(SynNode*, SynNode*);

public:
	// Constructor
	SynNode(string synonym);

	// Getters
	string getSynonym();		
	int getWeight();			// get Synonym weight
	int getComponentNum();		// get Component synNode is in

	// Setters
	void setSynonym(string);	// set Synonym name
	void setWeight(int);		// set Synonym weight
	void setComponentNum(int);	// set Component synNode is in
};