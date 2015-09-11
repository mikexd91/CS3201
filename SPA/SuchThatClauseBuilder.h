#include "SuchThatClause.h"

#pragma once
class SuchThatClauseBuilder
{
public:
	SuchThatClauseBuilder(ClauseType clauseType);
	~SuchThatClauseBuilder(void);

	// setting the arg value
	void setArg(int argNum, string arg);

	// setting the arg type
	void setArgType(int argNum, string argType);

	// setting whether arg is fixed or not ("" or syn basically)
	void setArgFixed(int argNum, bool argFixed);
	
	SuchThatClause* build();

private:
	ClauseType clauseType;
	string firstArg;
	bool firstArgFixed;
	string firstArgType;
	string secondArg;
	bool secondArgFixed;
	string secondArgType;
};

