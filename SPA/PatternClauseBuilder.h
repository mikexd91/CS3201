#pragma once

#include "PatternClause.h"
#include "PatternAssgClause.h"
#include "PatternIfClause.h"
#include "PatternWhileClause.h"

class PatternClauseBuilder
{
public:
	PatternClauseBuilder(ClauseType clauseType);
	~PatternClauseBuilder(void);

	// setting the arg value
	void setArg(int argNum, string arg);

	// setting the arg type
	void setArgType(int argNum, string argType);

	// setting whether arg is fixed or not ("" or syn basically)
	void setArgFixed(int argNum, bool argFixed);

	PatternClause* build();

private:
	ClauseType clauseType;
	string firstArg;
	bool firstArgFixed;
	string firstArgType;
	string secondArg;
	bool secondArgFixed;
	string secondArgType;
};

