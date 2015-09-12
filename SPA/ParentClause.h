#pragma once
#include "SuchThatClause.h"
#include "StmtTable.h"
#include "Statement.h"
#include "Results.h"

class ParentClause 
	: public SuchThatClause {

public:
	ParentClause(void);
	~ParentClause(void);
	bool isValid(void);

protected:
	StmtTable* stmtTable;

	bool isParent(string stmtNum1, string stmtNum2);				// checks if s1 is parent of s2
	Statement::ChildrenSet  getChildren(int stmtNum, string stmtArgType);			// gets immediate children of stmtNum
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
	Results::ResultsTable* getAllS1AndS2();

private:
	void insertParentAndChildrenIntoResult(Results::ResultsTable*, unordered_set<Statement*>);
	void insertChildrenIntoStmtNum(unordered_set<string> &, unordered_set<Statement*>, string);
	void insertParentIntoStmtNum(unordered_set<string> &, unordered_set<Statement*>, string);
	int getParent(int, string);	

};