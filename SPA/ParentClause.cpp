#include "ParentClause.h"
#include "Statement.h"
#include <boost/lexical_cast.hpp>
#include <assert.h>
#include <iostream>

ParentClause::ParentClause(void):Clause(PARENT_){
	stmtTable = StmtTable::getInstance();
}

ParentClause::~ParentClause(void){
}

//to add if statements
bool ParentClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC) || (firstType == stringconst::ARG_ASSIGN);
	bool secondArg = (secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool ParentClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	return isParent(firstArg, secondArg);
}

//e.g. Parent(_,_)
bool ParentClause::evaluateS1GenericS2Generic() {
	//get all while statements
	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	//check if while stmt has children
	for (unordered_set<Statement*>::iterator iter = whileStmts.begin(); iter != whileStmts.end(); ++iter) {
		Statement* whileStmt = *iter;
		if (!whileStmt->getChildren().empty()) {
			return true;
		}
	}
	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	for (unordered_set<Statement*>::iterator iter = ifStmts.begin(); iter != ifStmts.end(); ++iter) {
		Statement* ifStmt = *iter;
		if (!ifStmt->getChildren().empty()) {
			return true;
		}
	}
	return false;
}

//e.g. Parent(_,2)
bool ParentClause::evaluateS1GenericS2Fixed(string s2) {
	int parentNum = getParent(stoi(s2), stringconst::ARG_STATEMENT);
	return parentNum != -1;
}

//e.g. Parent(2,_)
bool ParentClause::evaluateS1FixedS2Generic(string s1){
	Statement::ChildrenSet  children =  getChildren(stoi(s1), stringconst::ARG_STATEMENT);
	return !children.empty();
}

//e.g. Parent(2, s2)
unordered_set<string> ParentClause::getAllS2WithS1Fixed(string s1) {
	string argType = this->secondArgType;
	Statement::ChildrenSet  children = getChildren(stoi(s1), argType);
	unordered_set<string> stmtNumSet;
	for (Statement::ChildrenSet ::iterator iter = children.begin(); iter != children.end(); iter++) {
		stmtNumSet.insert(boost::lexical_cast<string>(*iter));
	}
	return stmtNumSet;
}

//e.g. Parent(_, s2)
//get all children
unordered_set<string> ParentClause::getAllS2() {
	unordered_set<string> stmtNumSet;
	//get all while statements
	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	//check if while stmt has children
	for (unordered_set<Statement*>::iterator iter = whileStmts.begin(); iter != whileStmts.end(); ++iter) {
		Statement* whileStmt = *iter;
		Statement::ChildrenSet children = getChildren(whileStmt->getStmtNum(), this->secondArgType);
		for (Statement::ChildrenSet ::iterator iter = children.begin(); iter != children.end(); iter++) {
			stmtNumSet.insert(boost::lexical_cast<string>(*iter));
		}
	}
	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	for (unordered_set<Statement*>::iterator iter = ifStmts.begin(); iter != ifStmts.end(); ++iter) {
		Statement* ifStmt = *iter;
		Statement::ChildrenSet  children = getChildren(ifStmt->getStmtNum(), this->secondArgType);
		for (Statement::ChildrenSet ::iterator iter = children.begin(); iter != children.end(); iter++) {
			stmtNumSet.insert(boost::lexical_cast<string>(*iter));
		}
	}
	return stmtNumSet;
}

//e.g. Parent(s1,2)
//get parent of string
unordered_set<string> ParentClause::getAllS1WithS2Fixed(string s2) {
	int parentNum = getParent(stoi(s2), firstArgType);
	unordered_set<string> stmtNumSet;
	if (parentNum != -1) {
		stmtNumSet.insert(boost::lexical_cast<string>(parentNum));
	}
	return stmtNumSet;
}

//e.g. Parent(s1,_)
//get all parents
unordered_set<string> ParentClause::getAllS1() {
	unordered_set<string> stmtNumSet;
	//get all while statements
	if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_WHILE) {
		unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
		//check if while stmt has children
		for (unordered_set<Statement*>::iterator iter = whileStmts.begin(); iter != whileStmts.end(); ++iter) {
			Statement* whileStmt = *iter;
			Statement::ChildrenSet children = getChildren(whileStmt->getStmtNum(), this->secondArgType);
			if(!children.empty()) {
				stmtNumSet.insert(boost::lexical_cast<string>(whileStmt->getStmtNum()));
			}
		}
	}
	if (firstArgType == stringconst::ARG_STATEMENT) {// || firstArgType == stringconst::ARG_IF) {
		unordered_set<Statement*>ifStmts = stmtTable->getIfStmts();
		for (unordered_set<Statement*>::iterator iter = ifStmts.begin(); iter != ifStmts.end(); ++iter) {
			Statement* ifStmt = *iter;
			Statement::ChildrenSet children = getChildren(ifStmt->getStmtNum(), this->secondArgType);
			if(!children.empty()) {
				stmtNumSet.insert(boost::lexical_cast<string>(ifStmt->getStmtNum()));
			}
		}
	}
	return stmtNumSet;
}

//Parent(s1, s2)
Results::ResultsTable* ParentClause::getAllS1AndS2() {
	Results::ResultsTable* results = new Results::ResultsTable();
	//get all while statements

	if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_WHILE) {
		unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
		//check if while stmt has children
			int count = 0;
		for (unordered_set<Statement*>::iterator iter = whileStmts.begin(); iter != whileStmts.end(); ++iter) {
			Statement* whileStmt = *iter;
			Statement::ChildrenSet children = getChildren(whileStmt->getStmtNum(), this->secondArgType);
			for (Statement::ChildrenSet::iterator iter = children.begin(); iter != children.end(); iter++) {
				Results::Row* pair = new Results::Row();
				int child = *iter;
				int whileStmtNum = whileStmt->getStmtNum();
				(*pair)[firstArg] = boost::lexical_cast<string>(whileStmt->getStmtNum());
				(*pair)[secondArg] = boost::lexical_cast<string>(child);
				results->insert(pair);
			}
		}
	}
	if (firstArgType == stringconst::ARG_STATEMENT) {// || firstArgType == stringconst::ARG_IF) {
		unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
		for (unordered_set<Statement*>::iterator iter = ifStmts.begin(); iter != ifStmts.end(); ++iter) {
			unordered_map<string, string>* pair = new unordered_map<string, string>();
			Statement* ifStmt = *iter;
			Statement::ChildrenSet children = getChildren(ifStmt->getStmtNum(), this->secondArgType);
			for (Statement::ChildrenSet::iterator iter = children.begin(); iter != children.end(); iter++) {
				int child = *iter;
				(*pair)[firstArg] = boost::lexical_cast<string>(ifStmt->getStmtNum());
				(*pair)[secondArg] = boost::lexical_cast<string>(child);
				results->insert(pair);
			}
		}
	}
	return results;
}

bool ParentClause::isParent(string stmt1, string stmt2) {
	int stmtNum1 = atoi(stmt1.c_str());
	int stmtNum2 = atoi(stmt2.c_str());
	int stmt;
	stmt = getParent(stmtNum2, stringconst::ARG_STATEMENT);
	return stmt != -1 && stmt == stmtNum1;
}

Statement::ChildrenSet ParentClause::getChildren(int stmtNum, string stmtArgType) {
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == nullptr) {
		return unordered_set<int>();
	} else {
		Statement::ChildrenSet stmtSet = stmtObj->getChildren();
		if (stmtArgType == stringconst::ARG_ASSIGN) {
			return Utils::filterStatements(stmtSet, ASSIGN_STMT_);
		} else if (stmtArgType == stringconst::ARG_WHILE ) {
			return Utils::filterStatements(stmtSet, WHILE_STMT_);
		} else {
			//assert(stmtArgType == stringconst::ARG_STATEMENT);
			return stmtSet;
		}
	}
}

int ParentClause::getParent(int stmtNum, string argType) {
	NodeType nodeType = Utils::convertArgTypeToNodeType(argType);
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == nullptr || (nodeType != NULL_ && stmtObj->getType() != nodeType)) {
		return -1;
	} else {
		int stmt = stmtObj->getParent();
		return stmt;
	}
}



/**

Results* ParentClause::addParentPairToResult(set<Statement*> containerStmts, string firstArgType, string secondArgType) {
	Results* resultsObj = new Results();
	//TODO: Set number of syn beforehand
	for (set<Statement*>::iterator containerIter = containerStmts.begin(); containerIter != containerStmts.end(); containerIter++) {
		Statement* containerStmt = *containerIter;
		string containerStmtNo = boost::lexical_cast<string>(containerStmt->getStmtNum());
		set<int> children;
		if (secondArgType == stringconst::ARG_GENERIC || secondArgType == stringconst::ARG_STATEMENT) {
			children = containerStmt->getChildren();
		} else {
			children = Utils::filterStatements(containerStmt->getChildren(), Utils::convertArgTypeToNodeType(secondArgType));
		}
		if (!children.empty()) {
			resultsObj->setClausePassed(true);
			if (firstArgType == stringconst::ARG_GENERIC && secondArgType == stringconst::ARG_GENERIC) {
				resultsObj->setNumOfSyn(0);
				return resultsObj;
			} else if (firstArgType == stringconst::ARG_GENERIC) {
				resultsObj->setNumOfSyn(1);
				for (set<int>::iterator childrenIter = children.begin(); childrenIter != children.end(); childrenIter++) {
					string childStmtNo = boost::lexical_cast<string>(*childrenIter);
					resultsObj->addSingleResult(childStmtNo);
				}
			} else if (secondArgType == stringconst::ARG_GENERIC) {
				resultsObj->setNumOfSyn(1);
				resultsObj->addSingleResult(containerStmtNo);
			} else {
				resultsObj->setNumOfSyn(2);
				for (set<int>::iterator childrenIter = children.begin(); childrenIter != children.end(); childrenIter++) {
					string childStmtNo = boost::lexical_cast<string>(*childrenIter);
					resultsObj->addPairResult(containerStmtNo, childStmtNo);
				}
			}
		}
	}
	return resultsObj;
}
**/

/**
Results* ParentClause::evaluateS1FixedS2Wild(string firstArgSyn, string secondArgSyn, string secondArgType) {
	Results* resultsObj = new Results();
	set<int> children = getChildren(firstArgSyn, secondArgType);
	if (!children.empty()) {
		resultsObj->setClausePassed(true);
		if (secondArgType == stringconst::ARG_GENERIC) {
			resultsObj->setNumOfSyn(0);
		} else {
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(secondArgSyn);
			for (set<int>::iterator it = children.begin(); it != children.end(); ++it) {
				resultsObj->addSingleResult(boost::lexical_cast<string>(*it));
			}
		}
	}
	return resultsObj;
}

Results* ParentClause::evaluateS1WildS2Fixed(string firstArgSyn, string firstArgType, string secondArgSyn) {
	Results* resultsObj = new Results();
	int stmt1 = getParent(atoi(secondArgSyn.c_str()));
	if (stmt1 != -1) {
		resultsObj->setClausePassed(true);
		if (firstArgType == stringconst::ARG_GENERIC) {
			resultsObj->setNumOfSyn(0);
		} else {
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(firstArgSyn);
			resultsObj->addSingleResult(boost::lexical_cast<string>(stmt1));
		}
	}
	return resultsObj;
}

Results* ParentClause::evaluateS1WildS2Wild(string firstArgSyn, string secondArgSyn, string firstArgType, string secondArgType) {
	Results* resultsObj = new Results();

	if(firstArgSyn == secondArgSyn && firstArgType != stringconst::ARG_GENERIC) {
		return resultsObj;
	}

	if (firstArgType != stringconst::ARG_ASSIGN) {
		//first arg type can only be while
		//get all while statements
		set<Statement*> whileStmts = stmtTable->getWhileStmts();
		resultsObj = addParentPairToResult(whileStmts, firstArgType, secondArgType);
	}
	if (resultsObj->isClausePassed()) {
		if (firstArgType == stringconst::ARG_GENERIC && secondArgType == stringconst::ARG_GENERIC) {
			resultsObj->setNumOfSyn(0);
		} else if (firstArgType == stringconst::ARG_GENERIC) {
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(secondArgSyn);
		} else if (secondArgType == stringconst::ARG_GENERIC) {
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(firstArgSyn);
		} else {
			resultsObj->setNumOfSyn(2);
			resultsObj->setFirstClauseSyn(firstArgSyn);
			resultsObj->setSecondClauseSyn(secondArgSyn);
		}
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
		resultsObj = evaluateS1WildS2Fixed(firstArgSyn, firstArgType, secondArgSyn);
	} else if (!isFirstFixed && !isSecondFixed) {
		resultsObj = evaluateS1WildS2Wild(firstArgSyn, secondArgSyn, firstArgType, secondArgType);
	}
	return *resultsObj;
}

**/