#pragma once
#include "Clause.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstTable.h"

class PatternClause 
	: public Clause {

protected:
	ClauseType clauseType;
	string syn;
	string synType;
	string var;
	bool varFixed;
	string varType;

	StmtTable* stmtTable;
	VarTable* varTable;
	ProcTable* procTable;

	// given a stmt num (in string form) and a fixed varname, return true if the pattern matches for this stmt.
	virtual bool matchPattern(string synVal, string varVal)=0;

	// specific syn values depending on assg, while or if
	virtual unordered_set<string> getAllSynValues()=0;

private:
	bool isBaseValidityCheck();
	bool isValidStmtNumber(string);
	bool isValidVariable(string);
	bool isValidProcedure(string);
	bool isValidConstant(string);
	bool isValidIf(string);
	bool isValidAssign(string);
	bool isValidWhile(string);

	// shud be the same across the board, just getvarnames from vartable
	unordered_set<string> getAllVarValues();

public:
	PatternClause(void);
	~PatternClause(void);

	ClauseType getClauseType();
	string getSyn();
	string getVar();
	string getVarType();
	bool isVarFixed();

	void setVarFixed(bool);
	void setVarType(string);

	bool evaluate(Result*);

	//Check the validity of the specific clause
	//Substitute the original clause isValid for this one
	virtual bool isValid(void)=0;
};

