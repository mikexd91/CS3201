#pragma once
#include <string>
#include "PatternClause.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(string syn);
	~PatternAssgClause(void);

private:
	Results evaluateVarWildExprWild(void);
	Results evaulateVarWildExpr(void);
	Results evaluateVarFixedExprWild(void);
	Results evaluateVarFixedExpr(void);
	Results evaluateVarExprWild(void);
	Results evaluateVarExpr(void);
};

