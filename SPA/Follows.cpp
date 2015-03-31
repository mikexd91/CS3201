#include "Follows.h"

using namespace std;

StmtTable* table = StmtTable::getInstance();			// stmt table instance
set<Statement*>::iterator setIter;						// stmt set iterator
set<int>::iterator intIter;								// int set iterator

Follows::Follows(void)
{
}

Follows::~Follows(void)
{
}

bool Follows::isFollows(int stmtNum1, int stmtNum2) {
	int stmt = getFollows(stmtNum1);
	if (stmt != -1 && stmt == stmtNum2) {
		return true;
	} else {
		return false;
	}
}

// gets immediate statement after stmtNum
int Follows::getFollows(int stmtNum) {
	Statement* stmtObj = table->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getFollows();
		if (stmt != -1) {
			return stmt;
		}
	} 
	return -1;
}

// gets immediate statement before stmtNum
int Follows::getFollowedBy(int stmtNum) {
	Statement* stmtObj = table->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getFollowedBy();
		if (stmt != -1) {
			return stmt;
		}
	}
	return -1;
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
	set<int> set;
	int stmt = getFollowedBy(stmtNum);
	
	while (stmt != -1) {
		set.insert(stmt);
		stmt = getFollowedBy(stmt);
	}

	return set;
}

// gets all those after a in Follows(a, b)
set<int> Follows::getFollowsStar(int stmtNum) {
	set<int> set;
	int stmt = getFollows(stmtNum);
	
	while (stmt != -1) {
		set.insert(stmt);
		stmt = getFollows(stmt);
	}

	return set;
}

/*
set<set<int>> Follows::getAllFollowsStar(int stmtNum1, int stmtNum2) {

}
*/