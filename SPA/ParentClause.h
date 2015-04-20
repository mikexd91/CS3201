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
	Results evaluate(void);

protected:
	StmtTable* stmtTable;

	bool isParent(string stmtNum1, string stmtNum2);				// checks if s1 is parent of s2
	set<int> getChildren(string stmt, string stmtArgType);			// gets immediate children of stmtNum

private:
	int getParent(int stmtNum);										// gets immdiate parent of stmtNum
	
	// set<set<int>> getAllParent(int stmtNum1, int stmtNum2);		// gets lists of all possible Parents

	Results* addParentPairToResult(set<Statement*>, string, string);
	Results* evaluateS1WildS2Wild(string, string, string, string);				// Case: Parent(s1,s2) - stmt1 wild, stmt2 wild
	Results* evaluateS1WildS2Fixed(string, string, string);			// Case: Parent(s1,2) - stmt1 wild, stmt2 fixed
	Results* evaluateS1FixedS2Wild(string, string, string);			// Case: Parent(1,s2) - stmt1 fixed, stmt2 wild
	Results* evaluateS1FixedS2Fixed(string, string);	// Case: Parent(1,2) - stmt1 fixed, stmt2 fixed

};
