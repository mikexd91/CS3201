#pragma once
#include "Clause.h"

class FollowsClause : public Clause
{
public:
	FollowsClause(void);
	~FollowsClause(void);
	bool isValid(void);
	Results evaluate(void);

private:
	bool isFollows(string stmtNum1, string stmtNum2);
	string getFollows(string stmtNum, string unfixedStmtType);			// gets list of statements that follows stmtNum
	string getFollowedBy(string stmtNum, string unfixedStmtType);		// gets list of statements that stmtNum follows
};

