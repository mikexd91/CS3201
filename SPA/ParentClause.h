#pragma once
#include "Clause.h"
#include "StmtTable.h"
#include "Statement.h"
#include "Results.h"

class ParentClause : public Clause{
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
	int getParent(int, string);									// gets immdiate parent of stmtNum

	// set<set<int>> getAllParent(int stmtNum1, int stmtNum2);		// gets lists of all possible Parents

	/**
	Results* addParentPairToResult(set<Statement*>, string, string);
	Results* evaluateS1WildS2Wild(string, string, string, string);				// Case: Parent(s1,s2) - stmt1 wild, stmt2 wild
	Results* evaluateS1WildS2Fixed(string, string, string);			// Case: Parent(s1,2) - stmt1 wild, stmt2 fixed
	Results* evaluateS1FixedS2Wild(string, string, string);			// Case: Parent(1,s2) - stmt1 fixed, stmt2 wild
	Results* evaluateS1FixedS2Fixed(string, string);	// Case: Parent(1,2) - stmt1 fixed, stmt2 fixed
	**/
};
