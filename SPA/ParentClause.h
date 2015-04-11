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

	void addParentPairToResult(set<Statement*>, NodeType, Results*);
};

