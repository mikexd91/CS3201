#include "Parent.h"

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
/*
void fillAllChildSet(set<int> childSet, set<int> &set, Parent *inst) {
	if (!childSet.empty()) {
		for (pIntIter=childSet.begin(); pIntIter!=childSet.end(); pIntIter++) {
			set.insert(*pIntIter);
			set<int> childOfChild; // typedef ? or sth to do with namespace?
			// inst->getChild(*pIntIter);
		}
	} 
}

set<int> Parent::getChildStar(int stmtNum) {
	set<int> immediateChildSet = getChild(stmtNum);
	set<int> allChildSet;
	fillAllChildSet(immediateChildSet, allChildSet, this);
	return allChildSet;
}
*/
set<int> Parent::getChildStar(int stmtNum) {
	return set<int>();
}

/*
set<set<int>> Parent::getAllParentStar(int stmtNum1, int stmtNum2) {

}
*/