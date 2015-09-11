#include "PatternClause.h"
#include "Utils.h"

using namespace stringconst;

PatternClause::~PatternClause(void) {
}

string PatternClause::getSynonym() {
	return this->firstArg;
}

string PatternClause::getVar() {
	return this->secondArg;
}

string PatternClause::getVarType() {
	return this->secondArgType;
}

bool PatternClause::getVarFixed() {
	return this->secondArgFixed;
}

void PatternClause::setVar(string var) {
	this->secondArg = var;
}

void PatternClause::setVarType(string varType) {
	this->secondArgType = varType;
}

void PatternClause::setVarFixed(bool fixed) {
	this->secondArgFixed = fixed;
}

bool PatternClause::isBaseValidityCheck() {
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

bool PatternClause::evaluate(Results* res) {
	//clause is pass at each stage
	res->setClauseFail();
	if(!isBaseValidityCheck()) {
		return false;
	}

	if(!isValid()) {
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
			//Parent(_,2)
		} else if (firstArgType == stringconst::ARG_GENERIC && isSecondFixed) {
			if (evaluateS1GenericS2Fixed(secondArgSyn)) {
				res->setClausePass();
			}
			//Parent(2,_)
		} else if (isFirstFixed && secondArgType == stringconst::ARG_GENERIC) {
			if (evaluateS1FixedS2Generic(firstArgSyn)) {
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
