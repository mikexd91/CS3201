#include "CallsStarClause.h"

using namespace stringconst;

CallsStarClause::CallsStarClause(void) : SuchThatClause(CALLSSTAR_) {
	procTable = ProcTable::getInstance();
 }


CallsStarClause::~CallsStarClause(void) {
}

bool CallsStarClause::isValid(void) {
	bool isValidFirstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_PROCEDURE);
	bool isValidSecondArg = (secondArgType == ARG_GENERIC) || (secondArgType == ARG_PROCEDURE);
	return isValidFirstArg && isValidSecondArg;
}

//e.g. Calls*(proc1, proc2)
bool CallsStarClause::evaluateS1FixedS2Fixed(string proc1, string proc2) {
	if (!isProcObjNull(proc1) && !isProcObjNull(proc2)) {
		Procedure* procObj1 = procTable->getProcObj(proc1);
		Procedure* procObj2 = procTable->getProcObj(proc2);
		return isCallsStar(*procObj1, *procObj2);
	}
	return false;
}

//e.g. Calls*(_,_)
bool CallsStarClause::evaluateS1GenericS2Generic() {
	unordered_set<Procedure*> procSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		int calledBySize = procObj->getCalledBy().size();
		int callsSize = procObj->getCalls().size();
		if (calledBySize > 0 || callsSize > 0) {
			return true;
		}
	}
	return false;
}

//e.g. Calls*(_,string)
bool CallsStarClause::evaluateS1GenericS2Fixed(string proc) {
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		int calledBySize = procObj->getCalledBy().size();
		if (calledBySize > 0) {
			return true;
		}
	}
	return false;
}

//Calls*(string,_)
bool CallsStarClause::evaluateS1FixedS2Generic(string proc) {
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		int callsSize = procObj->getCalls().size();
		if (callsSize > 0) {
			return true;
		}
	}
	return false;
}

//Calls*(proc ,s2)
unordered_set<string> CallsStarClause::getAllS2WithS1Fixed(string proc) {
	unordered_set<string> res;
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		getAllS2WithS1FixedHelper(*procObj, res);
	}
	return res;
}

void CallsStarClause::getAllS2WithS1FixedHelper(Procedure& proc, unordered_set<string>& res) {
	unordered_set<Procedure*> procSet = proc.getCalls();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		string name = procObj->getProcName();
		res.insert(name);
		getAllS2WithS1FixedHelper(*procObj, res);
	}
}

//Calls*(_,s2)
unordered_set<string> CallsStarClause::getAllS2() {
	unordered_set<string> results;
	unordered_set<Procedure*> procSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		unordered_set<Procedure*> getCalledBySet = procObj->getCalledBy();
		int sizeOfGetCalledBySet = getCalledBySet.size();
		if (sizeOfGetCalledBySet > 0) {
			string name = procObj->getProcName();
			results.insert(name);
		}
	}
	return results;
}

//Calls*(s1, proc)
unordered_set<string> CallsStarClause::getAllS1WithS2Fixed(string proc) {
	unordered_set<string> res;
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		getAllS1WithS2FixedHelper(*procObj, res);
	}
	return res;
}

void CallsStarClause::getAllS1WithS2FixedHelper(Procedure& proc, unordered_set<string>& res) {
	unordered_set<Procedure*> procSet = proc.getCalledBy();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		string name = procObj->getProcName();
		res.insert(name);
		getAllS1WithS2FixedHelper(*procObj, res);
	}
}

//Calls*(s1,__)
unordered_set<string> CallsStarClause::getAllS1() {
	unordered_set<string> results;
	unordered_set<Procedure*> procSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		unordered_set<Procedure*> getCallsSet = procObj->getCalls();
		int sizeOfGetCallsSet = getCallsSet.size();
		if (sizeOfGetCallsSet > 0) {
			string name = procObj->getProcName();
			results.insert(name);
		}
	}
	return results;
}

//Calls*(s1,s2)
unordered_set<vector<string>>  CallsStarClause::getAllS1AndS2() {
	unordered_set<vector<string>>  resTable = unordered_set<vector<string>>();
	if (firstArg != secondArg) {
		unordered_set<Procedure*> procSet = procTable->getAllProcs();
		for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
			Procedure* procObj = *i;
			string name = procObj->getProcName();
			unordered_set<string> calledSet = getAllS2WithS1Fixed(name);
			insertS1AndS2IntoResTable(name, calledSet, resTable);
		}
	}
	return resTable;
}

void CallsStarClause::insertS1AndS2IntoResTable(string procName, unordered_set<string>& calledSet, unordered_set<vector<string>>& resTable) {
	for (unordered_set<string>::iterator i = calledSet.begin(); i != calledSet.end(); ++i) {
		vector<string> pair = vector<string>();
		pair.push_back(procName);
		pair.push_back(*i);
		resTable.insert(pair);
	}
}

// might need a helper class for recursion
bool CallsStarClause::isCallsStar(Procedure& proc1, Procedure& proc2) {
	bool directCall = isCalls(proc1, proc2);
	if (!directCall) {
		unordered_set<Procedure*> getCalledSet = proc2.getCalledBy();
		if (!getCalledSet.empty()) {
			for (unordered_set<Procedure*>::iterator i = getCalledSet.begin(); i != getCalledSet.end(); ++i) {
				Procedure* procObj = *i;
				return isCallsStar(proc1, *procObj);
			}
		} else {
			return false;
		}
	}

	return true;
}

bool CallsStarClause::isCalls(Procedure& proc1, Procedure& proc2) {
	unordered_set<Procedure*> callsSet = proc1.getCalls();
	return callsSet.find(&proc2) != callsSet.end();
}

bool CallsStarClause::isProcObjNull(string proc) {
	Procedure* procObj = procTable->getProcObj(proc);
	if (procObj == NULL) {
		return true;
	} else {
		return false;
	}
}