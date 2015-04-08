#pragma once

#include "Clause.h"

class PatternClause : public Clause{
public:
	PatternClause();
	~PatternClause(void);

	string getSynonym();
	string getVar();
	bool getVarFixed();
	string getExpression();

	void setVar(string var);
	void setVarFixed(bool fixed);
	void setExpression(string expr);

protected:
	string _expr;

};

