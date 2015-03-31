#include "Parent.h"

using namespace std;

StmtTable* pTable = StmtTable::getInstance();			// stmt table instance
set<Statement*>::iterator pSetIter;						// stmt set iterator
set<int>::iterator pIntIter;								// int set iterator

Parent::Parent(void)
{
}

Parent::~Parent(void)
{
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
	Statement* stmtObj = pTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getParentOf();
		if (stmt != -1) {
			return stmt;
		}
	} 
	return -1;
}

int Parent::getChild(int stmtNum) {
	Statement* stmtObj = pTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
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
		for (pIntIter=stmtList.begin(); pIntIter!=stmtList.end(); pIntIter++) {
			int stmtNum = *pIntIter;
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