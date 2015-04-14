#pragma once

#include "Clause.h"

class ModifiesClause : public Clause {

public :
	ModifiesClause(void);
	~ModifiesClause(void);
	
	bool isStmtFixed();
	bool isVarFixed();
	string getStmt();
	int getStmtNum();
	string getVar();
	string getStmtType();

	bool isValid(void);
	Results evaluate(void);
	
private:
	Results evaluateFixedFixed(int stmtNum, string varName);
	Results evaluateFixedSyn(int stmtNum, string var);
	Results evaluateSynFixed(string stmt, string varName);
	Results evaluateSynSyn(string stmt, string var);
	
	bool isModifies(int stmtNum, string var);
};

