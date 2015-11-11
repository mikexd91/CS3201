#pragma once
#include "SuchThatClause.h"

class ModifiesClause 
	: public SuchThatClause {

public :
	ModifiesClause(void);
	~ModifiesClause(void);
	
	bool isValid(void);

protected:

	//e.g. Parent(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic();
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string);
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string);
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string);
	//Parent(_,s2)
	unordered_set<string> getAllS2();
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string);
	//Parent(s1,__)
	unordered_set<string> getAllS1();
	//Parent(s1,s2)
	unordered_set<vector<string>> getAllS1AndS2();

private:
	bool isStmtType(string argType);
	bool isProcedureModifies(string proc, string var);
	bool isStmtModifies(int stmtnum, string var);
	//void transferSinglesToResult(set<string>* singles, Results* res);
	//void transferPairsToResult(set<pair<string, string>>* pairs, Results* res);
};

