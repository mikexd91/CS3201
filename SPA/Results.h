#pragma once
#include <string>
#include <vector>

using namespace std;

class Results
{
public:
	Results(void);
	~Results(void);

	void addStmt(int);
	void addVar(string);
	void setResultType(string);

	vector<int> getStmtResults();
	vector<string> getVarResults();

	string toString();

private:
	// Vector<tuple<int, int>> tupleResultSet;
	vector<int> stmtResultSet;
	vector<string> varResultSet;
	string resultType;
};

