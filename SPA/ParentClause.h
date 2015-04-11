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

private: 
	Results* evaluateS1WildS2Wild(string, string);				// Case: Parent(s1,s2) - stmt1 wild, stmt2 wild
	Results* evaluateS1WildS2Fixed(string, string);			// Case: Parent(s1,2) - stmt1 wild, stmt2 fixed
	Results* evaluateS1FixedS2Wild(string, string, string);			// Case: Parent(1,s2) - stmt1 fixed, stmt2 wild
	Results* evaluateS1FixedS2Fixed(string, string);	// Case: Parent(1,2) - stmt1 fixed, stmt2 fixed

	bool isParent(string stmtNum1, string stmtNum2);				// checks if s2 follows s1

	int getParent(int stmtNum);										// gets immdiate parent of stmtNum
	set<int> getChildren(string stmt, string stmtArgType);			// gets immediate children of stmtNum

	// set<set<int>> getAllParent(int stmtNum1, int stmtNum2);		// gets lists of all possible Parents

	bool isParentStar(int stmtNum1, int stmtNum2);					// checks if Parent*(s1, s2)

	set<int> getParentStar(int stmtNum);							// get set of stmts that are parents of this stmt
	set<int> getChildStar(int stmtNum);								// get set of stmts that are children of this stmt

	StmtTable* stmtTable;

	Results* addParentPairToResult(set<Statement*>, NodeType);
};

