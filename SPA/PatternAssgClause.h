#pragma once
#include <string>
#include "PatternClause.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(string syn);
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
};

