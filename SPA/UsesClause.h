#pragma once

#include "Clause.h"
class UsesClause : public Clause{
public:
	UsesClause(void);
	~UsesClause(void);

	bool isValid(void);
	Results evaluate(void);

private:
	// ONLY EVALUATES PROTOTYPE CASES
	Results evaluateStmtWildVarWild();
	Results evaluateStmtWildVarFixed();
	Results evaluateStmtFixedVarWild();
	Results evaluateStmtFixedVarFixed();
};

