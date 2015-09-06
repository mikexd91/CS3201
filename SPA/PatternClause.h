#pragma once

#include "Clause.h"

class PatternClause : public Clause {
public:
	PatternClause();
	~PatternClause(void);

	string getSynonym();
	string getVar();
	string getVarType();
	bool getVarFixed();

	void setVar(string var);
	void setVarType(string varType);
	void setVarFixed(bool fixed);

};

