#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Results
{
public:
	typedef unordered_set<unordered_map<string, string>>* ResultsTable;
	typedef unordered_map<string, string> ResultsRow;
	
	Results(void);
	~Results(void);

	bool isClausePass();
	void setClausePass();
	void resetClausePass();

	// checks if syn exist in resultsTable
	bool hasResults(string syn);

	// we are using database query language as a guide, hence select instead of get
	
	// for clauses with 2 or more synonyms
	ResultsTable selectMultiSyn(unordered_set<string> synList); 
	// for clauses with 1 synonym
	unordered_set<string>* selectSyn(string syn);
	
	// for clauses with 2 or more synonyms
	bool insertMultiResult(ResultsRow results);
	// for clauses with 1 synonym
	bool insertResult(string syn, string value);
	// called after all results have been inserted. push tells me what to delete
	bool push();

	// Testing
	int getResultsTableSize();

private:
	bool clausePassed;
	ResultsTable resultsTable;
	
};

