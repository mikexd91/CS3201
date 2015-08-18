#pragma once
#include "Clause.h"
#include "TNode.h"

class FollowsClause : public Clause
{
public:
	FollowsClause(void);
	~FollowsClause(void);

	bool isValid(void);

	Results evaluate(void);

	bool isFollows(string stmtNum1, string stmtNum2);
	string getFollows(string stmtNum, string unfixedStmtType);			// gets list of statements that follows stmtNum
	string getFollowedBy(string stmtNum, string unfixedStmtType);		// gets list of statements that stmtNum follows
	bool checkIsSameType(NodeType type, string stmtType);
	void followsBothUnfixedArg(string firstArgType, string secondArgType, Results &resObj);
	void followsWithOneUnderscore(string firstArgType, string secondArgType, Results &resObj);

private:
	
};

