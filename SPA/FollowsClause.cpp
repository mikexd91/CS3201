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

bool checkIsSameType(NodeType type, string stmtType) {
	if((type == STMTLST_ && stmtType == stringconst::ARG_STATEMENT) || 
		(type == WHILE_STMT_ && stmtType == stringconst::ARG_WHILE) ||
		(type == ASSIGN_STMT_ && stmtType == stringconst::ARG_STATEMENT)) {
		return true;
	
	} else {
		return false;
	}
}

// gets immediate statement after stmtNum
string FollowsClause::getFollows(string stmtNum, string unfixedStmtType) {
	StmtTable* table = StmtTable::getInstance();
	int stmt = stoi(stmtNum);
	Statement* stmtObj = table->getStmtObj(stmt);
	
	if (stmtObj != NULL) {
		int stmt2 = stmtObj->getFollowsAfter();
		Statement* stmtObj2 = table->getStmtObj(stmt2);
		NodeType type = stmtObj2->getType();
		bool isSameType = checkIsSameType(type, unfixedStmtType);
		
		if (stmt != -1 && isSameType) {
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
string FollowsClause::getFollowedBy(string stmtNum, string unfixedStmtType) {
	StmtTable* table = StmtTable::getInstance();
	int stmt = stoi(stmtNum);
	Statement* stmtObj = table->getStmtObj(stmt);
	
	if (stmtObj != NULL) {
		int stmt1 = stmtObj->getFollowsBefore();
		Statement* stmtObj1 = table->getStmtObj(stmt1);
		NodeType type = stmtObj1->getType();
		bool isSameType = checkIsSameType(type, unfixedStmtType);

		if (stmt != -1 && isSameType) {
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

void followsBothUnfixedArg(string firstArgType, string secondArgType, Results &resObj) {
	if (firstArgType == stringconst::ARG_ASSIGN) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> assignList = stmtTable->getAssgStmts();
		for (set<Statement*>::iterator iter = assignList.begin() ; iter != assignList.end(); iter++) {
			Statement* stmtObj1 = *iter;
			int stmt1 = stmtObj1->getStmtNum();
			int stmt2 = stmtObj1->getFollowsAfter();

			if (stmt2 != -1) {
				Statement* stmtObj2 = stmtTable->getStmtObj(stmt2);
				NodeType type = stmtObj2->getType();
				bool isSameType = checkIsSameType(type, secondArgType);

				if (isSameType) {
					resObj.setClauseBool(true);
					resObj.setNumOfSyn(2);

					stringstream ss;
					ss << stmt1 << stmt2;
					string strStmt1, strStmt2;
					ss >> strStmt1 >> strStmt2;

					resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else if (firstArgType == stringconst::ARG_WHILE) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> whileList = stmtTable->getWhileStmts();
		for (set<Statement*>::iterator iter = whileList.begin() ; iter != whileList.end(); iter++) {
			Statement* stmtObj1 = *iter;
			int stmt1 = stmtObj1->getStmtNum();
			int stmt2 = stmtObj1->getFollowsAfter();

			if (stmt2 != -1) {
				Statement* stmtObj2 = stmtTable->getStmtObj(stmt2);
				NodeType type = stmtObj2->getType();
				bool isSameType = checkIsSameType(type, secondArgType);

				if (isSameType) {
					resObj.setClauseBool(true);
					resObj.setNumOfSyn(2);

					stringstream ss;
					ss << stmt1 << stmt2;
					string strStmt1, strStmt2;
					ss >> strStmt1 >> strStmt2;

					resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else if (firstArgType == stringconst::ARG_STATEMENT) {
		StmtTable* stmtTable = StmtTable::getInstance();
		boost::unordered_map<int, Statement*>::iterator iter;
		
		for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
			int stmt1 = iter->first;
			Statement* stmtObj1 = iter->second;
			int stmt2 = stmtObj1->getFollowsAfter();
			
			if (stmt2 != -1) {
				Statement* stmtObj2 = stmtTable->getStmtObj(stmt2);
				NodeType type = stmtObj2->getType();
				bool isSameType = checkIsSameType(type, secondArgType);

				if (isSameType) {
					resObj.setClauseBool(true);
					resObj.setNumOfSyn(2);
					
					stringstream ss;
					ss << stmt1 << stmt2;
					string strStmt1, strStmt2;
					ss >> strStmt1 >> strStmt2;

					resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else {
		// throw error
	}
}

Results FollowsClause::evaluate(void) {
	Results* resultsObj = new Results();
	bool isFirstFixed = this->getFirstArgFixed();
	bool isSecondFixed = this->getSecondArgFixed();
	string firstArgSyn = this->getFirstArg();
	string secondArgSyn = this->getSecondArg();
	string firstArgType = this->getFirstArgType();
	string secondArgType = this->getSecondArgType();

	if (isFirstFixed && isSecondFixed) {
		bool isClauseTrue = isFollows(firstArgSyn, secondArgSyn);
		resultsObj->setClauseBool(isClauseTrue);
		resultsObj->setNumOfSyn(0);
		return *resultsObj;

	} else if (isFirstFixed && !isSecondFixed) {
		string stmt2 = getFollows(firstArgSyn, secondArgType);
		if (stmt2 != "-1") {
			resultsObj->setClauseBool(true);
		} 
		resultsObj->setNumOfSyn(1);
		resultsObj->setFirstClauseSyn(secondArgSyn);
		resultsObj->addSingleResult(stmt2);
		return *resultsObj;

	} else if (!isFirstFixed && isSecondFixed) {
		string stmt1 = getFollowedBy(secondArgSyn, firstArgType);
		if (stmt1 != "-1") {
			resultsObj->setClauseBool(true);
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(firstArgSyn);
			resultsObj->addSingleResult(stmt1);
			return *resultsObj;
		}

	} else if (!isFirstFixed && !isSecondFixed) {
		Results resObj = *resultsObj;
		followsBothUnfixedArg(firstArgType, secondArgType, resObj);
		if (resObj.isClauseTrue()) {
			resObj.setFirstClauseSyn(firstArgSyn);
			resObj.setSecondClauseSyn(secondArgSyn);
		}
		return resObj;
		/*
		StmtTable* stmtTable = StmtTable::getInstance();
		boost::unordered_map<int, Statement*>::iterator iter;
		for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
			int stmt1 = iter->first;
			Statement stmtObj1 = *iter->second;
			int stmt2 = stmtObj1.getFollowsAfter();

			if (stmt2 != -1) {
				stringstream ss;
				 ss << stmt1 << stmt2;
				 string strStmt1, strStmt2;
				 ss >> strStmt1 >> strStmt2;
				 //set first and second clause syn
				resultsObj->addPairResult(strStmt1, strStmt2);
			}
		}
		return *resultsObj;
		*/
	} else {
		// Error
		return *resultsObj;
	}
}