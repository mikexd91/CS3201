#include "SuchThatClause.h"
#include "PQLExceptions.h"
#include "Utils.h"
#include <string>
#include <map>

using std::map;
using std::string;
using namespace stringconst;

SuchThatClause::SuchThatClause(ClauseType type){
	this->clauseType = type;
}

SuchThatClause::~SuchThatClause(void){
}

ClauseType SuchThatClause::getClauseType() {
	return clauseType;
}

void SuchThatClause::setFirstArg(string first){
	this->firstArg = first;
}

void SuchThatClause::setFirstArgType(string type){
	this->firstArgType = type;
}

void SuchThatClause::setFirstArgFixed(bool fixed){
	this->firstArgFixed = fixed;
}

void SuchThatClause::setSecondArg(string second){
	this->secondArg = second;
}

void SuchThatClause::setSecondArgType(string type){
	this->secondArgType = type;
}

void SuchThatClause::setSecondArgFixed(bool fixed){
	this->secondArgFixed = fixed;
}

string SuchThatClause::getFirstArg(void){
	return this->firstArg;
}

string SuchThatClause::getFirstArgType(void){
	return this->firstArgType;
}

string SuchThatClause::getSecondArg(void){
	return this->secondArg;
}

string SuchThatClause::getSecondArgType(void){
	return this->secondArgType;
}

bool SuchThatClause::getFirstArgFixed(void){
	return this->firstArgFixed;
}

bool SuchThatClause::getSecondArgFixed(void){
	return this->secondArgFixed;
}

bool SuchThatClause::isBaseValidityCheck() {
	bool firstSynValidity = true;
	bool secondSynValidity = true;

	if(firstArgFixed) {
		if(firstArgType == ARG_PROGLINE || firstArgType == ARG_STATEMENT) {
			firstSynValidity = isValidStmtNumber(firstArg);
		} else if(firstArgType == ARG_PROCEDURE) {
			firstSynValidity = isValidProcedure(firstArg);
		} else if(firstArgType == ARG_IF) {
			firstSynValidity = isValidIf(firstArg);
		} else if(firstArgType == ARG_WHILE) {
			firstSynValidity = isValidWhile(firstArg);
		} else if(firstArgType == ARG_ASSIGN) {
			firstSynValidity = isValidAssign(firstArg);
		}
	}

	if(secondArgFixed) {
		if(secondArgType == ARG_PROGLINE || secondArgType == ARG_STATEMENT) {
			secondSynValidity = isValidStmtNumber(secondArg);
		} else if(secondArgType == ARG_VARIABLE) {
			secondSynValidity = isValidVariable(secondArg);
		} else if(secondArgType == ARG_PROCEDURE) {
			secondSynValidity = isValidProcedure(secondArg);
		} else if(secondArgType == ARG_CONSTANT) {
			secondSynValidity = isValidConstant(secondArg);
		}
	}

	return firstSynValidity && secondSynValidity;
}

bool SuchThatClause::isValidStmtNumber(string stmt) {
	StmtTable* stmtTable = StmtTable::getInstance();
	size_t stmtNum = atoi(stmt.c_str());
	int totalNumStatements = stmtTable->getAllStmts().size();
	return (stmtNum <= stmtTable->getAllStmts().size()) && (stmtNum > 0);
}

bool SuchThatClause::isValidVariable(string var) {
	VarTable* varTable = VarTable::getInstance();
	return varTable->contains(var);
}

bool SuchThatClause::isValidProcedure(string proc) {
	ProcTable* procTable = ProcTable::getInstance();
	return procTable->contains(proc);
}

bool SuchThatClause::isValidConstant(string constant) {
	ConstTable* constTable = ConstTable::getInstance();
	return constTable->contains(constant);
}

bool SuchThatClause::isValidIf(string ifStr) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(ifStr.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == IF_STMT_;
}

bool SuchThatClause::isValidAssign(string assign) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(assign.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == ASSIGN_STMT_;
}

bool SuchThatClause::isValidWhile(string whileStr) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(whileStr.c_str());
	return stmtTable->getStmtObj(stmtNum)->getType() == WHILE_STMT_;
}

bool SuchThatClause::evaluate(Result* res) {
	if(!isBaseValidityCheck() || !isValid()) {
		return false;
	}

	bool isFirstFixed = this->getFirstArgFixed();
	bool isSecondFixed = this->getSecondArgFixed();
	string firstArgSyn = this->getFirstArg();
	string secondArgSyn = this->getSecondArg();
	string firstArgType = this->getFirstArgType();
	string secondArgType = this->getSecondArgType();
	if (this->isValid()) {
		//Parent(1,2)
		if (isFirstFixed && isSecondFixed) {
			NoSynInsert insert = NoSynInsert();
			insert.setPass(evaluateS1FixedS2Fixed(firstArgSyn, secondArgSyn));
			return res->push(insert);
		//Parent(_,_)
		} else if (firstArgType == stringconst::ARG_GENERIC && secondArgType == stringconst::ARG_GENERIC) {
			NoSynInsert insert = NoSynInsert();
			insert.setPass(evaluateS1GenericS2Generic());
			return res->push(insert);
		//Parent(_,2)
		} else if (firstArgType == stringconst::ARG_GENERIC && isSecondFixed) {
			NoSynInsert insert = NoSynInsert();
			insert.setPass(evaluateS1GenericS2Fixed(secondArgSyn));
			return res->push(insert);
		//Parent(2,_)
		} else if (isFirstFixed && secondArgType == stringconst::ARG_GENERIC) {
			NoSynInsert insert = NoSynInsert();
			insert.setPass(evaluateS1FixedS2Generic(firstArgSyn));
			return res->push(insert);
		//Parent(2,a) or Parent (_,a)
		} else if (!isSecondFixed && secondArgType != stringconst::ARG_GENERIC && (isFirstFixed || firstArgType == stringconst::ARG_GENERIC)) {
			//find if second syn is in table
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(secondArgSyn);
			bool hasSecondSynInTable = res->isSynPresent(secondArgSyn);
			//second syn is in table
			if (hasSecondSynInTable) {
				//get values of a from the table
				unordered_set<string> secondValues = res->getSyn(secondArgSyn);
				//for each value of a, if it passes (Parent (2,a) is correct or Parent(_,a) is corect), add it back to table (insertSingleSyn)
				for (unordered_set<string>::iterator it = secondValues.begin(); it != secondValues.end(); ++it) {
					string secondArgValue = *it;
					bool fixedPass = isFirstFixed && evaluateS1FixedS2Fixed(firstArgSyn, secondArgValue);
					bool genericPass = !isFirstFixed && evaluateS1GenericS2Fixed(secondArgValue);
					if (fixedPass || genericPass) {
						insert.insertValue(secondArgValue);
					}
				}
				//second syn is not in table
			} else {
				//generate all possible values of a
				unordered_set<string> secondValues;
				if (isFirstFixed) {
					secondValues = getAllS2WithS1Fixed(firstArgSyn);
				} else {
					secondValues = getAllS2();
				}
				//for each value of a, add it back to table 
				for (unordered_set<string>::iterator it = secondValues.begin(); it != secondValues.end(); ++it) {
					insert.insertValue(*it);
				}				
			}
			return res->push(insert);
		//Parent(a,2) or Parent(a,_)
		} else if (!isFirstFixed && firstArgType != stringconst::ARG_GENERIC && (isSecondFixed || secondArgType == stringconst::ARG_GENERIC)) {
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(firstArgSyn);
			//find if firstSyn is in table
			bool hasFirstSynInTable = res->isSynPresent(firstArgSyn);
			//firstSyn is in table
			if (hasFirstSynInTable) {
				//get values of a from the table
				unordered_set<string> firstValues = res->getSyn(firstArgSyn);
				//for each value of a, if it passes (Parent (a,2) is correct or Parent(a,_) is corect), add it back to table (insertSingleSyn)
				for (unordered_set<string>::iterator it = firstValues.begin(); it != firstValues.end(); ++it) {
					string firstArgValue = *it;
					bool fixedPass = isSecondFixed && evaluateS1FixedS2Fixed(firstArgValue, secondArgSyn);
					bool genericPass = !isSecondFixed && evaluateS1FixedS2Generic(firstArgValue);
					if (fixedPass || genericPass) {
						insert.insertValue(firstArgValue);
					}
				}
				//firstSyn is not in table, generate all possible values of a
			} else {
				unordered_set<string> firstValues;
				//Parent(a,2)
				if (isSecondFixed) {
					firstValues = getAllS1WithS2Fixed(secondArgSyn);
					//Parent(a,_)
				} else {
					firstValues = getAllS1();
				}
				//for each value of a, add it back to table 
				for (unordered_set<string>::iterator firstValue = firstValues.begin(); firstValue != firstValues.end(); ++firstValue) {
					insert.insertValue(*firstValue);
				}				
			}
			return res->push(insert);
			//Parent(a1,a2)
		} else if(!isFirstFixed && firstArgType != stringconst::ARG_GENERIC && !isSecondFixed && secondArgType != stringconst::ARG_GENERIC) {
			MultiSynInsert insert = MultiSynInsert();
			string synArray[] = {firstArgSyn, secondArgSyn};
			vector<string> syns = vector<string>(synArray, synArray+2);
			insert.setSyns(syns);
			bool isFirstInTable = res->isSynPresent(firstArgSyn);
			bool isSecondInTable = res->isSynPresent(secondArgSyn);
			//both are in table
			if (isFirstInTable && isSecondInTable) {
				//get all a1 and a2 value 
				vector<string> synList;
				synList.push_back(firstArgSyn);
				synList.push_back(secondArgSyn);
				unordered_set<vector<string>> pairs = res->getMultiSyn(synList);
				//check if true, if true insert into table 
				for (auto iter = pairs.begin(); iter != pairs.end(); iter++) {
					vector<string> currentRow = *iter;
					string firstValue = currentRow.at(0);
					string secondValue = currentRow.at(1);
					if (evaluateS1FixedS2Fixed(firstValue, secondValue)) {
						vector<string> newRow = vector<string>();
						newRow.push_back(firstValue);
						newRow.push_back(secondValue);
						insert.insertValues(newRow);
					}
				}
				//prevent memroy leak
				pairs.clear();
				//left in table
			} else if (isFirstInTable) {
				//get all a1 values
				unordered_set<string> firstValues = res->getSyn(firstArgSyn);
				//for each a1 value, get all a2 values
				for (unordered_set<string>::iterator iter = firstValues.begin(); iter != firstValues.end(); ++iter) {
					string firstValue = *iter;
					unordered_set<string> secondValues = getAllS2WithS1Fixed(firstValue);
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = secondValues.begin(); iter!= secondValues.end(); ++iter) {
						string secondValue = *iter2;
						vector<string> newRow = vector<string>();
						newRow.push_back(firstValue);
						newRow.push_back(secondValue);
						insert.insertValues(newRow);
					}
				}
				//right in table
			} else if (isSecondInTable) {
				//get all a2 values
				unordered_set<string> secondValues = res->getSyn(secondArgSyn);
				//for each a1 value, get all a2 values
				for (unordered_set<string>::iterator iter = secondValues.begin(); iter != secondValues.end(); ++iter) {
					string secondValue = *iter;
					unordered_set<string> firstValues = getAllS2WithS1Fixed(secondValue);
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = firstValues.begin(); iter!= firstValues.end(); ++iter) {
						string firstValue = *iter2;
						vector<string> newRow = vector<string>();
						newRow.push_back(firstValue);
						newRow.push_back(secondValue);
						insert.insertValues(newRow);
					}
				}
				//both absent in table
			} else {
				/**
				//generate all a1 and a2 values
				Results::ResultsTable* firstSecondValues = getAllS1AndS2();
				for (Results::ResultsTable::iterator iter = firstSecondValues->begin();
					iter != firstSecondValues->end();
					++iter) {
						Results::Row* row = *iter;
						string firstValue = (*row)[firstArgSyn];
						string secondValue = (*row)[secondArgSyn];
						res->insertMultiResult(row);
				}
				firstSecondValues->clear();
				**/
			}
			return res->push(insert);
		} else {
			NoSynInsert insert = NoSynInsert();
			insert.setPass(false);
			return res->push(insert);
			//throw error, all cases should be covered
			//or we should improve the conditions more to prevent such a situtation from happening
		}
	} else {
		NoSynInsert insert = NoSynInsert();
		insert.setPass(false);
		return res->push(insert);
	}
}
