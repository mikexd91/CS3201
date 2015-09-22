#pragma once
#include <string>
#include "PatternClause.h"
#include "WhileNode.h"

using namespace std;

class PatternWhileClause :
	public PatternClause {

public:
	PatternWhileClause(const string&, const string&, const string&);
	~PatternWhileClause(void);

	string getStmtList();
	bool isValid();

protected:
	// ---- new stuff ----
	bool matchPattern(string, string);
	unordered_set<string> getAllSynValues();
	// ---- end new stuff -

private:
	bool matchStmtList(WhileNode*, string);
	bool matchVar(WhileNode*, string);

	bool isStmtListWild();
	bool isVarWild();

	string _stmtList;
};

