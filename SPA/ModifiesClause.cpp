#include "ModifiesClause.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Utils.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace stringconst;
using namespace boost;

ModifiesClause::ModifiesClause(void):Clause(MODIFIES_){
}

ModifiesClause::~ModifiesClause(void){

}

bool ModifiesClause::isStmtFixed() {
	return firstArgFixed;
}

bool ModifiesClause::isVarFixed() {
	return secondArgFixed;
}

string ModifiesClause::getStmt() {
	return firstArg;
}

int ModifiesClause::getStmtNum() {
	if (isStmtFixed()) {
		return atoi(firstArg.c_str());
	} else {
		return 0;
	}
}

string ModifiesClause::getVar() {
	return secondArg;
}

string ModifiesClause::getStmtType() {
	return firstArgType;
}


bool ModifiesClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == ARG_GENERIC) || (firstType == ARG_STATEMENT) || (firstType == ARG_ASSIGN) || (firstType == ARG_WHILE) || (firstType == ARG_PROGLINE);
	bool secondArg = (secondType == ARG_GENERIC) || (secondType == ARG_VARIABLE);
	return (firstArg && secondArg);
}

Results ModifiesClause::evaluate(void) {

	/*

	stmt fixed
		var fixed == set the result to true if true
		var syn == return all var that mod by stmt

	stmt syn - can be assg, while, plainstmt
		var fixed == return all stmt that mods this var
		var syn == return all pair (stmt, var) that mod holds

	*/

	if (isStmtFixed()) {
		if (isVarFixed()) {
			return evaluateFixedFixed(getStmtNum(), getVar());
		} else {
			return evaluateFixedSyn(getStmtNum(), getVar());
		}
	} else {
		if (isVarFixed()) {
			return evaluateSynFixed(getStmt(), getVar());
		} else {
			return evaluateSynSyn(getStmt(), getVar());
		}
	}
}

Results ModifiesClause::evaluateFixedFixed(int stmtNum, string varName) {
	Results* res = new Results();
	res->setClausePassed(isModifies(stmtNum, varName));
	res->setNumOfSyn(0);

	return *res;
}

Results ModifiesClause::evaluateFixedSyn(int stmtNum, string var) {
	Results* res = new Results();
	if(this->getSecondArgType() != ARG_GENERIC) {
		res->setFirstClauseSyn(var);
		res->setNumOfSyn(1);
	}

	VarTable* vtable = VarTable::getInstance();
	vector<string>* allVarNames = vtable->getAllVarNames();

	for (size_t i = 0; i < allVarNames->size(); i++) {
		if (isModifies(stmtNum, allVarNames->at(i))) {
			res->addSingleResult(allVarNames->at(i));
		}
	}

	res->setClausePassed(res->getSinglesResults().size() > 0);

	return *res;
}

Results ModifiesClause::evaluateSynFixed(string stmt, string varName) {
	Results* res = new Results();
	if(this->getFirstArgType() != ARG_GENERIC) {
		res->setFirstClauseSyn(stmt);
		res->setNumOfSyn(1);
	}

	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allStmt;	
	
	if (getStmtType() == ARG_ASSIGN) {
		allStmt = stable->getAssgStmts();
	} else if (getStmtType() == ARG_WHILE) {
		allStmt = stable->getWhileStmts();
	} else {
		allStmt = stable->getAllStmts();
	}

	BOOST_FOREACH(auto p, allStmt) {
		int stmtNum = p->getStmtNum();
		if (isModifies(stmtNum, varName)) {
			res->addSingleResult(lexical_cast<string>(stmtNum));
		}
	}

	res->setClausePassed(res->getSinglesResults().size() > 0);

	return *res;
}

Results ModifiesClause::evaluateSynSyn(string stmt, string var) {
	Results* res = new Results();
	res->setFirstClauseSyn(stmt);
	res->setSecondClauseSyn(var);

	// set synonym count
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	if(firstType==ARG_GENERIC && secondType==ARG_GENERIC) {
		res->setNumOfSyn(2);
	} else if(firstType==ARG_GENERIC || secondType==ARG_GENERIC) {
		res->setNumOfSyn(1);
	} else {
		res->setNumOfSyn(0);
	}

	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allStmt;
	
	VarTable* vtable = VarTable::getInstance();
	vector<string>* allVarNames = vtable->getAllVarNames();
	
	if (getStmtType() == ARG_ASSIGN) {
		allStmt = stable->getAssgStmts();
	} else if (getStmtType() == ARG_WHILE) {
		allStmt = stable->getWhileStmts();
	} else {
		allStmt = stable->getAllStmts();
	}

	//cout << endl << "all stmt size " << allStmt.size() << endl;

	BOOST_FOREACH(auto p, allStmt) {
		int stmtNum = p->getStmtNum();

		//BOOST_FOREACH(string varName, allVarNames) {
		for (size_t i = 0; i < allVarNames->size(); i++) {
			string varName = allVarNames->at(i);
			if (isModifies(stmtNum, varName)) {
				res->addPairResult(lexical_cast<string>(stmtNum), varName);
			}
		}
	}

	res->setClausePassed(res->getPairResults().size() > 0);

	return *res;
}

bool ModifiesClause::isModifies(int stmtNum, string varName) {
	StmtTable* stable = StmtTable::getInstance();
	int maxStmts = stable->getAllStmts().size();
	if (stmtNum > maxStmts) {
		return false;
	} else {
		Statement* stmt = stable->getStmtObj(stmtNum);

		set<string> mods = stmt->getModifies();

		return mods.find(varName) != mods.end();
	}
}
