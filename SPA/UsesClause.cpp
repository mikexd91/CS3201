#include "UsesClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Results.h"

#include "boost\lexical_cast.hpp"
#include "boost\unordered_set.hpp"

using namespace boost;
using namespace std;
using namespace stringconst;
using boost::unordered_set;

StmtTable* stmtTable = StmtTable::getInstance();
VarTable* varTable = VarTable::getInstance();
ProcTable* procTable = ProcTable::getInstance();

UsesClause::UsesClause(void):Clause(USES_){
}

UsesClause::~UsesClause(void){
}

bool UsesClause::isValid(){
	*string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == ARG_GENERIC) 
		|| (firstType == ARG_STATEMENT) 
		|| (firstType == ARG_ASSIGN) 
		|| (firstType == ARG_WHILE) 
		|| (firstType == ARG_PROGLINE)
		|| (firstType == ARG_PROCEDURE);
	bool secondArg = (secondType == ARG_GENERIC) 
		|| (secondType == ARG_VARIABLE);
	return (firstArg && secondArg);*/
}

//e.g. Uses(string,string)
bool UsesClause::evaluateS1FixedS2Fixed(string s1, string s2) {

	// Uses(s,v) - statement uses
	if(firstArgType==ARG_STATEMENT || firstArgType==ARG_ASSIGN || firstArgType==ARG_WHILE || firstArgType==ARG_PROGLINE) {
		// get stmt object
		Statement* stmt = stmtTable.getStmtObj(s1);

		// get set of variables stmt uses
		Statement::UsesSet stmtUses = stmt->getUses();
					
		// checks if stmt uses variable
		if(stmtUses.find(s2) != stmtUses.end()) {
			return true;
		} else {
			return false;
		}

	// Uses(p,v) - procedure uses
	} else {
		// get proc object
		Procedure* proc = procTable.getProcObj(s1);

		// get set of variables proc uses
		Procedure::UsesSet procUses = proc->getUses();

		// checks if proc uses variable
		if(procUses.find(s2) != stmtUses.end()) {
			return true;
		} else {
			return false;
		}
	}
}

//e.g. Parent(_,_)
bool UsesClause::evaluateS1GenericS2Generic() {

	// get all variables
	vector<Variable*>* allVars = varTable.getAllVariables();

	// check if any of variables are being used (by stmt or proc)
	for(auto i=allVars.begin(); i!=allVars.end(); i++) {
		unordered_set<int> usedByStmts = i->getUsedByStmts();
		unordered_set<string> usedByProc = i->getUsedByProc();

		if(!usedByStmts.empty() || !usedByProc.empty()) {
			return true;
		}
	}

	return false;
}

//e.g. Uses(_,string)
bool UsesClause::evaluateS1GenericS2Fixed(string s2) {
	
	// get var object
	Variable* var = varTable.getVariable(s2);

	unordered_set<int> usedByStmts = var->getUsedByStmts();
	unordered_set<string> usedByProc = var->getUsedByProc();

	if(usedByStmts.empty() && usedByProc.empty()) {
		return false;
	} else {
		return true;
	}
}

//Uses(string,_)
bool UsesClause::evaluateS1FixedS2Generic(string s1) {

	// Uses(s,v) - statement uses
	if(firstArgType==ARG_STATEMENT || firstArgType==ARG_ASSIGN || firstArgType==ARG_WHILE || firstArgType==ARG_PROGLINE) {
		// get stmt object
		Statement* stmt = stmtTable.getStmtObj(s1);

		// get set of variables stmt uses
		Statement::UsesSet stmtUses = stmt->getUses();
					
		// checks if stmt uses anything
		return !stmtUses.empty();

	// Uses(p,v) - procedure uses
	} else {
		// get proc object
		Procedure* proc = procTable.getProcObj(s1);

		// get set of variables proc uses
		Procedure::UsesSet procUses = proc->getUses();

		// checks if proc uses variable
		return !procUses.empty();
	}
}

//Uses(string,s2)
unordered_set<string> UsesClause::getAllS2WithS1Fixed(string) {

	// Uses(s,v) - statement uses
	if(firstArgType==ARG_STATEMENT || firstArgType==ARG_ASSIGN || firstArgType==ARG_WHILE || firstArgType==ARG_PROGLINE) {
		// get stmt object
		Statement* stmt = stmtTable.getStmtObj(s1);

		// get set of variables stmt uses
		Statement::UsesSet stmtUses = stmt->getUses();
					
		return stmtUses;

	// Uses(p,v) - procedure uses
	} else {
		// get proc object
		Procedure* proc = procTable.getProcObj(s1);

		// get set of variables proc uses
		Procedure::UsesSet procUses = proc->getUses();

		return procUses;
	}
}

//Uses(_,s2)
unordered_set<string> getAllS2() {

	unordered_set<string> varNames = unordered_set<string>();

	// get all variables
	vector<Variable*>* varTable.getAllVariables();

	// for each variable, check if it is used by anything



}

//Uses(s1,string)
unordered_set<string> getAllS1WithS2Fixed(string) {

	// get var object
	Variable* var = varTable.getVariable(s2);

	if(firstArgType==ARG_STATEMENT || firstArgType==ARG_ASSIGN || firstArgType==ARG_WHILE || firstArgType==ARG_PROGLINE) {
		unordered_set<string> usedByStmts = var->getUsedByStmtsAsString();
		return usedByStmts;
	} else {
		unordered_set<string> usedByProc = var->getUsedByProc();
		return usedByProc;
	}
}


//Parent(s1,__)
unordered_set<string> getAllS1();


//Parent(s1,s2)
unordered_set<unordered_map<string, string>> getAllS1AndS2();