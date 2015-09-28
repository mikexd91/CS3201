#pragma once

#include "WithClause.h"

class WithClauseBuilder
{
public:
	WithClauseBuilder(ClauseType clauseType);
	~WithClauseBuilder(void);

	// setting the arg value
	void setEntity(int, string);
	void setEntityType(int, string);

	// setting the arg type
	void setArgType(int argNum, string argType);

	// setting whether arg is fixed or not ("" or syn basically)
	void setArgFixed(int argNum, bool argFixed);

	WithClause* build();

private:
	ClauseType clauseType;
	string firstArg;
	bool firstArgFixed;
	string firstArgType;
	string secondArg;
	bool secondArgFixed;
	string secondArgType;
};

