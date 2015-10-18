#pragma once
#include <string>
#include "PatternClause.h"
#include "AssgNode.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(const string& syn, const string& var, const string& expr);
	~PatternAssgClause(void);
	
	string getExpression();
	bool isValid();

protected:
	// ---- new stuff ----
	bool matchPattern(string, string);
	unordered_set<string> getAllSynValues();
	// ---- end new stuff -
	
private:
	bool matchExpr(AssgNode* assg, string expr);
	bool matchVar(AssgNode* assg, string var);
	
	bool isExprWild();
	bool isExprSidesWild();

	string _expr;
};

