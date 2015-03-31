#include "Parent.h"

using namespace std;

set<Statement*>::iterator setIter;						// stmt set iterator
set<int>::iterator intIter;								// int set iterator

Parent::Parent(void)
{
}


Parent::~Parent(void)
{
}

// Get the StmtObj in StmtTable
Statement* getStmtObj(int stmtnum) {
	StmtTable* table = StmtTable::getInstance();
	const set<Statement*> set = table->getAssgStmts();
	for (setIter=set.begin(); setIter!=set.end(); setIter++) {
		Statement* stmtObj = *setIter;
		int stmtnum2 = stmtObj->getStmtNum();
		if (stmtnum2 == stmtnum) {
			return stmtObj;
		}
	}
	return 0;
}

bool Parent::isParent(int stmtNum1, int stmtNum2) {
	int stmt = getParent(stmtNum2);
	if (stmt != -1 && stmt == stmtNum1) {
		return true;
	} else {
		return false;
	}
}

int Parent::getParent(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		int stmt = stmtObj->getParentOf();
		if (stmt != -1) {
			return stmt;
		}
	} 
	return -1;
}

int Parent::getChild(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	if (stmtObj != 0) {
		int stmt = stmtObj->getChildOf();
		if (stmt != -1) {
			return stmt;
		}
	} 
	return -1;
}

/*
set<set<int>> Parent::getAllParent(int stmtNum1, int stmtNum2) {

}
*/

bool Parent::isParentStar(int stmtNum1, int stmtNum2) {
	set<int> stmtList = getParentStar(stmtNum2);
	if (!stmtList.empty()) {
		for (intIter=stmtList.begin(); intIter!=stmtList.end(); intIter++) {
			int stmtNum = *intIter;
			if (stmtNum == stmtNum1) {
				return true;
			}
		}
	}
	return false;
}

set<int> Parent::getParentStar(int stmtNum) {
	set<int> set;
	int stmt = getParent(stmtNum);
	
	while (stmt != -1) {
		set.insert(stmt);
		stmt = getParent(stmt);
	}

	return set;
}

set<int> Parent::getChildStar(int stmtNum) {
	set<int> set;
	int stmt = getChild(stmtNum);
	
	while (stmt != -1) {
		set.insert(stmt);
		stmt = getChild(stmt);
	}

	return set;
}

/*
set<set<int>> Parent::getAllParentStar(int stmtNum1, int stmtNum2) {

}
*/