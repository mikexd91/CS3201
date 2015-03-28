#pragma once
#include <string>
#include <set>

using namespace std;

class Results
{
public:
	enum Type { Letter, Digit, Tuple, Boolean, None};

	Results(void);
	~Results(void);

	void addStmt(int);
	void addVar(string);
	void setBool(bool);
	void setResultType(Type);

	set<int> getStmtResults();
	set<string> getVarResults();
	bool getBoolResults();

	string toString();

private:
	set<int> stmtResultSet;
	set<string> varResultSet;
	bool exist;
	Type resultType;
};

