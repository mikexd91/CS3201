#pragma once
#include <string>
#include <vector>
#include <set>
#include "StringPair.h"

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	void setClausePassed(bool passed);			// clause is true if it contains at least 1 result for unfixed arg or returns true for boolean
	void setFirstClauseSyn(string s1);
	void setSecondClauseSyn(string s2);
	void setNumOfSyn(int n);
	void addSingleResult(string s);
	void addPairResult(string s1, string s2);
	void setSingleResult(vector<string>& v);
	void setPairResult(vector<pair<string, string>>& v);

	void addSinglesResultsDuplicateHack(string s); // This hack implements uses a set to prevent duplicate results before 
															//	turning it to a vector
	void addPairResultsDuplicateHack(string s1, string s2);
	void convertSinglesSetToVector();
	void convertPairSetToVector();

	bool isClausePassed();
	bool usesSyn(string syn);

	string getFirstClauseSyn();
	string getSecondClauseSyn();
	vector<string> getSinglesResults();
	vector<pair<string, string>> getPairResults();
	set<string> getSelectSynResult(string syn);
	int getNumOfSyn();
	void getIntersect(Results &res);		// Mutates the obj that called this method.
											// if no intercept found, object results will be empty
private:

	void getSingleIntercept(Results &res1, Results &res2);
	void getSinglePairIntercept(Results &res1, Results &res2);
	void getPairIntercept(Results &res1, Results &res2);

	bool clausePassed;
	string firstClauseSyn;
	string secondClauseSyn;
	int numOfSyn;

	vector<pair<string, string>> pairResults;
	vector<string> singleResults;

	set<StringPair> pairSet;
	set<string> singleSet;
};

