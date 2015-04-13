#pragma once

#include "Clause.h"

class PatternClause : public Clause{
public:
	PatternClause();
	~PatternClause(void);

	string getSynonym();
	string getVar();
	bool getVarFixed();

	void setVar(string var);
	void setVarFixed(bool fixed);

protected:

};

