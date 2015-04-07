#include "Parent.h"
#include "StmtTable.h"
#include <boost/lockfree/queue.hpp>

using namespace std;

StmtTable* pStmtTable = StmtTable::getInstance();			// stmt table instance
set<Statement*>::iterator pSetIter;						// stmt set iterator
set<int>::iterator pIntIter;							// int set iterator

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
	Statement* stmtObj = pStmtTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getParent();
		return stmt;
	} 
	return -1;
}

set<int> Parent::getChild(int stmtNum) {
	Statement* stmtObj = pStmtTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		set<int> stmtSet = stmtObj->getChildren();
		return stmtSet;
	} 
	return set<int>();
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
	set<int> childSet = getChild(stmtNum);
	set<int> results;
	boost::lockfree::queue<int> queue;
	for (set<int>::iterator child = childSet.begin(); child != childSet.end(); child++) {
		queue.push(*child);
		results.insert(*child);
	}

	while (!queue.empty()) {
		int stmt;
		queue.pop(stmt);
		childSet = getChild(stmt);

		for (set<int>::iterator child = childSet.begin(); child != childSet.end(); child++) {
			queue.push(*child);
			results.insert(*child);
		}
	}
	return results;
}

/*
set<set<int>> Parent::getAllParentStar(int stmtNum1, int stmtNum2) {

}
*/