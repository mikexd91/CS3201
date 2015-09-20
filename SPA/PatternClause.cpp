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
	vector<string> allVarVector = *(varTable->getAllVarNames());
	unordered_set<string> allVarValues;
	BOOST_FOREACH(string val, allVarVector) {
		allVarValues.insert(val);
	}

	return allVarValues;
}

bool PatternClause::evaluate(Results* res) {

	if (!isBaseValidityCheck()) {
		res->setClauseFail();
		return false;
	}

	if (!isValid()) {
		res->setClauseFail();
		return false;
	}

	// a("v", ?)
	if (isVarFixed() && getVarType() == ARG_VARIABLE) {
		//if syn is in table
		if (res->hasResults(getSyn())) {
			//get values of syn from the table
			unordered_set<string> synValues = res->selectSyn(getSyn());
			//for each syn value, if it passes, add it back to table
			for (unordered_set<string>::iterator it = synValues.begin(); it != synValues.end(); ++it) {
				string synValue = *it;
				string varValue = getVar();
				// check if this specific syn value matches the pattern of this specific var value (and its exprs)
				if (matchPattern(synValue, varValue)) {
					res->insertResult(syn, synValue);
				}
			}
		//else syn is not in table, generate all possible values of a
		} else {
			unordered_set<string> allSynValues = getAllSynValues();
			string varValue = getVar();
			BOOST_FOREACH(string testSynValue, allSynValues) {
				if (matchPattern(testSynValue, varValue)) {
					res->insertResult(getSyn(), testSynValue);
				}
			}
		}

	// a(_, ?)
	} else if (!isVarFixed() && getVarType() == ARG_GENERIC) {
		//if syn is in table
		if (res->hasResults(getSyn())) {
			//get values of syn from the table
			unordered_set<string> synValues = res->selectSyn(getSyn());
			string underscore = getVar();
			//for each syn value, if it passes, add it back to table
			for (unordered_set<string>::iterator it = synValues.begin(); it != synValues.end(); ++it) {
				string synValue = *it;
				// check if this specific syn value matches the pattern of _ var (and its exprs)
				if (matchPattern(synValue, underscore)) {
					res->insertResult(getSyn(), synValue);
				}
			}
			//else syn is not in table, generate all possible values of a
		} else {
			unordered_set<string> allSynValues = getAllSynValues();
			string underscore = getVar();
			BOOST_FOREACH(string testSynValue, allSynValues) {
				if (matchPattern(testSynValue, underscore)) {
					res->insertResult(getSyn(), testSynValue);
				}
			}			
		}

	// a(v, ?)
	} else if (!varFixed && varType == ARG_VARIABLE) {
		bool isSynInTable = res->hasResults(getSyn());
		bool isVarInTable = res->hasResults(getVar());
		//both are in table
		if (isSynInTable && isVarInTable) {
			//get all a and v value 
			unordered_set<string> synList;
			synList.insert(getSyn());
			synList.insert(getVar());
			Results::ResultsTable pairs = res->selectMultiSyn(synList);
			//check if true, if true insert into table 
			for (Results::ResultsTable::iterator iter = pairs.begin(); iter != pairs.end(); iter++) {
				Results::Row* currentRow = *iter;
				string synValue = (*currentRow)[getSyn()];
				string varValue = (*currentRow)[getVar()];
				if (matchPattern(synValue, varValue)) {
					unordered_map<string, string>* newRow = new Results::Row();
					(*newRow)[getSyn()] = synValue;
					(*newRow)[getVar()] = varValue;
					res->insertMultiResult(newRow);
				}
			}
			//prevent memroy leak
			pairs.clear();
		//only syn in table
		} else if (isSynInTable) {
			//get all a1 values
			unordered_set<string> synValues = res->selectSyn(getSyn());
			//for each a1 value, get all a2 values
			for (unordered_set<string>::iterator iter = synValues.begin(); iter != synValues.end(); ++iter) {
				string synValue = *iter;
				unordered_set<string> allVarValues = getAllVarValues();
				BOOST_FOREACH(string testVarValue, allVarValues) {
					unordered_set<string> validVarValues;
					if (matchPattern(synValue, testVarValue)) {
						validVarValues.insert(testVarValue);
					}
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = validVarValues.begin(); iter!= validVarValues.end(); ++iter) {
						string validVarValue = *iter2;
						unordered_map<string, string>* newRow = new Results::Row();
						(*newRow)[getSyn()] = synValue;
						(*newRow)[getVar()] = validVarValue;
						res->insertMultiResult(newRow);
					}
				}
			}
		//only var in table
		} else if (isVarInTable) {
			//get all a2 values
			unordered_set<string> varValues = res->selectSyn(getVar());
			//for each a1 value, get all a2 values
			for (unordered_set<string>::iterator iter = varValues.begin(); iter != varValues.end(); ++iter) {
				string varValue = *iter;
				unordered_set<string> allSynValues = getAllSynValues();
				BOOST_FOREACH(string testSynValue, allSynValues) {
					unordered_set<string> validSynValues;
					if (matchPattern(testSynValue, varValue)) {
						validSynValues.insert(testSynValue);
					}
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = validSynValues.begin(); iter!= validSynValues.end(); ++iter) {
						string validSynValue = *iter2;
						unordered_map<string, string>* newRow = new Results::Row();
						(*newRow)[getSyn()] = validSynValue;
						(*newRow)[getVar()] = varValue;
						res->insertMultiResult(newRow);
					}
				}
			}
			//both absent in table
		} else {
			//generate all a1 and a2 values
			unordered_set<string> allSynValues = getAllSynValues();
			unordered_set<string> allVarValues = getAllVarValues();
			Results::ResultsTable* synAndVarValues = new Results::ResultsTable();

			BOOST_FOREACH(string testSynValue, allSynValues) {
				BOOST_FOREACH(string testVarValue, allVarValues) {
					if (matchPattern(testSynValue, testVarValue)) {
						// TODO add pair result;
						Results::Row* pair = new Results::Row();
						(*pair)[getSyn()] = testSynValue;
						(*pair)[getVar()] = testVarValue;
						synAndVarValues->insert(pair);
					}
				}
			}
			for (Results::ResultsTable::iterator iter = synAndVarValues->begin(); iter != synAndVarValues->end(); ++iter) {
					Results::Row* row = *iter;
					string firstValue = (*row)[getSyn()];
					string secondValue = (*row)[getVar()];
					res->insertMultiResult(row);
			}
			synAndVarValues->clear();
		}

	} else {
		// fail, no such combi
		res->setClauseFail();
		return false;
	}

	return res->push();
}
