#include "FollowsClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <string>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace stringconst;
using boost::lexical_cast;

FollowsClause::FollowsClause(void):SuchThatClause(FOLLOWS_){
	stmtTable = StmtTable::getInstance();
}

FollowsClause::~FollowsClause(void){
}

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
	if (a != -1) {
		results.insert(lexical_cast<string>(a));
	}
	return results;
}
//Parent(_,s2)

unordered_set<string> FollowsClause::getAllS2(){
	unordered_set<string> res;
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter = stmtSet.begin(); iter != stmtSet.end();  ++iter) {
		Statement* stmtObj = *iter;
		int a = stmtObj->getFollowsAfter();
		if (a != -1) {
			res.insert(lexical_cast<string>(a));
		}
	}
	return res;
}

//Follows(_,s2)
unordered_set<string> FollowsClause::getAllS1WithS2Fixed(string s2){
	int stmtNum = atoi(s2.c_str());
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	int a = stmt->getFollowsBefore();
	unordered_set<string> results;
	if (a != -1) {
		results.insert(lexical_cast<string>(a));
	}
	return results;
}
//Follows(s1,__)

unordered_set<string> FollowsClause::getAllS1(){
	unordered_set<string> res;
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter = stmtSet.begin(); iter != stmtSet.end(); ++iter) {
		Statement* stmtObj = *iter;
		int b = stmtObj->getFollowsBefore();
		if (b != -1) {
			res.insert(lexical_cast<string>(b));
		}
	}
	return res;
}


//Follows(s1,s2)

unordered_set<vector<string>> FollowsClause::getAllS1AndS2(){
	unordered_set<vector<string>> results = unordered_set<vector<string>>();
	//handle case where first and second args are the same -> they should not be
	if (firstArg != secondArg) {
		unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
		for (auto iter = stmtSet.begin(); iter != stmtSet.end(); ++iter) {
			Statement* stmtObj = *iter;
			int a = stmtObj->getStmtNum();
			int b = stmtObj->getFollowsAfter();
			if (a != -1 && b != -1) {
				vector<string> resVec;
				resVec.push_back(lexical_cast<string>(a));
				resVec.push_back(lexical_cast<string>(b));
				results.insert(resVec);
			}
		}
	}
	return results;
}
