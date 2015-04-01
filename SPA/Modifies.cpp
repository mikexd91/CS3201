#include "Modifies.h"

using namespace std;

VarTable* mVarTable = VarTable::getInstance();				// var table instance
StmtTable* mStmtTable = StmtTable::getInstance();			// stmt table instance
set<int>::iterator mIntIter;							// int set iterator

Modifies::Modifies(void)
{
}


Modifies::~Modifies(void)
{
}

bool Modifies::isModifies(int stmt, string varName) {
	set<int> stmtList = this->getAssignOfModifies(varName);
	if (!stmtList.empty()) {
		for (mIntIter=stmtList.begin(); mIntIter!=stmtList.end(); mIntIter++) {
			int stmtNum = *mIntIter;
			if (stmtNum == stmt) {
				return true;
			}
		}
	}
	return false;
}

set<int> Modifies::getAssignOfModifies(string varName) {
	Variable* variableObj = mVarTable->getVariable(varName);
	if (variableObj != NULL) {
		set<int> stmtSet = variableObj->getModifiedByStmts();
		return stmtSet;
	}
	return set<int>();
}

set<string> Modifies::getVarOfModifies(int stmtNum) {
	Statement* stmtObj = mStmtTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		set<string> varStmt = stmtObj->getModifies();
		return varStmt;
	}
	return set<string>();
}

/*
set<set<int>> Modifies::getAllModifies(int stmt1, int stmt2) {

}
*/