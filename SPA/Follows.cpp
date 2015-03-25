#include "Follows.h"
#include <set>

using namespace std;

set<Statement*>::iterator setIter;						// stmt set iterator
set<int>::iterator intIter;								// int set iterator

Follows::Follows(void)
{
}


Follows::~Follows(void)
{
}


Statement* getStmtObj(int stmtnum) {
	StmtTable* table = StmtTable.getInstance();
	const set<Statement*> set = table->getAssgStmts();
	for (setIter=set.begin(); setIter!=set.end(); setIter++) {
		Statement* stmt = *setIter;
		int stmtnum2 = stmt->getStmtNum();
		if (stmtnum2 == stmtnum) {
			return stmt;
		}
	}
}

bool Follows::isFollows(int stmtNum1, int stmtNum2) {
	StmtTable* table = StmtTable.getInstance();
	const set<Statement*> assignStmts = table->getAssgStmts();
	set<int> getFollowSet = getFollows(stmtNum1);
}

// gets all those before a in Follows(a, b)
set<int> Follows::getFollows(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	set<int> stmtsSet = stmtObj->getFollows();
	return stmtsSet;
}

// gets all those after b in Follows(a, b)
set<int> Follows::getFollowedBy(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	set<int> stmtsSet = stmtObj->getFollowedBy();
	return stmtsSet;
}

vector<vector<int>> Follows::getAllFollows(int stmtNum1, int stmtNum2) {

}

bool Follows::isFollowsStar(int stmtNum1, int stmtNum2) {

}

vector<int> Follows::getFollowsStar(int stmtNum) {

}

vector<int> Follows::getFollowedStarBy(int stmtNum) {

}

vector<vector<int>> Follows::getAllFollowsStar(int stmtNum1, int stmtNum2) {

}
