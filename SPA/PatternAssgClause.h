#pragma once
#include <string>
#include "PatternClause.h"
#include "AssgNode.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(const string&);
	~PatternAssgClause(void);

	Results evaluate();
	bool isValid();

private:
	Results evaluateVarWildExprWild(vector<int> assgNums);
	Results evaulateVarWildExpr(vector<int> assgNums, string expr);
	Results evaluateVarFixedExprWild(vector<int> assgNums);
	Results evaluateVarFixedExpr(vector<int> assgNums, string expr);
	Results evaluateVarExprWild(vector<int> assgNums);
	Results evaluateVarExpr(vector<int> assgNums, string expr);

	bool matchExpr(AssgNode* assg, string expr);
	bool matchVar(AssgNode* assg, string var);
};

