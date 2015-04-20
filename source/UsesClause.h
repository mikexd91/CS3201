#pragma once

#include "Clause.h"
#include "Statement.h"

class UsesClause : public Clause{
public:
	UsesClause(void);
	~UsesClause(void);

	bool isValid(void);
	Results evaluate(void);

private:
	// ONLY EVALUATES PROTOTYPE CASES (only assign statements)
	Results evaluateStmtWildVarWild();			// Case: Uses(s,v) - stmt wild, var wild
	Results evaluateStmtWildVarFixed();			// Case: Uses(s,'x') - stmt wild, var fixed
	Results evaluateStmtFixedVarWild();			// Case: Uses(1,v) - stmt fixed, var wild
	Results evaluateStmtFixedVarFixed();		// Case: Uses(1,'x') - stmt fixed, var fixed

};