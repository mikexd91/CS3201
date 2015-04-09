#pragma once
#include <string>
#include <vector>
#include <set>

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	void setClauseTrue();
	void setFirstClauseSyn(string s1);
	void setSecondClauseSyn(string s2);
	void setNumOfSyn(int n);
	void addSingleResult(string s);
	void addPairResult(string s1, string s2);

	bool isClauseTrue();

	string getFirstClauseSyn();
	string getSecondClauseSyn();
	vector<string> getSinglesResults();
	vector<pair<string, string>> getPairResults();
	set<string> getSelectSynResult(string syn);
	int getNumOfSyn();
	bool usesSelectSyn(string syn);

	void getIntersect(Results res);			// Mutates the obj that called this method.
											// Transfer all result to the mutated obj
private:

	bool isValid;
	string firstClauseSyn;
	string secondClauseSyn;
	int numOfSyn;
	vector<pair<string, string>> pairResults;
	vector<string> singleResults;

};

