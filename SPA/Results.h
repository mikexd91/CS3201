#pragma once
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	bool isClausePass();
	void setClausePass();
	void resetClausePass();

	bool hasResults(string syn);

	vector<unordered_map<string, string>>* selectMultiSyn(vector<string> synList); // we are using database query language as a guide, hence select instead of get
	vector<string>* selectSyn(string syn);
	vector<unordered_map<string, string>>* selectSynWhere(string syn, string val, vector<unordered_map<string, string>> results);
	
	// Insert will help create the synonym and add results to it
	bool insertMultiResult(unordered_map<string, string> results);
	bool insertResult(string syn, string value);
	bool pushChanges();

	bool delMultiResult(unordered_map<string, string> results);
	bool delResult(string syn, string value);

private:
	static bool clausePassed;
	static bool insertBufferFlag;
	static bool delBufferFlag;

	vector<unordered_map<string, string>> resultsTable;
	set<unordered_map<string, string>> insertStack;
	set<unordered_map<string, string>> delStack;

	bool isResultsTableEmpty();
	void setInsertBufferFlag();
	void resetInsertBufferFlag();
	void setDelBufferFlag();
	void resetDelBufferFlag();
	bool pushInsertResults();
	bool pushDelResults();

	unordered_map<string, string> getSynResults(unordered_map<string, string> row, vector<string> synList);
};

