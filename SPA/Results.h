#pragma once
#include <string>
#include <vector>
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

	vector<bool> isExist(vector<string> synList);
	vector<unordered_map<string, string>> selectMultiSyn(vector<string> synList); // we are using database query language as a guide, hence select instead of get
	vector<string> selectSyn(string syn);
	vector<unordered_map<string, string>> selectSynWhere(string syn, string val, vector<unordered_map<string, string>>);
	
	// Insert will help create the synonym and add results to it
	bool insertMultiResult(unordered_map<string, string> results);
	bool insertResult(string syn, string value);

	bool delMultiResult(unordered_map<string, string> results);
	bool delResult(string syn, string value);

private:
	static bool clausePassed;
	static vector<unordered_map<string, string>> resultsTable;
};

