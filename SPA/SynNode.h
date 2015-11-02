#pragma once
#include <string>
#include <vector>

using namespace std;

class SynNode
{
private:
	string synonym;
	int weight;
	int componentNum;

public:
	// Constructor
	SynNode(void);
	~SynNode(void);

	// Getters
	string getSynonym();		
	int getWeight();			// get Synonym weight
	int getComponentNum();		// get Component synNode is in

	// Setters
	void setSynonym(string);	// set Synonym name
	void setWeight(int);		// set Synonym weight
	void setComponentNum(int);	// set Component synNode is in

	bool isSame(SynNode);
};