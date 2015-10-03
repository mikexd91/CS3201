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

	// setting the synonym value i.e. the a in pattern a(_ _ _)
	void setSynonym(string syn);

	// setting the var value i.e. the v in pattern a(v _ _)
	void setVar(string var);

	// setting the var type
	void setVarType(string varType);

	// setting whether var is fixed or not ("" or syn basically)
	void setVarFixed(bool varFixed);

	// setting the expr value i.e. the _"x"_ in pattern a(v _"x"_ _)
	void setExpr(int exprNum, string expr);

	PatternClause* build();

private:
	ClauseType clauseType;
	string syn;
	string var;
	bool varFixed;
	string varType;
	string firstExpr;
	string secondExpr;
};

