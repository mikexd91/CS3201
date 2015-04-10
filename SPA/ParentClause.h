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
	bool isParent(string stmtNum1, string stmtNum2);				// checks if s2 follows s1

	int getParent(int stmtNum);										// gets immdiate parent of stmtNum
	set<int> getChildren(string stmt, string stmtArgType);			// gets immediate children of stmtNum

	// set<set<int>> getAllParent(int stmtNum1, int stmtNum2);		// gets lists of all possible Parents

	bool isParentStar(int stmtNum1, int stmtNum2);					// checks if Parent*(s1, s2)

	set<int> getParentStar(int stmtNum);							// get set of stmts that are parents of this stmt
	set<int> getChildStar(int stmtNum);								// get set of stmts that are children of this stmt

	StmtTable* stmtTable;

	void addParentPairToResult(set<Statement*>, NodeType, Results*);
};

