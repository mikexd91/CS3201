#include "FollowsClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <string>
#include <iostream>
#include <sstream>

FollowsClause::FollowsClause(void):Clause(FOLLOWS_){
}

FollowsClause::~FollowsClause(void){
}

// gets immediate statement after stmtNum
string FollowsClause::getFollows(string stmtNum) {
	StmtTable* table = StmtTable::getInstance();
	int stmt = stoi(stmtNum);
	Statement* stmtObj = table->getStmtObj(stmt);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getFollowsAfter();
		if (stmt != -1) {
			 stringstream ss;
			 ss << stmt;
			 string str;
			 ss >> str;
			return str;
		}
	} 
	return "-1";
}

// gets immediate statement before stmtNum
string FollowsClause::getFollowedBy(string stmtNum) {
	StmtTable* table = StmtTable::getInstance();
	int stmt = stoi(stmtNum);
	Statement* stmtObj = table->getStmtObj(stmt);
	if (stmtObj != NULL) {
		int stmt = stmtObj->getFollowsBefore();
		if (stmt != -1) {
			 stringstream ss;
			 ss << stmt;
			 string str;
			 ss >> str;
			return str;
		}
	}
	return "-1";
}

bool FollowsClause::isFollows(string stmtNum1, string stmtNum2) {
	string stmt2 = getFollows(stmtNum1);
	if (stmt2 != "-1" && stmt2 == stmtNum2) {
		return true;
	} else {
		return false;
	}
}

bool FollowsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_WHILE);
	return (firstArg && secondArg);
}

Results FollowsClause::evaluate(void) {
	Results* resultsObj = new Results();
	bool isFirstFixed = this->getFirstArgFixed();
	bool isSecondFixed = this->getSecondArgFixed();
	string firstArgSyn = this->getFirstArg();
	string secondArgSyn = this->getSecondArg();

	if (isFirstFixed && isSecondFixed) {
		bool isClauseTrue = isFollows(firstArgSyn, secondArgSyn);
		resultsObj->setClauseBool(isClauseTrue);
		resultsObj->setNumOfSyn(0);
		return *resultsObj;

	} else if (isFirstFixed && !isSecondFixed) {
		string stmt2 = getFollows(firstArgSyn);
		if (stmt2 != "-1") {
			resultsObj->setClauseBool(true);
		} 
		resultsObj->setNumOfSyn(1);
		resultsObj->setFirstClauseSyn(secondArgSyn);
		resultsObj->addSingleResult(stmt2);
		return *resultsObj;

	} else if (!isFirstFixed && isSecondFixed) {
		string stmt1 = getFollowedBy(secondArgSyn);
		if (stmt1 != "-1") {
			resultsObj->setClauseBool(true);
		}
		resultsObj->setNumOfSyn(1);
		resultsObj->setFirstClauseSyn(firstArgSyn);
		resultsObj->addSingleResult(stmt1);
		return *resultsObj;

	} else if (!isFirstFixed && !isSecondFixed) {
		StmtTable* stmtTable = StmtTable::getInstance();
		boost::unordered_map<int, Statement*>::iterator iter;
		for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
			int stmt1 = iter->first;
			Statement stmtObj = *iter->second;
			int stmt2 = stmtObj.getFollowsAfter();

			if (stmt2 != -1) {
				stringstream ss;
				 ss << stmt1 << stmt2;
				 string strStmt1, strStmt2;
				 ss >> strStmt1 >> strStmt2;
				resultsObj->addPairResult(strStmt1, strStmt2);
			}
		}
		return *resultsObj;

	} else {
		// Error
		return *resultsObj;
	}
}