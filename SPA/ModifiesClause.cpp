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

// Modifies(p, "x") or Modifies(if, "x") or Modifies(w, "x") or Modifies(s, "x");
unordered_set<string> ModifiesClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> results;
	Variable* var = varTable->getVariable(s2);

	if(firstArgType == ARG_PROCEDURE) {
		results = var->getModifiedByProc();
	} else if(firstArgType == ARG_IF) {
		unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
		for(auto i = ifStmts.begin(); i != ifStmts.end(); i++) {
			if(isStmtModifies((*i)->getStmtNum(), s2)) {
				results.insert(boost::lexical_cast<string>((*i)->getStmtNum()));
			}
		}
	} else if(firstArgType == ARG_WHILE) {
		unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
		for(auto i = whileStmts.begin(); i != whileStmts.end(); i++) {
			if(isStmtModifies((*i)->getStmtNum(), s2)) {
				results.insert(boost::lexical_cast<string>((*i)->getStmtNum()));
			}
		}
	} else if(firstArgType == ARG_PROGLINE) {
		results = var->getUsedByStmtsAsString();
	}

	return results;
}

// Modifies(p, _) or Modifies(if, _) or Modifies(w, _) or Modifies(s, _)
unordered_set<string> ModifiesClause::getAllS1() {
	unordered_set<string> results;

	if(firstArgType == ARG_PROCEDURE) {
		
	}

	return results;
}

unordered_set<unordered_map<string, string>> ModifiesClause::getAllS1AndS2() {
	unordered_set<unordered_map<string, string>> results;
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

//bool ModifiesClause::isModifies(int stmtNum, string varName) {
//	StmtTable* stable = StmtTable::getInstance();
//	int maxStmts = stable->getAllStmts().size();
//	if (stmtNum > maxStmts || stmtNum <= 0) {
//		return false;
//	} else {
//		Statement* stmt = stable->getStmtObj(stmtNum);
//
//		set<string> mods = stmt->getModifies();
//
//		return mods.find(varName) != mods.end();
//	}
//}
//bool ModifiesClause::isStmtFixed() {
//	return firstArgFixed;
//}
//
//bool ModifiesClause::isVarFixed() {
//	return secondArgFixed;
//}
//
//string ModifiesClause::getStmt() {
//	return firstArg;
//}
//
//int ModifiesClause::getStmtNum() {
//	if (isStmtFixed()) {
//		return atoi(firstArg.c_str());
//	} else {
//		return 0;
//	}
//}
//
//string ModifiesClause::getVar() {
//	return secondArg;
//}
//
//string ModifiesClause::getStmtType() {
//	return firstArgType;
//}
//
//
//bool ModifiesClause::isValid(void){
//	string firstType = this->getFirstArgType();
//	string secondType = this->getSecondArgType();
//	bool firstArg = (firstType == ARG_GENERIC) || (firstType == ARG_STATEMENT) || (firstType == ARG_ASSIGN) || (firstType == ARG_WHILE) || (firstType == ARG_PROGLINE);
//	bool secondArg = (secondType == ARG_GENERIC) || (secondType == ARG_VARIABLE);
//	return (firstArg && secondArg);
//}
//
//Results ModifiesClause::evaluate(void) {
//
//	/*
//
//	stmt fixed
//		var fixed == set the result to true if true
//		var syn == return all var that mod by stmt
//
//	stmt syn - can be assg, while, plainstmt
//		var fixed == return all stmt that mods this var
//		var syn == return all pair (stmt, var) that mod holds
//
//	*/
//
//	if (isStmtFixed()) {
//		if (isVarFixed()) {
//			return evaluateFixedFixed(getStmtNum(), getVar());
//		} else {
//			return evaluateFixedSyn(getStmtNum(), getVar());
//		}
//	} else {
//		if (isVarFixed()) {
//			return evaluateSynFixed(getStmt(), getVar());
//		} else {
//			return evaluateSynSyn(getStmt(), getVar());
//		}
//	}
//}
//
//Results ModifiesClause::evaluateFixedFixed(int stmtNum, string varName) {
//	Results* res = new Results();
//	res->setClausePassed(isModifies(stmtNum, varName));
//	res->setNumOfSyn(0);
//
//	return *res;
//}
//
//Results ModifiesClause::evaluateFixedSyn(int stmtNum, string var) {
//	Results* res = new Results();
//	res->setFirstClauseSyn(var);
//	if(this->getSecondArgType() != ARG_GENERIC) {
//		res->setNumOfSyn(1);
//	} else {
//		res->setNumOfSyn(0);
//	}
//
//	VarTable* vtable = VarTable::getInstance();
//	vector<string>* allVarNames = vtable->getAllVarNames();
//
//	for (size_t i = 0; i < allVarNames->size(); i++) {
//		if (isModifies(stmtNum, allVarNames->at(i))) {
//			res->addSingleResult(allVarNames->at(i));
//		}
//	}
//
//	res->setClausePassed(res->getSinglesResults().size() > 0);
//
//	return *res;
//}
//
//Results ModifiesClause::evaluateSynFixed(string stmt, string varName) {
//	Results* res = new Results();
//	res->setFirstClauseSyn(stmt);
//	if(this->getFirstArgType() != ARG_GENERIC) {
//		res->setNumOfSyn(1);
//	} else {
//		res->setNumOfSyn(0);
//	}
//
//	StmtTable* stable = StmtTable::getInstance();
//	set<Statement*> allStmt;	
//	
//	if (getStmtType() == ARG_ASSIGN) {
//		allStmt = stable->getAssgStmts();
//	} else if (getStmtType() == ARG_WHILE) {
//		allStmt = stable->getWhileStmts();
//	} else {
//		allStmt = stable->getAllStmts();
//	}
//
//	BOOST_FOREACH(auto p, allStmt) {
//		int stmtNum = p->getStmtNum();
//		if (isModifies(stmtNum, varName)) {
//			res->addSingleResult(lexical_cast<string>(stmtNum));
//		}
//	}
//
//	res->setClausePassed(res->getSinglesResults().size() > 0);
//
//	return *res;
//}
//
//Results ModifiesClause::evaluateSynSyn(string stmt, string var) {
//	bool isFirstGeneric = getStmtType() == ARG_GENERIC;
//	bool isSecondGeneric = getSecondArgType() == ARG_GENERIC;
//	bool isBothGeneric = isFirstGeneric && isSecondGeneric;
//
//	Results* res = new Results();
//	res->setNumOfSyn(0);
//	if (isBothGeneric) {
//		res->setNumOfSyn(0);
//	} else if (isFirstGeneric) {
//		res->setFirstClauseSyn(stmt);
//		res->setNumOfSyn(1);
//	} else if (isSecondGeneric) {
//		res->setFirstClauseSyn(var);
//		res->setNumOfSyn(1);
//	} else {
//		res->setFirstClauseSyn(stmt);
//		res->setSecondClauseSyn(var);
//		res->setNumOfSyn(2);
//	}
//
//	StmtTable* stable = StmtTable::getInstance();
//	set<Statement*> allStmt;
//	
//	VarTable* vtable = VarTable::getInstance();
//	vector<string>* allVarNames = vtable->getAllVarNames();
//	
//	if (getStmtType() == ARG_ASSIGN) {
//		allStmt = stable->getAssgStmts();
//	} else if (getStmtType() == ARG_WHILE) {
//		allStmt = stable->getWhileStmts();
//	} else {
//		allStmt = stable->getAllStmts();
//	}
//
//	set<string>* singles = new set<string>();
//	set<pair<string, string>>* pairs = new set<pair<string, string>>();
//
//	BOOST_FOREACH(auto p, allStmt) {
//		int stmtNum = p->getStmtNum();
//
//		//BOOST_FOREACH(string varName, allVarNames) {
//		for (size_t i = 0; i < allVarNames->size(); i++) {
//			string varName = allVarNames->at(i);
//			if (isModifies(stmtNum, varName)) {
//				if (isBothGeneric) {
//					pair<string, string> pr = pair<string, string>(lexical_cast<string>(stmtNum), lexical_cast<string>(varName));
//					pairs->emplace(pr);
//				} else if (isFirstGeneric) {
//					singles->emplace(lexical_cast<string>(stmtNum));
//				} else if (isSecondGeneric) {
//					singles->emplace(lexical_cast<string>(varName));
//				} else {
//					pair<string, string> pr = pair<string, string>(lexical_cast<string>(stmtNum), lexical_cast<string>(varName));
//					pairs->emplace(pr);
//				}
//			}
//		}
//	}
//
//	// transfer from set to vector
//	if (isBothGeneric) {
//		transferPairsToResult(pairs, res);
//	} else if (isFirstGeneric) {
//		transferSinglesToResult(singles, res);
//	} else if (isSecondGeneric) {
//		transferSinglesToResult(singles, res);
//	} else {
//		transferPairsToResult(pairs, res);
//	}
//
//	return *res;
//}
//

//
//void ModifiesClause::transferSinglesToResult(set<string>* singles, Results* res) {
//	
//	BOOST_FOREACH(auto p, *singles) {
//		res->addSingleResult(p);
//	}
//	res->setClausePassed(res->getSinglesResults().size() > 0);
//}
//
//void ModifiesClause::transferPairsToResult(set<pair<string, string>>* pairs, Results* res) {
//	
//	BOOST_FOREACH(auto p, *pairs) {
//		res->addPairResult(p.first, p.second);
//	}
//	res->setClausePassed(res->getPairResults().size() > 0);
//}