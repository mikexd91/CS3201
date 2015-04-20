#pragma once
#include "Clause.h"
class FollowsStarClause : public Clause{
public:
	FollowsStarClause(void);
	~FollowsStarClause(void);

	bool isValid(void);

	int getFirstStmtNum();
	int getSecondStmtNum();
	string getFirstStmtSyn();
	string getSecondStmtSyn();
	string getFirstStmtType();
	string getSecondStmtType();

	Results evaluate(void);

private:
	Results evaluateFollowsStarFixedFixed(int firstStmtNum, int secondStmtNum);
	Results evaluateFollowsStarFixedSyn(int firstStmtNum, string secondStmtSyn);
	Results evaluateFollowsStarSynFixed(string firstStmtSyn, int secondStmtNum);
	Results evaluateFollowsStarSynSyn(string firstStmtSyn, string secondStmtSyn);

	bool isFollowsStar(int stmt1, int stmt2);
	void transferSinglesToResult(set<string>* singles, Results* res);
	void transferPairsToResult(set<pair<string, string>>* pairs, Results* res);
};

