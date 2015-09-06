#include "FollowsClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

using namespace stringconst;
using boost::unordered_set;
using boost::lexical_cast;

StmtTable* stmtTable = StmtTable::getInstance();

FollowsClause::FollowsClause(void):Clause(FOLLOWS_){
}

FollowsClause::~FollowsClause(void){
}
/*
bool FollowsClause::checkIsSameType(NodeType type, string stmtType) {
	if ((stmtType == stringconst::ARG_STATEMENT) ||
		(type == WHILE_STMT_ && stmtType == stringconst::ARG_WHILE) ||
		(type == ASSIGN_STMT_ && stmtType == stringconst::ARG_ASSIGN) ||
		(stmtType == stringconst::ARG_GENERIC)) {
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
*/
bool FollowsClause::isFollows(string stmtNum1, string stmtNum2) {
	StmtTable* table = StmtTable::getInstance();
	int stmt1 = stoi(stmtNum1);
	Statement* stmtObj1 = table->getStmtObj(stmt1);

	if (stmtObj1 != NULL) {
		int stmt2 = stmtObj1->getFollowsAfter();
		if (stmt2 != -1) {

			stringstream ss;
			ss << stmt2;
			string stmt2Str;
			ss >> stmt2Str;

			if (stmt2Str == stmtNum2) {
				return true;
			} else {
				return false;
			}
		}
	}
	return false;
}

bool FollowsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) 
		|| (firstType == stringconst::ARG_ASSIGN) 
		|| (firstType == stringconst::ARG_WHILE) 
		|| (firstType == stringconst::ARG_PROGLINE) 
		|| (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) 
		|| (secondType == stringconst::ARG_ASSIGN) 
		|| (secondType == stringconst::ARG_WHILE) 
		|| (secondType == stringconst::ARG_PROGLINE) 
		|| (secondType == stringconst::ARG_GENERIC);
	return (firstArg && secondArg);
}

/*
void FollowsClause::followsBothUnfixedArg(string firstArgType, string secondArgType, Results &resObj) {
	if (firstArgType == stringconst::ARG_ASSIGN) {
		StmtTable* stmtTable = StmtTable::getInstance();
		unordered_set<Statement*> assignList = stmtTable->getAssgStmts();
		
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

					//resObj.addPairResult(strStmt1, strStmt2);
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

					//resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else if (firstArgType == stringconst::ARG_STATEMENT ||
		firstArgType == stringconst::ARG_GENERIC) {
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

					//resObj.addPairResult(strStmt1, strStmt2);
				}
			}
		}

	} else {
		// throw error
	}

	//if (resObj.getPairResults().size() > 0) {
		//resObj.setClausePassed(true);
		//resObj.setNumOfSyn(2);
	//}
}
*/
/*
void FollowsClause::followsWithOneUnderscore(string firstArgType, string secondArgType, Results &resObj) {
	StmtTable* stmtTable = StmtTable::getInstance();
	boost::unordered_map<int, Statement*>::iterator iter;
		
	for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
		int stmt1 = iter->first;
		Statement* stmtObj1 = iter->second;
			
		int stmt2;
		if (firstArgType == stringconst::ARG_GENERIC) {
			stmt2 = stmtObj1->getFollowsAfter();
		} else {
			stmt2 = stmtObj1->getFollowsBefore();
		}

		if (stmt2 != -1) {
			Statement* stmtObj2 = stmtTable->getStmtObj(stmt2);
			NodeType type = stmtObj2->getType();
				
			bool isSameType;
			if (firstArgType == stringconst::ARG_GENERIC) {
				isSameType = checkIsSameType(type, secondArgType);
			} else {
				isSameType = checkIsSameType(type, firstArgType);
			}

			if (isSameType) {
				stringstream ss;
				ss << stmt2;
				string strStmt2;
				ss >> strStmt2;

				//resObj.addSingleResult(strStmt2);
			}
		}
	}	
}
*/
/*
Results FollowsClause::evaluate(void) {
	Results* resultsObj = new Results();
	bool isFirstFixed = this->getFirstArgFixed();
	bool isSecondFixed = this->getSecondArgFixed();
	string firstArgSyn = this->getFirstArg();
	string secondArgSyn = this->getSecondArg();
	string firstArgType = this->getFirstArgType();
	string secondArgType = this->getSecondArgType();
	*/
	/*
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
		
		if (firstArgType == stringconst::ARG_GENERIC && secondArgType == stringconst::ARG_GENERIC) {
			followsBothUnfixedArg(firstArgType, secondArgType, resObj);
			if (resObj.isClausePassed()) {
				resObj.setNumOfSyn(0);
			}
		}

		if (firstArgType == stringconst::ARG_GENERIC) {
			followsWithOneUnderscore(firstArgType, secondArgType, resObj);
			if (resObj.isClausePassed()) {
				resObj.setNumOfSyn(1);
				resObj.setFirstClauseSyn(secondArgSyn);
			}
		}

		if (secondArgType == stringconst::ARG_GENERIC) {
			followsWithOneUnderscore(firstArgType, secondArgType, resObj);
			if (resObj.isClausePassed()) {
				resObj.setNumOfSyn(1);
				resObj.setFirstClauseSyn(firstArgSyn);
			}
		}

		if (firstArgSyn != secondArgSyn) {
			followsBothUnfixedArg(firstArgType, secondArgType, resObj);
			if (resObj.isClausePassed()) {
				resObj.setNumOfSyn(2);
				resObj.setFirstClauseSyn(firstArgSyn);
				resObj.setSecondClauseSyn(secondArgSyn);
			}
		}
		return resObj;

	} else {*/
		// Error
		//return *resultsObj;
	//}
	
//}

//Follows(1,2)
bool FollowsClause::evaluateS1FixedS2Fixed(string s1, string s2){
	bool isClauseTrue = isFollows(s1, s2);
	return isClauseTrue;
}

//e.g. Follows(_,_)
bool FollowsClause::evaluateS1GenericS2Generic(){
	unordered_set<Statement*> stmts = stmtTable->getAllStmts();
	for (unordered_set<Statement*>::iterator iter = stmts.begin(); iter != stmts.end(); iter++){
		Statement* current = *iter;
		int a = current->getFollowsAfter();
		int b = current->getFollowsBefore();
		if (a != -1 || b != -1){
			return true;
		}
	}
	return false;
}
//e.g. Follows(_,2) bool
bool FollowsClause::evaluateS1GenericS2Fixed(string s2){
	Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s2));
	int a = stmt->getFollowsBefore();
	return (a != -1);
}
//Follows(1,_) bool
bool FollowsClause::evaluateS1FixedS2Generic(string s1){
	Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s1));
	int a = stmt->getFollowsAfter();
	return (a != -1);
}
//Follows(s1,_)
unordered_set<string> FollowsClause::getAllS2WithS1Fixed(string s1){
	Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s1));
	int a = stmt->getFollowsAfter();
	unordered_set<string> results;
	results.insert(lexical_cast<string>(a));
	return results;
}
//Parent(_,s2)
//is this necessary for follows???

unordered_set<string> FollowsClause::getAllS2(){
	unordered_set<string> asd;
	return asd;
}

//Follows(_,s2)
unordered_set<string> FollowsClause::getAllS1WithS2Fixed(string s2){
	Statement* stmt = stmtTable->getStmtObj(lexical_cast<int>(s2));
	int a = stmt->getFollowsBefore();
	unordered_set<string> results;
	results.insert(lexical_cast<string>(a));
	return results;
}
//Follows(s1,__)
//is this necessary for follows???

unordered_set<string> FollowsClause::getAllS1(){
	unordered_set<string> asd;
	return asd;
}


//Follows(s1,s2)
//is this necessary for follows???

Results::ResultsTable* FollowsClause::getAllS1AndS2(){
	Results::ResultsTable* asd = new Results::ResultsTable();
	return asd;
}
