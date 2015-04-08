#pragma once
#include <string>
#include <set>

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	void addStmt(int);
	void addVar(string);
	void setBool(bool);

	set<int> getStmtResults();
	set<string> getVarResults();
	bool getBoolResults();

	string toString();

private:
	set<int> stmtResultSet;
	set<string> varResultSet;
	bool exist;
};

