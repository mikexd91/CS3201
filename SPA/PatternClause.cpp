#include "PatternClause.h"
#include "Utils.h"
#include <boost/foreach.hpp>
#include "boost/lexical_cast.hpp"
#include <iostream>

using namespace stringconst;

PatternClause::PatternClause() {
	this->stmtTable = StmtTable::getInstance();
	this->varTable = VarTable::getInstance();
	this->procTable = ProcTable::getInstance();
}

PatternClause::~PatternClause(void) {
}

string PatternClause::getSyn() {
	return this->syn;
}

string PatternClause::getVar() {
	return this->var;
}

string PatternClause::getVarType() {
	return this->varType;
}

bool PatternClause::isVarFixed() {
	return this->varFixed;
}

void PatternClause::setVarFixed(bool fixed) {
	this->varFixed = fixed;
}

void PatternClause::setVarType(string type) {
	this->varType = type;
}

bool PatternClause::isBaseValidityCheck() {
	bool synValidity = true;
	bool varValidity = true;

	synValidity = (synType == ARG_IF) || (synType == ARG_WHILE) || (synType == ARG_ASSIGN);

	if(varFixed) {
		if(varType == ARG_PROGLINE || varType == ARG_STATEMENT) {
			varValidity = isValidStmtNumber(var);
		} else if(varType == ARG_VARIABLE) {
			varValidity = isValidVariable(var);
		} else if(varType == ARG_PROCEDURE) {
			varValidity = isValidProcedure(var);
		} else if(varType == ARG_CONSTANT) {
			varValidity = isValidConstant(var);
		}
	} else {
		varValidity = (varType == ARG_GENERIC) || (varType == ARG_VARIABLE);
	}

	return synValidity && varValidity;
}

bool PatternClause::isValidStmtNumber(string stmt) {
	StmtTable* stmtTable = StmtTable::getInstance();
	size_t stmtNum = atoi(stmt.c_str());
	int totalNumStatements = stmtTable->getAllStmts().size();
	return (stmtNum <= stmtTable->getAllStmts().size()) && (stmtNum > 0);
}

bool PatternClause::isValidVariable(string var) {
	VarTable* varTable = VarTable::getInstance();
	return varTable->contains(var);
}

bool PatternClause::isValidProcedure(string proc) {
	ProcTable* procTable = ProcTable::getInstance();
	return procTable->contains(proc);
}

bool PatternClause::isValidConstant(string constant) {
	ConstTable* constTable = ConstTable::getInstance();
	return constTable->contains(constant);
}

bool PatternClause::isValidIf(string ifStr) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(ifStr.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == IF_STMT_;
}

bool PatternClause::isValidAssign(string assign) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(assign.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == ASSIGN_STMT_;
}

bool PatternClause::isValidWhile(string whileStr) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(whileStr.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == WHILE_STMT_;
}

unordered_set<string> PatternClause::getAllVarValues() {
	unordered_set<string> allVars = varTable->getAllVarNames();
	return allVars;
}

bool PatternClause::evaluate(Result* res) {

	if(!isBaseValidityCheck() || !isValid()) {
		return false;
	}

	// a("v", ?)
	if (isVarFixed() && getVarType() == ARG_VARIABLE) {
		//if syn is in table
		if (res->isSynPresent(getSyn())) {
			//get values of syn from the table
			unordered_set<string> synValues = res->getSyn(getSyn());
			//for each syn value, if it passes, add it back to table
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(getSyn());
			for (unordered_set<string>::iterator it = synValues.begin(); it != synValues.end(); ++it) {
				string synValue = *it;
				string varValue = getVar();
				// check if this specific syn value matches the pattern of this specific var value (and its exprs)
				if (matchPattern(synValue, varValue)) {
					insert.insertValue(synValue);
				}
			}
			return res->push(insert);
		//else syn is not in table, generate all possible values of a
		} else {
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(this->getSyn());
			unordered_set<string> allSynValues = getAllSynValues();
			string varValue = getVar();
			BOOST_FOREACH(string testSynValue, allSynValues) {
				if (matchPattern(testSynValue, varValue)) {
					insert.insertValue(testSynValue);
				}
			}
			return res->push(insert);
		}

	// a(_, ?)
	} else if (!isVarFixed() && getVarType() == ARG_GENERIC) {
		//if syn is in table
		if (res->isSynPresent(getSyn())) {
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(getSyn());
			//get values of syn from the table
			unordered_set<string> synValues = res->getSyn(getSyn());
			string underscore = getVar();
			//for each syn value, if it passes, add it back to table
			for (unordered_set<string>::iterator it = synValues.begin(); it != synValues.end(); ++it) {
				string synValue = *it;
				// check if this specific syn value matches the pattern of _ var (and its exprs)
				if (matchPattern(synValue, underscore)) {
					insert.insertValue(synValue);
				}
			}
			return res->push(insert);
		//else syn is not in table, generate all possible values of a
		} else {
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(getSyn());
			unordered_set<string> allSynValues = getAllSynValues();
			string underscore = getVar();
			BOOST_FOREACH(string testSynValue, allSynValues) {
				if (matchPattern(testSynValue, underscore)) {
					insert.insertValue(testSynValue);
				}
			}
			return res->push(insert);	
		}

	// a(v, ?)
	} else if (!varFixed && varType == ARG_VARIABLE) {
		MultiSynInsert insert = MultiSynInsert();
		string synArray[] = {getSyn(), getVar()};
		vector<string> syns = vector<string>(synArray, synArray+2);
		insert.setSyns(syns);
		bool isSynInTable = res->isSynPresent(getSyn());
		bool isVarInTable = res->isSynPresent(getVar());
		//both are in table
		if (isSynInTable && isVarInTable) {
			//get all a and v value 
			vector<string> synList;
			synList[0] = (getSyn());
			synList[1] = (getVar());
			unordered_set<vector<string>> pairs = res->getMultiSyn(synList);
			//check if true, if true insert into table 
			BOOST_FOREACH(vector<string> pair, pairs) {
				string synValue = pair[0];
				string varValue = pair[1];
				if (matchPattern(synValue, varValue)) {
					vector<string> newRow = vector<string>();
					newRow.push_back(synValue);
					newRow.push_back(varValue);
					insert.insertValues(newRow);
				}
			}
			//prevent memroy leak
			pairs.clear();
		//only syn in table
		} else if (isSynInTable) {
			//get all a1 values
			unordered_set<string> synValues = res->getSyn(getSyn());
			//for each a1 value, get all a2 values
			for (unordered_set<string>::iterator iter = synValues.begin(); iter != synValues.end(); ++iter) {
				string synValue = *iter;
				unordered_set<string> allVarValues = getAllVarValues();
				BOOST_FOREACH(string testVarValue, allVarValues) {
					if (matchPattern(synValue, testVarValue)) {
						vector<string> newRow = vector<string>();
						newRow.push_back(synValue);
						newRow.push_back(testVarValue);
						insert.insertValues(newRow);
					}
				}
			}
		//only var in table
		} else if (isVarInTable) {
			//get all a2 values
			unordered_set<string> varValues = res->getSyn(getVar());
			//for each a1 value, get all a2 values
			for (unordered_set<string>::iterator iter = varValues.begin(); iter != varValues.end(); ++iter) {
				string varValue = *iter;
				unordered_set<string> allSynValues = getAllSynValues();
				BOOST_FOREACH(string testSynValue, allSynValues) {
					if (matchPattern(testSynValue, varValue)) {
						vector<string> newRow = vector<string>();
						newRow.push_back(testSynValue);
						newRow.push_back(varValue);
						insert.insertValues(newRow);
					}
				}
			}
			//both absent in table
		} else {
			//generate all a1 and a2 values
			unordered_set<string> allSynValues = getAllSynValues();
			unordered_set<string> allVarValues = getAllVarValues();

			BOOST_FOREACH(string testSynValue, allSynValues) {
				BOOST_FOREACH(string testVarValue, allVarValues) {
					if (matchPattern(testSynValue, testVarValue)) {
						vector<string> newRow = vector<string>();
						newRow.push_back(testSynValue);
						newRow.push_back(testVarValue);
						insert.insertValues(newRow);
					}
				}
			}
		}
		return res->push(insert);

	} else {
		// fail, no such combi
		NoSynInsert insert = NoSynInsert();
		insert.setPass(false);
		return res->push(insert);
	}
}
