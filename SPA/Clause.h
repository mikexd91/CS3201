#pragma once
#include "ConstTable.h"
#include "ProcTable.h"
#include "Results.h"
#include "StmtTable.h"
#include "VarTable.h"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::map;
using std::string;

enum ClauseType{
	FOLLOWS_, 
	FOLLOWSSTAR_, 
	PARENT_, 
	PARENTSTAR_, 
	USES_, 
	MODIFIES_, 
	PATTERN_
};

class Clause {

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
	virtual unordered_set<unordered_map<string, string>> getAllS1AndS2()=0;

	bool isValidityCheck();
	bool isValidStmtNumber(string);
	bool isValidVariable(string);
	bool isValidProcedure(string);
	bool isValidConstant(string);

	virtual bool childValidityCheck();
	
public:
	Clause(void);
	Clause(ClauseType);
	~Clause(void);

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

	
	bool evaluate(Results*); 
	virtual bool isValid()=0;
	
};
