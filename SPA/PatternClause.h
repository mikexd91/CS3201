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
	string firstArg;
	bool firstArgFixed;
	string firstArgType;
	string secondArg;
	bool secondArgFixed;
	string secondArgType;
	string patternArg;
	Results::ResultsTable table;

	StmtTable* stmtTable;
	VarTable* varTable;
	ProcTable* procTable;

	//e.g. Parent(string,string)
	virtual bool evaluateS1FixedS2Fixed(string, string)=0;
	//e.g. Parent(_,_)
	virtual bool evaluateS1GenericS2Generic()=0;
	//e.g. Parent(_,string)
	virtual bool evaluateS1GenericS2Fixed(string)=0;
	//Parent(string,_)
	virtual bool evaluateS1FixedS2Generic(string)=0;
	//Parent(string,s2)
	virtual unordered_set<string> getAllS2WithS1Fixed(string)=0;
	//Parent(_,s2)
	virtual unordered_set<string> getAllS2()=0;
	//Parent(s1,string)
	virtual unordered_set<string> getAllS1WithS2Fixed(string)=0;
	//Parent(s1,__)
	virtual unordered_set<string> getAllS1()=0;
	//Parent(s1,s2)
	virtual Results::ResultsTable* getAllS1AndS2()=0;

private:
	bool isBaseValidityCheck();
	bool isValidStmtNumber(string);
	bool isValidVariable(string);
	bool isValidProcedure(string);
	bool isValidConstant(string);
	bool isValidIf(string);
	bool isValidAssign(string);
	bool isValidWhile(string);

public:
	~PatternClause(void);

	ClauseType getClauseType();
	void setFirstArg(string);
	void setSecondArg(string);
	void setFirstArgType(string);
	void setSecondArgType(string);
	void setFirstArgFixed(bool);
	void setSecondArgFixed(bool);

	string getFirstArg();
	string getSecondArg();
	string getFirstArgType();
	string getSecondArgType();
	bool getFirstArgFixed();
	bool getSecondArgFixed();

	string getSynonym();
	string getVar();
	string getVarType();
	bool getVarFixed();

	virtual bool evaluate(Results*)=0;

	//Check the validity of the specific clause
	//Substitute the original clause isValid for this one
	virtual bool isValid(void)=0;

	void setVar(string var);
	void setVarType(string varType);
	void setVarFixed(bool fixed);

};

