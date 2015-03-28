#include "Follows.h"

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
	StmtTable* table = StmtTable::getInstance();
	const set<Statement*> set = table->getAssgStmts();
	for (setIter=set.begin(); setIter!=set.end(); setIter++) {
		Statement* stmt = *setIter;
		int stmtnum2 = stmt->getStmtNum();
		if (stmtnum2 == stmtnum) {
			return stmt;
		}
	}
	return 0;
}

bool Follows::isFollows(int stmtNum1, int stmtNum2) {
	int stmt = getFollows(stmtNum1);
	if (stmt != 0 && stmt == stmtNum2) {
		return true;
	} else {
		return false;
	}
}

// gets immediate statement after stmtNum
int Follows::getFollows(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		set<int> stmtsSet = stmtObj->getFollows();
		if (!stmtsSet.empty()) {
			intIter = stmtsSet.begin();
			int stmt = *intIter;
			return stmt;
		}
	} 
	return 0;
}

// gets immediate statement before stmtNum
int Follows::getFollowedBy(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		set<int> stmtsSet = stmtObj->getFollowedBy();
		if (!stmtsSet.empty()) {
			intIter = stmtsSet.end();
			int stmt = *intIter;
			return stmt;
		}
	}
	return 0;
}

/*
set<set<int>> Follows::getAllFollows(int stmtNum1, int stmtNum2) {

}
*/

bool Follows::isFollowsStar(int stmtNum1, int stmtNum2) {
	set<int> stmtList = getFollowsStar(stmtNum1);
	if (!stmtList.empty()) {
		for (intIter=stmtList.begin(); intIter!=stmtList.end(); intIter++) {
			int stmtNum = *intIter;
			if (stmtNum == stmtNum2) {
				return true;
			}
		}
	}
	return false;
}

// gets all those before b in Follows(a, b)
set<int> Follows::getFollowedStarBy(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		set<int> stmtsSet = stmtObj->getFollowedBy();
		return stmtsSet;
	}
	return set<int>();
}

// gets all those after a in Follows(a, b)
set<int> Follows::getFollowsStar(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		set<int> stmtsSet = stmtObj->getFollows();
		return stmtsSet;
	}
	return set<int>();
}

/*
set<set<int>> Follows::getAllFollowsStar(int stmtNum1, int stmtNum2) {

}
*/