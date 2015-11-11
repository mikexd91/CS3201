#pragma once
#include "SuchThatClause.h"
#include "StmtTable.h"

class FollowsStarClause 
	: public SuchThatClause {

public:
	FollowsStarClause(void);
	~FollowsStarClause(void);
	bool isValid(void);

protected:
	StmtTable* stmtTable;
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

	bool isNeededArgType(string, int);
	unordered_set<Statement*> getSetFromArgType(string);
	//unordered_set<vector<string>> evalFirstArgStmt();
	//unordered_set<vector<string>> evalFirstArg();
};

