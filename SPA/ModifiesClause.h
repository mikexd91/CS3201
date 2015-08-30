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

protected:
	bool isValid(void);

	//e.g. Parent(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic()=0;
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string)=0;
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string)=0;
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string)=0;
	//Parent(_,s2)
	unordered_set<string> getAllS2()=0;
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string)=0;
	//Parent(s1,__)
	unordered_set<string> getAllS1()=0;
	//Parent(s1,s2)
	unordered_set<unordered_map<string, string>> getAllS1AndS2()=0;

private:
	bool isProcedureModifies(string proc, string var);
	bool isStmtModifies(int stmtNum, string var);
	void transferSinglesToResult(set<string>* singles, Results* res);
	void transferPairsToResult(set<pair<string, string>>* pairs, Results* res);
};

