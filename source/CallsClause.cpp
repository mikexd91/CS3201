#include "CallsClause.h"
#include <iostream>
#include <boost/foreach.hpp>

using namespace stringconst;
using namespace std;

CallsClause::CallsClause(void):SuchThatClause(CALLS_){
	procTable = ProcTable::getInstance();
}

CallsClause::~CallsClause(void){
}

bool CallsClause::isValid(void) {
	bool isValidFirstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_PROCEDURE);
	bool isValidSecondArg = (secondArgType == ARG_GENERIC) || (secondArgType == ARG_PROCEDURE);
	//cout << isValidFirstArg << "a" << isValidSecondArg << endl;
	return isValidFirstArg && isValidSecondArg;
}

//e.g. Calls(proc, proc)
bool CallsClause::evaluateS1FixedS2Fixed(string proc1, string proc2) {
	return isCalls(proc1, proc2);
}

//e.g. Calls(_,_)
// There exist a procedure that calls other procedures Or gets called by other procedures
bool CallsClause::evaluateS1GenericS2Generic() {
	unordered_set<Procedure*> procedureSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procedureSet.begin(); i != procedureSet.end(); ++i) {
		Procedure* procObj = *i;
		int numOfProcThatCalledIt = procObj->getCalledBy().size();
		int numOfProcThatItCalled = procObj->getCalls().size();
		if (numOfProcThatCalledIt > 0 || numOfProcThatItCalled > 0) {
			return true;
		}
	}
	return false;
}

//e.g. Calls(_, proc)
bool CallsClause::evaluateS1GenericS2Fixed(string proc) {
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		unordered_set<Procedure*> procSet = procObj->getCalledBy();
		if (procSet.size() > 0) {
			return true;
		}
	}
	return false;
}

//Calls(proc,_)
bool CallsClause::evaluateS1FixedS2Generic(string proc) {
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		unordered_set<Procedure*> procSet = procObj->getCalls();
		if (procSet.size() > 0) {
			return true;
		}
	}
	return false;
}

//Calls(proc, synonym)
unordered_set<string> CallsClause::getAllS2WithS1Fixed(string proc) {
	unordered_set<string> results = unordered_set<string>();
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		unordered_set<Procedure*> procSet = procObj->getCalls();
		for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
			Procedure* procObj = *i;
			string name = procObj->getProcName();
			results.insert(name);
		}
	} 
	return results;
}

//Calls(synonym, proc)
unordered_set<string> CallsClause::getAllS1WithS2Fixed(string proc) {
	unordered_set<string> results = unordered_set<string>();
	if (!isProcObjNull(proc)) {
		Procedure* procObj = procTable->getProcObj(proc);
		unordered_set<Procedure*> procSet = procObj->getCalledBy();
		for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
			Procedure* procObj = *i;
			string name = procObj->getProcName();
			results.insert(name);
		}
	}
	return results;
}

//Calls(_,synonym)
unordered_set<string> CallsClause::getAllS2() {
	unordered_set<string> results = unordered_set<string>();
	unordered_set<Procedure*> procSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		unordered_set<Procedure*> callsSet = procObj->getCalledBy();
		if (!callsSet.empty()) {
			results.insert(procObj->getProcName());
		}
	}
	return results;
}

//Calls(synonym,__)
unordered_set<string> CallsClause::getAllS1() {
	unordered_set<string> results = unordered_set<string>();
	unordered_set<Procedure*> procSet = procTable->getAllProcs();
	for (unordered_set<Procedure*>::iterator i = procSet.begin(); i != procSet.end(); ++i) {
		Procedure* procObj = *i;
		unordered_set<Procedure*> callsSet = procObj->getCalls();
		if (!callsSet.empty()) {
			results.insert(procObj->getProcName());
		}
	}
	return results;
}

//Calls(synonym, synonym)
unordered_set<vector<string>>  CallsClause::getAllS1AndS2() {
	unordered_set<vector<string>> results = unordered_set<vector<string>>();
	if (firstArg != secondArg) {
		unordered_set<Procedure*> procSet = procTable->getAllProcs();
		BOOST_FOREACH(auto i, procSet) {
			unordered_set<Procedure*> callsSet = i->getCalls();
			BOOST_FOREACH(auto j, callsSet) {
				vector<string> synonyms = vector<string>();
				synonyms.push_back(i->getProcName());
				synonyms.push_back(j->getProcName());
				results.insert(synonyms);
			}
		}
	}
	return results;
}

bool CallsClause::isCalls(string proc1, string proc2) {
	if (!isProcObjNull(proc1) && !isProcObjNull(proc2)) {
		Procedure* procObj1 = procTable->getProcObj(proc1);
		Procedure* procObj2 = procTable->getProcObj(proc2);
		unordered_set<Procedure*> procSet = procObj1->getCalls();
		if (procSet.find(procObj2) != procSet.end()) {
			return true;
		}
	} 
	return false;
}

bool CallsClause::isProcObjNull(string proc) {
	Procedure* procObj = procTable->getProcObj(proc);
	if (procObj == NULL) {
		return true;
	} else {
		return false;
	}
}