#include "Uses.h"

using namespace std;

VarTable* uVarTable = VarTable::getInstance();				// var table instance
StmtTable* uStmtTable = StmtTable::getInstance();			// stmt table instance
set<int>::iterator uIntIter;							// int set iterator

Uses::Uses(void)
{
}


Uses::~Uses(void)
{
}

bool Uses::isUses(int stmt, string varName) {
	set<int> stmtList = this->getAssignOfUses(varName);
	if (!stmtList.empty()) {
		for (uIntIter=stmtList.begin(); uIntIter!=stmtList.end(); uIntIter++) {
			int stmtNum = *uIntIter;
			if (stmtNum == stmt) {
				return true;
			}
		}
	}
	return false;
}

set<int> Uses::getAssignOfUses(string varName) {
	Variable* variableObj = uVarTable->getVariable(varName);
	if (variableObj != NULL) {
		set<int> stmtSet = variableObj->getUsedByStmts();
		return stmtSet;
	}
	return set<int>();
}

set<string> Uses::getVarOfUses(int stmtNum) {
	Statement* stmtObj = uStmtTable->getStmtObj(stmtNum);
	if (stmtObj != NULL) {
		set<string> varStmt = stmtObj->getUses();
		return varStmt;
	}
	return set<string>();
}

/*
set<set<int>> Uses::getAllUses(int stmt1, int stmt2) {

}
*/