#include "ModifiesClause.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Utils.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace stringconst;
using namespace boost;

StmtTable* stmtTable = StmtTable::getInstance();
ProcTable* procTable = ProcTable::getInstance();
VarTable* varTable = VarTable::getInstance();

ModifiesClause::ModifiesClause(void):Clause(MODIFIES_){
}

ModifiesClause::~ModifiesClause(void){

}

bool ModifiesClause::isValid(void) {
	bool isValidFirstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_STATEMENT) 
		|| (firstArgType == ARG_ASSIGN) || (firstArgType == ARG_PROCEDURE) || (firstArgType == ARG_IF)
		|| (firstArgType == ARG_WHILE) || (firstArgType == ARG_PROGLINE);
	bool isValidSecondArg = (secondArgType == ARG_GENERIC) || (secondArgType == ARG_VARIABLE);

	return isValidFirstArg && isValidSecondArg;
}

// Modifies(1, "x") or Modifies("procedure", "x")
bool ModifiesClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	// Modifies can only take in a fixed proc or a fixed stmt number as the first arg
	if(firstArgType == ARG_PROCEDURE) {
		return isProcedureModifies(s1, s2);
	} else if(firstArgType == ARG_PROGLINE) {
		return isStmtModifies(atoi(s1.c_str()), s2);
	}
	
	return false;
}

// Modifies(_, _) 
bool ModifiesClause::evaluateS1GenericS2Generic() {
	vector<Variable*>* allVar = varTable->getAllVariables();
	vector<Variable*>::iterator iter;
	
	for(iter = allVar->begin(); iter != allVar->end(); iter++) {
		if(!(*iter)->getModifiedByProc().empty() || !(*iter)->getModifiedByStmts().empty()) {
			return true;
		}
	}

	return false;
}

// Modifies(_, "x")
bool ModifiesClause::evaluateS1GenericS2Fixed(string s2) {
	Variable* varObj = varTable->getVariable(s2);
	
	if(!varObj->getModifiedByProc().empty() || !varObj->getModifiedByStmts().empty()) {
		return true;
	}

	return false;
}

// Modifies(1, _) or Modifies("procedure", _)
bool ModifiesClause::evaluateS1FixedS2Generic(string s1) {
	if(firstArgType == ARG_PROCEDURE) {
		Procedure* proc = procTable->getProcObj(s1);
		if(!proc->getModifies().empty()) {
			return true;
		}
	} else if(firstArgType == ARG_PROGLINE) {
		Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
		if(!stmt->getModifies().empty()) {
			return true;
		}
	}
	
	return false;
}

// Modifies("procedure", v) or Modifies(1, v);
unordered_set<string> ModifiesClause::getAllS2WithS1Fixed(string s1) {
	unordered_set<string> results;
	unordered_set<string> modifiesVar;
	unordered_set<string>::iterator iter;

	if(firstArgType == ARG_PROCEDURE) {
		Procedure* proc = procTable->getProcObj(s1);
		modifiesVar = proc->getModifies();
	} else {
		Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
		modifiesVar = stmt->getModifies();
	}

	for(iter = modifiesVar.begin(); iter != modifiesVar.end(); iter++) {
		results.insert(*iter);
	}

	return results;
}

// Modifies(_, v)
unordered_set<string> ModifiesClause::getAllS2() {
	unordered_set<string> results;

	vector<Variable*>* allVar = varTable->getAllVariables();
	vector<Variable*>::iterator iter;

	for(iter = allVar->begin(); iter != allVar->end(); iter++) {
		Variable* var = *iter;
		unordered_set<string> modifiedByProc = var->getModifiedByProc();
		unordered_set<int> modifiedByStmt = var->getModifiedByStmts();

		for(auto i = modifiedByProc.begin(); i != modifiedByProc.end(); i++) {
			results.insert(*i);
		}

		for(auto j = modifiedByStmt.begin(); j != modifiedByStmt.end(); j++) {
			results.insert(boost::lexical_cast<string>(*j));
		}
	}

	return results;
}

// Modifies(p, "x") or Modifies(if, "x") or Modifies(w, "x") or Modifies(s, "x") or Modifies(a, "x")
unordered_set<string> ModifiesClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> results;
	Variable* var = varTable->getVariable(s2);

	if(firstArgType == ARG_PROCEDURE) {
		results = var->getModifiedByProc();
	} else if(firstArgType == ARG_PROGLINE) {
		results = var->getUsedByStmtsAsString();
	} else {
		unordered_set<Statement*> stmts;
		if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else {
			stmts = stmtTable->getAssgStmts();
		}

		for(auto i = stmts.begin(); i != stmts.end(); i++) {
			int stmtNum = (*i)->getStmtNum();
			if(isStmtModifies(stmtNum, s2)) {
				results.insert(lexical_cast<string>(stmtNum));
			}
		}
	} 

	return results;
}

// Modifies(p, _) or Modifies(if, _) or Modifies(w, _) or Modifies(s, _) or Modifies(a, _)
unordered_set<string> ModifiesClause::getAllS1() {
	unordered_set<string> results;

	if(firstArgType == ARG_PROCEDURE) {
		unordered_set<Procedure*> allProc = procTable->getAllProcs();
		for(auto i = allProc.begin(); i != allProc.end(); i++) {
			if(!(*i)->getModifies().empty()) {
				results.insert((*i)->getProcName());
			}
		}
	} else {
		unordered_set<Statement*> stmts;
		if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_ASSIGN) {
			stmts = stmtTable->getAssgStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		for(auto i = stmts.begin(); i != stmts.end(); i++) {
			if(!(*i)->getModifies().empty()) {
				results.insert(lexical_cast<string>((*i)->getStmtNum()));
			}
		}
	}

	return results;
}

// Modifies(p, v) or Modifies(s, v) or Modifies(if, v) or Modifies(w, v)
Results::ResultsTable* ModifiesClause::getAllS1AndS2() {
	Results::ResultsTable* results = new Results::ResultsTable();

	if(firstArgType == ARG_PROCEDURE) {
		unordered_set<Procedure*> allProc = procTable->getAllProcs();
		for(auto i = allProc.begin(); i != allProc.end(); i++) {
			Procedure* proc = *i;
			unordered_set<string> modifies = proc->getModifies();
			for(auto j = modifies.begin(); j != modifies.end(); j++) {
				string var = *j;
				Results::Row* tuple = new Results::Row();
				(*tuple)[firstArg] = proc->getProcName();
				(*tuple)[secondArg] = var;
				results->insert(tuple);
			}
		}
	} else {
		unordered_set<Statement*> stmts;
		if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_ASSIGN) {
			stmts = stmtTable->getAssgStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		for(auto i = stmts.begin(); i != stmts.end(); i++) {
			Statement* statement = *i;
			unordered_set<string> modifies = statement->getModifies();
			for(auto j = modifies.begin(); j != modifies.end(); j++) {
				string var = *j;
				Results::Row* tuple = new Results::Row();
				(*tuple)[firstArg] = lexical_cast<string>(statement->getStmtNum());
				(*tuple)[secondArg] = var;
				results->insert(tuple);
			}
		}
	}

	return results;
}

bool ModifiesClause::isProcedureModifies(string proc, string var) {
	return false;
}

bool ModifiesClause::isStmtModifies(int stmtNum, string var) {
	StmtTable* stmtTable = StmtTable::getInstance();
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	
	unordered_set<string> modifiesSet = stmt->getModifies();
	return modifiesSet.find(var) != modifiesSet.end();
}
