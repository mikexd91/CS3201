#pragma once
#include <string>
#include <vector>

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	void setClauseTrue();
	void setFirstClauseSyn(string s1);
	void setSecondClauseSyn(string s2);
	void addSingleResult(string s);
	void addPairResult(string s1, string s2);

	bool isClauseTrue();

	string getFirstClauseSyn();
	string getSecondClauseSyn();
	vector<string> getSinglesResults();
	vector<pair<string, string>> getPairResults();

private:

	bool isValid;
	string firstClauseSyn;
	string secondClauseSyn;
	vector<pair<string, string>> pairResults;
	vector<string> singleResults;

};

