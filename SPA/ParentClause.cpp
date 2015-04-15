#include "ParentClause.h"
#include "Utils.h"
#include "Statement.h"
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <iostream>

ParentClause::ParentClause(void):Clause(PARENT_){
	stmtTable = StmtTable::getInstance();
}

ParentClause::~ParentClause(void){
}

bool ParentClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_STATEMENT);
	bool secondArg = (secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN);
	return firstArg && secondArg;
}

bool ParentClause::isParent(string stmt1, string stmt2) {
	int stmtNum1 = atoi(stmt1.c_str());
	int stmtNum2 = atoi(stmt2.c_str());
	int stmt;
	stmt = getParent(stmtNum2);
	return stmt != -1 && stmt == stmtNum1;
}

set<int> ParentClause::getChildren(string stmt, string stmtArgType) {
	int stmtNum = atoi(stmt.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == NULL) {
		return set<int>();
	} else {
		set<int> stmtSet = stmtObj->getChildren();
		if (stmtArgType == stringconst::ARG_ASSIGN) {
			return Utils::filterStatements(stmtSet, ASSIGN_STMT_);
		} else if (stmtArgType == stringconst::ARG_WHILE ) {
			return Utils::filterStatements(stmtSet, WHILE_STMT_);
		} else {
			assert(stmtArgType == stringconst::ARG_STATEMENT);
			return stmtSet;
		}
	}
}

int ParentClause::getParent(int stmtNum) {
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == NULL) {
		return -1;
	} else {
		int stmt = stmtObj->getParent();
		return stmt;
	}
}

Results* ParentClause::addParentPairToResult(set<Statement*> containerStmts, NodeType type) {
	Results* resultsObj = new Results();
	for (set<Statement*>::iterator containerIter = containerStmts.begin(); containerIter != containerStmts.end(); containerIter++) {
		Statement* containerStmt = *containerIter;
		string containerStmtNo = boost::lexical_cast<string>(containerStmt->getStmtNum());
		set<int> children;
		if (type == NULL_) {
			children = containerStmt->getChildren();
		} else {
			children = Utils::filterStatements(containerStmt->getChildren(), type);
		}
		if (!children.empty()) {
			resultsObj->setClausePassed(true);
			for (set<int>::iterator childrenIter = children.begin(); childrenIter != children.end(); childrenIter++) {
				string childStmtNo = boost::lexical_cast<string>(*childrenIter);
				resultsObj->addPairResult(containerStmtNo, childStmtNo);
			}
		}
	}
	return resultsObj;
}

Results* ParentClause::evaluateS1FixedS2Fixed(string firstArgSyn, string secondArgSyn) {
	Results* resultsObj = new Results();
	bool isClauseTrue = this->isParent(firstArgSyn, secondArgSyn);
	resultsObj->setClausePassed(isClauseTrue);
	resultsObj->setNumOfSyn(0);
	return resultsObj;
}

Results* ParentClause::evaluateS1FixedS2Wild(string firstArgSyn, string secondArgSyn, string secondArgType) {
	Results* resultsObj = new Results();
	set<int> children = getChildren(firstArgSyn, secondArgType);
	if (!children.empty()) {
		resultsObj->setClausePassed(true);
		resultsObj->setNumOfSyn(1);
		resultsObj->setFirstClauseSyn(secondArgSyn);
		for (set<int>::iterator it = children.begin(); it != children.end(); ++it) {
			resultsObj->addSingleResult(boost::lexical_cast<string>(*it));
		}
	}
	return resultsObj;
}

Results* ParentClause::evaluateS1WildS2Fixed(string firstArgSyn, string secondArgSyn) {
	Results* resultsObj = new Results();
	int stmt1 = getParent(atoi(secondArgSyn.c_str()));
	if (stmt1 != -1) {
		resultsObj->setClausePassed(true);
		resultsObj->setNumOfSyn(1);
		resultsObj->setFirstClauseSyn(firstArgSyn);
		resultsObj->addSingleResult(boost::lexical_cast<string>(stmt1));
	}
	return resultsObj;
}

Results* ParentClause::evaluateS1WildS2Wild(string firstArgSyn, string secondArgSyn) {
	Results* resultsObj = new Results();

	if(firstArgSyn == secondArgSyn) {
		return resultsObj;
	}

	if (firstArgType != stringconst::ARG_ASSIGN) {
		//first arg type can only be while
		//get all while statements
		set<Statement*> whileStmts = stmtTable->getWhileStmts();
		if (secondArgType == stringconst::ARG_ASSIGN) {
			//for each while statement, get the children, add
			resultsObj = addParentPairToResult(whileStmts, ASSIGN_STMT_);
		} else if (secondArgType == stringconst::ARG_WHILE) {
			resultsObj = addParentPairToResult(whileStmts, WHILE_STMT_);
		} else {
			assert(secondArgType == stringconst::ARG_STATEMENT);
			resultsObj = addParentPairToResult(whileStmts, NULL_);
		}
	}
	if (resultsObj->isClausePassed()) {
		resultsObj->setNumOfSyn(2);
		resultsObj->setFirstClauseSyn(firstArgSyn);
		resultsObj->setSecondClauseSyn(secondArgSyn);
	}
	return resultsObj;
}

Results ParentClause::evaluate(void) {
	Results* resultsObj = new Results();
	bool isFirstFixed = this->getFirstArgFixed();
	bool isSecondFixed = this->getSecondArgFixed();
	string firstArgSyn = this->getFirstArg();
	string secondArgSyn = this->getSecondArg();
	string firstArgType = this->getFirstArgType();
	string secondArgType = this->getSecondArgType();

	if (isFirstFixed && isSecondFixed) {
		resultsObj = evaluateS1FixedS2Fixed(firstArgSyn, secondArgSyn);
	} else if (isFirstFixed && !isSecondFixed) {
		resultsObj = evaluateS1FixedS2Wild(firstArgSyn, secondArgSyn, secondArgType);
	} else if (!isFirstFixed && isSecondFixed) {
		resultsObj = evaluateS1WildS2Fixed(firstArgSyn, secondArgSyn);
	} else if (!isFirstFixed && !isSecondFixed) {
		resultsObj = evaluateS1WildS2Wild(firstArgSyn, secondArgSyn);
	}
	return *resultsObj;
}

