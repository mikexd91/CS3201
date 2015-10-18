#include "ModifiesClause.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Utils.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace stringconst;
using namespace boost;

ModifiesClause::ModifiesClause(void):SuchThatClause(MODIFIES_){
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
	varTable = VarTable::getInstance();
}

ModifiesClause::~ModifiesClause(void){

}

bool ModifiesClause::isValid(void) {
	bool isValidFirstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_STATEMENT) 
		|| (firstArgType == ARG_ASSIGN) || (firstArgType == ARG_PROCEDURE) || (firstArgType == ARG_IF)
		|| (firstArgType == ARG_WHILE) || (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_CALL);
	bool isValidSecondArg = (secondArgType == ARG_GENERIC) || (secondArgType == ARG_VARIABLE);

	return isValidFirstArg && isValidSecondArg;
}

// Modifies(1, "x") or Modifies("procedure", "x")
bool ModifiesClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	// Modifies can only take in a fixed proc or a fixed stmt number as the first arg
	if(firstArgType == ARG_PROCEDURE) {
		return isProcedureModifies(s1, s2);
	} else {
		return isStmtModifies(atoi(s1.c_str()), s2);
	}
	
	return false;
}

// Modifies(_, _) 
bool ModifiesClause::evaluateS1GenericS2Generic() {
	unordered_set<Variable*> allVar = varTable->getAllVariables();
	
	BOOST_FOREACH(auto i, allVar) {
		Variable* var = i;
		if(!var->getModifiedByProc().empty() || !var->getModifiedByStmts().empty()) {
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
	} else if(firstArgType == ARG_PROGLINE || firstArgType == ARG_STATEMENT) {
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
	unordered_set<string>::iterator iter;

	if(firstArgType == ARG_PROCEDURE) {
		Procedure* proc = procTable->getProcObj(s1);
		results = proc->getModifies();
	} else {
		Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
		results = stmt->getModifies();
	}
	return results;
}

// Modifies(_, v)
unordered_set<string> ModifiesClause::getAllS2() {
	unordered_set<string> results;

	unordered_set<Variable*> allVar = varTable->getAllVariables();

	BOOST_FOREACH(auto i, allVar) {
		Variable* var = i;
		unordered_set<string> modifiedByProc = var->getModifiedByProc();
		unordered_set<int> modifiedByStmt = var->getModifiedByStmts();

		if(!modifiedByProc.empty() || !modifiedByStmt.empty()) {
			results.insert(var->getName());
		}
	}

	return results;
}

// Modifies(p, "x") or Modifies(if, "x") or Modifies(w, "x") or Modifies(s, "x") or Modifies(a, "x") or Modifies(c, "x")
unordered_set<string> ModifiesClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> results;
	Variable* var = varTable->getVariable(s2);

	if(firstArgType == ARG_PROCEDURE) {
		results = var->getModifiedByProc();
	} else if(firstArgType == ARG_PROGLINE || firstArgType == ARG_STATEMENT) {
		results = var->getModifiedByStmtsAsString();
	} else {
		unordered_set<Statement*> stmts;
		if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_CALL) {
			stmts = stmtTable->getCallStmts();
		} else {
			stmts = stmtTable->getAssgStmts();
		}

		BOOST_FOREACH(auto i, stmts) {
			int stmtNum = (i)->getStmtNum();
			if(isStmtModifies(stmtNum, s2)) {
				results.insert(lexical_cast<string>(stmtNum));
			}
		}
	} 

	return results;
}

// Modifies(p, _) or Modifies(if, _) or Modifies(w, _) or Modifies(s, _) or Modifies(a, _) or Modifies(c, _)
unordered_set<string> ModifiesClause::getAllS1() {
	unordered_set<string> results;

	if(firstArgType == ARG_PROCEDURE) {
		unordered_set<Procedure*> allProc = procTable->getAllProcs();
		BOOST_FOREACH(auto i, allProc) {
			if(!i->getModifies().empty()) {
				results.insert(i->getProcName());
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
		} else if(firstArgType == ARG_CALL) {
			stmts = stmtTable->getCallStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		BOOST_FOREACH(auto s, stmts) {
			if(!s->getModifies().empty()) {
				results.insert(lexical_cast<string>(s->getStmtNum()));
			}
		}
	}

	return results;
}

// Modifies(p, v) or Modifies(s, v) or Modifies(if, v) or Modifies(w, v) or Modifies(c, v)
unordered_set<vector<string>> ModifiesClause::getAllS1AndS2() {
	unordered_set<vector<string>> results = unordered_set<vector<string>>();

	if(firstArgType == ARG_PROCEDURE) {
		unordered_set<Procedure*> allProc = procTable->getAllProcs();
		BOOST_FOREACH(auto i, allProc) {
			Procedure* proc = i;
			unordered_set<string> modifies = proc->getModifies();
			BOOST_FOREACH(auto j, modifies) {
				string var = j;
				vector<string> tuple = vector<string>();
				tuple.push_back(proc->getProcName());
				tuple.push_back(var);
				results.insert(tuple);
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
		} else if(firstArgType == ARG_CALL) {
			stmts = stmtTable->getCallStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		BOOST_FOREACH(auto s, stmts) {
			unordered_set<string> modifies = s->getModifies();

			BOOST_FOREACH(auto v, modifies) {
				vector<string> tuple;
				tuple.push_back(lexical_cast<string>(s->getStmtNum()));
				tuple.push_back(v);
				results.insert(tuple);
			}
		}
	}

	return results;
}

bool ModifiesClause::isProcedureModifies(string proc, string var) {
	Procedure* procedure = procTable->getProcObj(proc);

	unordered_set<string> modifiesSet = procedure->getModifies();
	return modifiesSet.find(var) != modifiesSet.end();
}

bool ModifiesClause::isStmtModifies(int stmtNum, string var) {
	Statement* stmt = stmtTable->getStmtObj(stmtNum);

	unordered_set<string> modifiesSet = stmt->getModifies();
	return modifiesSet.find(var) != modifiesSet.end();
}
