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

bool FollowsClause::checkIsSameType(NodeType type, string stmtType) {
	if ((stmtType == stringconst::ARG_STATEMENT) ||
		(type == WHILE_STMT_ && stmtType == stringconst::ARG_WHILE) ||
		(type == ASSIGN_STMT_ && stmtType == stringconst::ARG_ASSIGN)) {
		return true;
	
	} else {
		return false;
	}
}

// gets immediate statement after stmtNum
string FollowsClause::getFollows(string stmtNum, string unfixedStmtType) {
	StmtTable* table = StmtTable::getInstance();
	int stmt = stoi(stmtNum);
	Statement* stmtObj1 = table->getStmtObj(stmt);
	
	if (stmtObj1 != NULL) {
		int stmt2 = stmtObj1->getFollowsAfter();
		if (stmt2 != -1) {
			Statement* stmtObj2 = table->getStmtObj(stmt2);
			NodeType type = stmtObj2->getType();
			bool isSameType = checkIsSameType(type, unfixedStmtType);
		
			if (isSameType) {
				 stringstream ss;
				 ss << stmt2;
				 string str;
				 ss >> str;

				return str;
			}
		}
	} 
	return "-1";
}

// gets immediate statement before stmtNum
string FollowsClause::getFollowedBy(string stmtNum, string unfixedStmtType) {
	StmtTable* table = StmtTable::getInstance();
	int stmt2 = stoi(stmtNum);
	Statement* stmtObj2 = table->getStmtObj(stmt2);
	
	if (stmtObj2 != NULL) {
		int stmt1 = stmtObj2->getFollowsBefore();
		if (stmt1 != -1) {
			Statement* stmtObj1 = table->getStmtObj(stmt1);
			NodeType type = stmtObj1->getType();
			bool isSameType = checkIsSameType(type, unfixedStmtType);

			if (isSameType) {
				 stringstream ss;
				 ss << stmt1;
				 string str;
				 ss >> str;

				return str;
			}
		}
	}
	return "-1";
}

bool FollowsClause::isFollows(string stmtNum1, string stmtNum2) {
	StmtTable* table = StmtTable::getInstance();
	int stmt1 = stoi(stmtNum1);
	Statement* stmtObj1 = table->getStmtObj(stmt1);
	int stmt2 = stmtObj1->getFollowsAfter();

	stringstream ss;
	ss << stmt2;
	string stmt2Str;
	ss >> stmt2Str;

	if (stmt2Str != "-1" && stmt2Str == stmtNum2) {
		return true;
	} else {
		return false;
	}
}

bool FollowsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_PROGLINE);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_PROGLINE);
	return (firstArg && secondArg);
}

void FollowsClause::followsBothUnfixedArg(string firstArgType, string secondArgType, Results &resObj) {
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
					stringstream ss;
					ss << stmt1 << ' ' << stmt2;
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
					stringstream ss;
					ss << stmt1 << ' ' << stmt2;
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
					stringstream ss;
					ss << stmt1 << ' ' << stmt2;
					string strStmt1, strStmt2;
					ss >> strStmt1 >> strStmt2;

					resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else {
		// throw error
	}

	if (resObj.getPairResults().size() > 0) {
		resObj.setClausePassed(true);
		//resObj.setNumOfSyn(2);
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
		resultsObj->setClausePassed(isClauseTrue);
		resultsObj->setNumOfSyn(0);
		return *resultsObj;

	} else if (isFirstFixed && !isSecondFixed) {
		string stmt2 = getFollows(firstArgSyn, secondArgType);
		if (stmt2 != "-1") {
			resultsObj->setClausePassed(true);
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(secondArgSyn);
			resultsObj->addSingleResult(stmt2);
		} 
		return *resultsObj;

	} else if (!isFirstFixed && isSecondFixed) {
		string stmt1 = getFollowedBy(secondArgSyn, firstArgType);
		if (stmt1 != "-1") {
			resultsObj->setClausePassed(true);
			resultsObj->setNumOfSyn(1);
			resultsObj->setFirstClauseSyn(firstArgSyn);
			resultsObj->addSingleResult(stmt1);
		}
		return *resultsObj;

	} else if (!isFirstFixed && !isSecondFixed) {
		Results resObj = *resultsObj;
		
		if (firstArgSyn != secondArgSyn) {
			followsBothUnfixedArg(firstArgType, secondArgType, resObj);
			if (resObj.isClausePassed()) {
				resObj.setNumOfSyn(2);
				resObj.setFirstClauseSyn(firstArgSyn);
				resObj.setSecondClauseSyn(secondArgSyn);
			}
		} 
		return resObj;

	} else {
		// Error
		return *resultsObj;
	}
}