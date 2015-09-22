#include "UsesClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Results.h"

#include "boost\lexical_cast.hpp"
#include "boost\unordered_set.hpp"
#include "boost\foreach.hpp"

using namespace boost;
using namespace std;
using namespace stringconst;
using boost::unordered_set;

UsesClause::UsesClause(void) : SuchThatClause(USES_) {
	stmtTable = StmtTable::getInstance();
	varTable = VarTable::getInstance();
	procTable = ProcTable::getInstance();
	
}

UsesClause::~UsesClause(void){
}

bool UsesClause::isValid(){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == ARG_GENERIC) 
		|| (firstType == ARG_STATEMENT) 
		|| (firstType == ARG_ASSIGN) 
		|| (firstType == ARG_WHILE) 
		|| (firstType == ARG_IF)
		|| (firstType == ARG_PROGLINE)
		|| (firstType == ARG_PROCEDURE);
	bool secondArg = (secondType == ARG_GENERIC) 
		|| (secondType == ARG_VARIABLE);
	return (firstArg && secondArg);
}

//e.g. Uses(1, "x") or Uses("proc", "x")
bool UsesClause::evaluateS1FixedS2Fixed(string s1, string s2) {

	// Uses(s,v) - statement uses
	if(isStmtType(firstArgType)) {
		return isStmtUses(lexical_cast<int>(s1), s2);

	// Uses(p,v) - procedure uses
	} else {
		return isProcedureUses(s1, s2);
	}
}

//e.g. Uses(_,_)
bool UsesClause::evaluateS1GenericS2Generic() {

	// get all variables
	vector<Variable*>* allVars = varTable->getAllVariables();

	// check if any of variables are being used (by stmt or proc)
	for(vector<Variable*>::iterator i=allVars->begin(); i!=allVars->end(); i++) {
		unordered_set<int> usedByStmts = (*i)->getUsedByStmts();
		unordered_set<string> usedByProc = (*i)->getUsedByProc();

		if(!usedByStmts.empty() || !usedByProc.empty()) {
			return true;
		}
	}

	return false;
}

//e.g. Uses(_, "x")
bool UsesClause::evaluateS1GenericS2Fixed(string s2) {
	Variable* var = varTable->getVariable(s2);

	unordered_set<string> modifiedBy = var->getUsedByProc();
	unordered_set<int> usedBy = var->getUsedByStmts();

	if(!modifiedBy.empty() || !usedBy.empty()) {
		return true;
	}

	return false;
}

//Uses(1, _) or Uses("proc", _)
bool UsesClause::evaluateS1FixedS2Generic(string s1) {

	// Uses(s,v) - statement uses
	if(isStmtType(firstArgType)) {
		// get stmt object
		Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s1));

		// get set of variables stmt uses
		Statement::UsesSet stmtUses = stmt->getUses();
					
		// checks if stmt uses anything
		return !stmtUses.empty();

	// Uses(p,v) - procedure uses
	} else {
		// get proc object
		Procedure* proc = procTable->getProcObj(s1);

		// get set of variables proc uses
		Procedure::UsesSet procUses = proc->getUses();

		// checks if proc uses variable
		return !procUses.empty();
	}
}

//Uses(1, v) or Uses("proc", v)
unordered_set<string> UsesClause::getAllS2WithS1Fixed(string s1) {

	// Uses(s,v) - statement uses
	if(isStmtType(firstArgType)) {
		// get stmt object
		Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s1));

		// get set of variables stmt uses
		Statement::UsesSet stmtUses = stmt->getUses();
					
		return stmtUses;

	// Uses(p,v) - procedure uses
	} else {
		// get proc object
		Procedure* proc = procTable->getProcObj(s1);

		// get set of variables proc uses
		Procedure::UsesSet procUses = proc->getUses();

		return procUses;
	}
}

//Uses(_,s2)
unordered_set<string> UsesClause::getAllS2() {

	// container for result
	unordered_set<string> vars = unordered_set<string>();

	// get all stmts and procs
	unordered_set<Statement*> stmts = stmtTable->getAllStmts();
	unordered_set<Procedure*> procs = procTable->getAllProcs();

	// consolidate stmt variables
	BOOST_FOREACH(auto s, stmts) {
		Statement::UsesSet uses = s->getUses();
		BOOST_FOREACH(auto v, uses) {
			vars.insert(v);
		}
	}

	// consolidate proc variables
	BOOST_FOREACH(auto p, procs) {
		Procedure::UsesSet uses = p->getUses();
		BOOST_FOREACH(auto v, vars) {
			vars.insert(v);
		}
	}

	return vars;
}

//Uses(s1,string)
unordered_set<string> UsesClause::getAllS1WithS2Fixed(string s2) {

	// get var object
	Variable* var = varTable->getVariable(s2);

	//Uses(s,v)
	if(isStmtType(firstArgType)) {
		// container for result
		unordered_set<string> result = unordered_set<string>();

		// stmt set to check
		unordered_set<Statement*> stmts;

		// check through the arg types
		if(firstArgType == ARG_ASSIGN) {
			stmts = stmtTable->getAssgStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		// stmts that uses s2
		unordered_set<int> usedBy = var->getUsedByStmts();

		// add those stmts that are of the right type
		BOOST_FOREACH(auto s, usedBy) {
			BOOST_FOREACH(auto st, stmts) {
				if(st->getStmtNum() == s) {
					result.insert(lexical_cast<string>(s));
				}
			}
		}

		return result;

	//Uses(p,v)
	} else {
		unordered_set<string> usedByProc = var->getUsedByProc();
		return usedByProc;
	}
}

//Uses(s1,__)
unordered_set<string> UsesClause::getAllS1() {

	// container for results
	unordered_set<string> result = unordered_set<string>();

	if(isStmtType(firstArgType)) {
		// stmt set to check
		unordered_set<Statement*> stmts;

		// check through the arg types
		if(firstArgType == ARG_ASSIGN) {
			stmts = stmtTable->getAssgStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		BOOST_FOREACH(auto s, stmts) {
			if(!s->getUses().empty()) {
				result.insert(lexical_cast<string>(s->getStmtNum()));
			}
		}

	// dealing with set of proc
	} else {
		// get all procs
		unordered_set<Procedure*> allProcs = procTable->getAllProcs();

		BOOST_FOREACH(auto p, allProcs) {
			if(!p->getUses().empty()) {
				result.insert(p->getProcName());
			}
		}
	}

	return result;
}

//Uses(s1,s2)
Results::ResultsTable* UsesClause::getAllS1AndS2() {
	Results::ResultsTable* results = new Results::ResultsTable();

	//Uses(s,v)
	if(isStmtType(firstArgType)) {
		// prepare stmt sets
		unordered_set<Statement*> stmts;

		// check through the arg types
		if(firstArgType == ARG_ASSIGN) {
			stmts = stmtTable->getAssgStmts();
		} else if(firstArgType == ARG_WHILE) {
			stmts = stmtTable->getWhileStmts();
		} else if(firstArgType == ARG_IF) {
			stmts = stmtTable->getIfStmts();
		} else {
			stmts = stmtTable->getAllStmts();
		}

		// add each s,v pair
		BOOST_FOREACH(auto s, stmts) {
			Statement::UsesSet vars = s->getUses();
			BOOST_FOREACH(auto v, vars) {
				// create tuple
				Results::Row* tuple = new Results::Row();
				(*tuple)[firstArg] = lexical_cast<string>(s->getStmtNum());
				(*tuple)[secondArg] = v;

				results->insert(tuple);
			}
		}

	//Uses(p,v)
	} else {
		unordered_set<Procedure*> procs = procTable->getAllProcs();

		// add each p,v pair
		BOOST_FOREACH(auto p, procs) {
			Procedure::UsesSet vars = p->getUses();
			BOOST_FOREACH(auto v, vars) {
				// create tuple
				Results::Row* tuple = new Results::Row();
				(*tuple)[firstArg] = p->getProcName();
				(*tuple)[secondArg] = v;

				results->insert(tuple);
			}
		}
	}

	return results;
}

bool UsesClause::isStmtType(string argType) {
	if(firstArgType==ARG_STATEMENT || firstArgType==ARG_ASSIGN || firstArgType==ARG_WHILE || firstArgType==ARG_IF || firstArgType==ARG_PROGLINE) {
		return true;
	} else {
		return false;
	}
}

bool UsesClause::isProcedureUses(string proc, string var) {
	Procedure* procedure = procTable->getProcObj(proc);

	unordered_set<string> usesSet = procedure->getUses();
	return usesSet.find(var) != usesSet.end();
}

bool UsesClause::isStmtUses(int stmtNum, string var) {
	Statement* stmt = stmtTable->getStmtObj(stmtNum);

	unordered_set<string> usesSet = stmt->getUses();
	return usesSet.find(var) != usesSet.end();
}