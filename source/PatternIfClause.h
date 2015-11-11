#pragma once
#include <string>
#include "PatternClause.h"
#include "IfNode.h"

using namespace std;

class PatternIfClause :
	public PatternClause {

public:
	PatternIfClause(const string&, const string&, const string&, const string&);
	~PatternIfClause(void);

	string getThenList();
	string getElseList();
	bool isValid();

	//void setThenList(string);
	//void setElseList(string);

protected:
	// ---- new stuff ----
	bool matchPattern(string, string);
	unordered_set<string> getAllSynValues();
	// ---- end new stuff -

private:
	bool matchThenList(IfNode*, string);
	bool matchElseList(IfNode*, string);
	bool matchVar(IfNode*, string);

	bool isThenListWild();
	bool isElseListWild();

	string _thenList;
	string _elseList;
};

