#include "Clause.h"
#include "PQLExceptions.h"
#include <string>
#include <map>
#include "Utils.h"

using std::map;
using std::string;
using namespace stringconst;

Clause::Clause(void){
	this->firstArgFixed = false;
	this->secondArgFixed = false;
}

Clause::Clause(ClauseType type){
	this->clauseType = type;
}

Clause::~Clause(void){
}

ClauseType Clause::getClauseType() {
	return clauseType;
}

void Clause::setFirstArg(string first){
	this->firstArg = first;
}

void Clause::setFirstArgType(string type){
	this->firstArgType = type;
}

void Clause::setFirstArgFixed(bool fixed){
	this->firstArgFixed = fixed;
}

void Clause::setSecondArg(string second){
	this->secondArg = second;
}

void Clause::setSecondArgType(string type){
	this->secondArgType = type;
}

void Clause::setSecondArgFixed(bool fixed){
	this->secondArgFixed = fixed;
}

string Clause::getFirstArg(void){
	return this->firstArg;
}

string Clause::getFirstArgType(void){
	return this->firstArgType;
}

string Clause::getSecondArg(void){
	return this->secondArg;
}

string Clause::getSecondArgType(void){
	return this->secondArgType;
}

bool Clause::getFirstArgFixed(void){
	return this->firstArgFixed;
}

bool Clause::getSecondArgFixed(void){
	return this->secondArgFixed;
}

bool Clause::isValidityCheck() {
	bool firstSynValidity = true;
	bool secondSynValidity = true;

	if(firstArgFixed) {
		if(firstArgType == ARG_STATEMENT) {
			firstSynValidity = isValidStmtNumber(firstArg);
		}
	}

	if(secondArgFixed) {
		if(secondArgType == ARG_STATEMENT) {
			secondSynValidity = isValidStmtNumber(secondArg);
		} else if(secondArgType == ARG_VARIABLE) {
			secondSynValidity = isValidVariable(secondArg);
		}
	}

	return firstSynValidity && secondSynValidity;
}

bool isValidStmtNumber(string stmt) {
	StmtTable* stmtTable = StmtTable::getInstance();
	int stmtNum = atoi(stmt.c_str());
	return (stmtNum < stmtTable->getAllStmts().size()) || (stmtNum > 0);
}

bool isValidVariable(string var) {
	VarTable* varTable = VarTable::getInstance();
	return varTable->contains(var);
}

bool Clause::evaluate(Results* res) {
	//clause is pass at each stage
	res->setClauseFail();
	if(!isValidityCheck()) {
		return false;
	}
	
	if(!childValidityCheck()) {
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
			if(evaluateS1FixedS2Fixed(firstArgSyn, secondArgSyn)) {
				res->setClausePass();
			} 
		//Parent(_,_)
		} else if (firstArgType == stringconst::ARG_GENERIC && secondArgType == stringconst::ARG_GENERIC) {
			if (evaluateS1GenericS2Generic()) {
				res->setClausePass();
			}
		//Parent(2,a) or Parent (_,a)
		} else if (!isSecondFixed && secondArgType != stringconst::ARG_GENERIC && (isFirstFixed || firstArgType == stringconst::ARG_GENERIC)) {
			//find if second syn is in table
			bool hasSecondSynInTable = res->hasResults(secondArgSyn);
			//second syn is in table
			if (hasSecondSynInTable) {
				//get values of a from the table
				unordered_set<string> secondValues = res->selectSyn(secondArgSyn);
				//for each value of a, if it passes (Parent (2,a) is correct or Parent(_,a) is corect), add it back to table (insertSingleSyn)
				for (unordered_set<string>::iterator it = secondValues.begin(); it != secondValues.end(); ++it) {
					string secondArgValue = *it;
					bool fixedPass = isFirstFixed && evaluateS1FixedS2Fixed(firstArgSyn, secondArgValue);
					bool genericPass = !isFirstFixed && evaluateS1GenericS2Fixed(secondArgValue);
					if (fixedPass || genericPass) {
						res->insertResult(secondArgSyn, secondArgValue);
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
					res->insertResult(secondArgSyn, *it);
				}				
			}
		//Parent(a,2) or Parent(a,_)
		} else if (!isFirstFixed && firstArgType != stringconst::ARG_GENERIC && (isSecondFixed || secondArgType == stringconst::ARG_GENERIC)) {
			//find if firstSyn is in table
			bool hasFirstSynInTable = res->hasResults(firstArgSyn);
			//firstSyn is in table
			if (hasFirstSynInTable) {
				//get values of a from the table
				unordered_set<string> firstValues = res->selectSyn(firstArgSyn);
				//for each value of a, if it passes (Parent (a,2) is correct or Parent(a,_) is corect), add it back to table (insertSingleSyn)
				for (unordered_set<string>::iterator it = firstValues.begin(); it != firstValues.end(); ++it) {
					string firstArgValue = *it;
					bool fixedPass = isSecondFixed && evaluateS1FixedS2Fixed(firstArgValue, secondArgSyn);
					bool genericPass = !isSecondFixed && evaluateS1FixedS2Generic(firstArgValue);
					if (fixedPass || genericPass) {
						res->insertResult(firstArgSyn, firstArgValue);
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
					res->insertResult(firstArgSyn, *firstValue);
				}				
			}
		//Parent(a1,a2)
		} else if(!isFirstFixed && firstArgType != stringconst::ARG_GENERIC && !isSecondFixed && secondArgType != stringconst::ARG_GENERIC) {
			bool isFirstInTable = res->hasResults(firstArgSyn);
			bool isSecondInTable = res->hasResults(secondArgSyn);
			//both are in table
			if (isFirstInTable && isSecondInTable) {
				//get all a1 and a2 value 
				unordered_set<string> synList;
				synList.insert(firstArgSyn);
				synList.insert(secondArgSyn);
				Results::ResultsTable pairs = res->selectMultiSyn(synList);
				//check if true, if true insert into table 
				for (Results::ResultsTable::iterator iter = pairs.begin(); iter != pairs.end(); iter++) {
					Results::Row* currentRow = *iter;
					string firstValue = (*currentRow)[firstArgSyn];
					string secondValue = (*currentRow)[secondArgSyn];
					if (evaluateS1FixedS2Fixed(firstValue, secondValue)) {
						unordered_map<string, string>* newRow = new Results::Row();
						(*newRow)[firstArgSyn] = firstValue;
						(*newRow)[secondArgSyn] = secondValue;
						res->insertMultiResult(newRow);
					}
				}
				//prevent memroy leak
				pairs.clear();
			//left in table
			} else if (isFirstInTable) {
				//get all a1 values
				unordered_set<string> firstValues = res->selectSyn(firstArgSyn);
				//for each a1 value, get all a2 values
				for (unordered_set<string>::iterator iter = firstValues.begin(); iter != firstValues.end(); ++iter) {
					string firstValue = *iter;
					unordered_set<string> secondValues = getAllS2WithS1Fixed(firstValue);
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = secondValues.begin(); iter!= secondValues.end(); ++iter) {
						string secondValue = *iter2;
						unordered_map<string, string>* newRow = new Results::Row();
						(*newRow)[firstArgSyn] = firstValue;
						(*newRow)[secondArgSyn] = secondValue;
						res->insertMultiResult(newRow);
					}
				}
			//right in table
			} else if (isSecondInTable) {
				//get all a2 values
				unordered_set<string> secondValues = res->selectSyn(secondArgSyn);
				//for each a1 value, get all a2 values
				for (unordered_set<string>::iterator iter = secondValues.begin(); iter != secondValues.end(); ++iter) {
					string secondValue = *iter;
					unordered_set<string> firstValues = getAllS2WithS1Fixed(secondValue);
					//add each row of a1 and a2 into the results table
					for (unordered_set<string>::iterator iter2 = firstValues.begin(); iter!= firstValues.end(); ++iter) {
						string firstValue = *iter2;
						unordered_map<string, string>* newRow = new Results::Row();
						(*newRow)[firstArgSyn] = firstValue;
						(*newRow)[secondArgSyn] = secondValue;
						res->insertMultiResult(newRow);
					}
				}
			//both absent in table
			} else {
				//generate all a1 and a2 values
				unordered_set<unordered_map<string, string>> firstSecondValues = getAllS1AndS2();
				for (unordered_set<unordered_map<string, string>>::iterator iter = firstSecondValues.begin();
					iter != firstSecondValues.end();
					++iter) {
					unordered_map<string, string> currentValues = *iter;
					unordered_map<string, string>* newRow = new Results::Row();
					(*newRow)[firstArgSyn] = currentValues[firstArgSyn];
					(*newRow)[secondArgSyn] = currentValues[secondArgSyn];
					res->insertMultiResult(newRow);
				}
			}
		} else {
			//throw error, all cases should be covered
			//or we should improve the conditions more to prevent such a situtation from happening
		}
	} else {
		res->setClauseFail();
	}
	return res->push();
}
