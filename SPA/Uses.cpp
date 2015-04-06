#include "Uses.h"

using namespace std;

VarTable* uVarTable = VarTable::getInstance();				// var table instance
StmtTable* uStmtTable = StmtTable::getInstance();			// stmt table instance
set<int>::iterator uIntIter;								// int set iterator
set<string>::iterator uStringIter;							// string set iterator

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

// for all assignment in set<int>, check if its var is in set<string> var
set<int> Uses::getAssignOfUsesUnfixed(set<int> assign, set<string> var0) {
	set<int> result;
	for (uIntIter=assign.begin(); uIntIter!=assign.end(); uIntIter++) {
		int stmtNum = *uIntIter;
		Statement* stmtObj = uStmtTable->getStmtObj(stmtNum);
		// get uses of stmtObj
		set<string> usedVar = stmtObj->getUses();
		// check if its var is in set<int> var
		for (uStringIter=usedVar.begin(); uStringIter!=usedVar.end(); uStringIter++) {
			string var1 = *uStringIter;
			set<string>::iterator iter = var0.find(var1);
			if (iter != var0.end()) {
				result.insert(stmtNum);
				break;
			}
		}
	}
	return result;
}

// for all var in set<string>, check if the assignment that uses it is in set<int> assign
set<string> Uses::getVarOfUsesUnfixed(set<int> assign, set<string> var0) {
	set<string> result;
	// for all variable in var0
	for (uStringIter=var0.begin(); uStringIter!=var0.end(); uStringIter++) {
		string var1 = *uStringIter;
		Variable *var2 = uVarTable->getVariable(var1);
		set<int> stmtSet = var2->getUsedByStmts();
		// for all stmt in stmtSet
		for (uIntIter=stmtSet.begin(); uIntIter!=stmtSet.end(); uIntIter++) {
			int stmt = *uIntIter;
			set<int>::iterator iter = assign.find(stmt);
			if (iter != assign.end()) {
				result.insert(var1);
				break;
			}
		}
	}
	return result;
}

/*
set<set<int>> Uses::getAllUses(int stmt1, int stmt2) {

}
*/