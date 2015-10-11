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
		|| (firstType == stringconst::ARG_CALL)
		|| (firstType == stringconst::ARG_IF)
		|| (firstType == stringconst::ARG_ASSIGN) 
		|| (firstType == stringconst::ARG_WHILE) 
		|| (firstType == stringconst::ARG_PROGLINE) 
		|| (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) 
		|| (secondType == stringconst::ARG_CALL)
		|| (secondType == stringconst::ARG_IF)
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

// Follows(1, s) || Follows(1, if) || Follows(1, w) || Follows(1, a) || Follows(1, call)
unordered_set<string> FollowsClause::getAllS2WithS1Fixed(string s1){
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	int afterStmt = stmt->getFollowsAfter();
	if (afterStmt != -1 && isNeededArgType(secondArgType, afterStmt)) {
		results.insert(lexical_cast<string>(afterStmt));
	}
	return results;
}

// Follows(_, s) || Follows(_, if) || Follows(_, w) || Follows(_, c) || Follows(_, a)
unordered_set<string> FollowsClause::getAllS2(){
	unordered_set<string> results;
	unordered_set<Statement*> setToEvaluate;

	if(secondArgType == ARG_STATEMENT || secondArgType == ARG_PROGLINE) {
		setToEvaluate = stmtTable->getAllStmts();
	} else if(secondArgType == ARG_IF) {
		setToEvaluate = stmtTable->getIfStmts();
	} else if(secondArgType == ARG_WHILE) {
		setToEvaluate = stmtTable->getWhileStmts();
	} else if(secondArgType == ARG_CALL) {
		setToEvaluate = stmtTable->getCallStmts();
	} else {
		setToEvaluate = stmtTable->getAssgStmts();
	}

	for (auto iter = setToEvaluate.begin(); iter != setToEvaluate.end(); ++iter) {
		int beforeStmt = (*iter)->getFollowsBefore();
		bool hasBeforeStmt = (beforeStmt != -1);
		if (hasBeforeStmt) {
			results.insert(lexical_cast<string>((*iter)->getStmtNum()));
		}
	}

	return results;
}

// Follows(s, 2) || Follows(if, 2) || Follows(w, 2) || Follows(a, 2) || Follows(c, 2)
unordered_set<string> FollowsClause::getAllS1WithS2Fixed(string s2){
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));
	int beforeStmt = stmt->getFollowsBefore();
	if (beforeStmt != -1 && isNeededArgType(firstArgType, beforeStmt)) {
		results.insert(lexical_cast<string>(beforeStmt));
	}
	return results;
}

// Follows(s, _) || Follows(if, _) || Follows(w, _) || Follows(a, _) || Follows(c, _)
unordered_set<string> FollowsClause::getAllS1(){
	unordered_set<string> results;
	unordered_set<Statement*> setToEvaluate;

	if(firstArgType == ARG_STATEMENT || firstArgType == ARG_PROGLINE) {
		setToEvaluate = stmtTable->getAllStmts();
	} else if(firstArgType == ARG_IF) {
		setToEvaluate = stmtTable->getIfStmts();
	} else if(firstArgType == ARG_WHILE) {
		setToEvaluate = stmtTable->getWhileStmts();
	} else if(firstArgType == ARG_CALL) {
		setToEvaluate = stmtTable->getCallStmts();
	} else {
		setToEvaluate = stmtTable->getAssgStmts();
	}

	for (auto iter = setToEvaluate.begin(); iter!= setToEvaluate.end(); ++iter) {
		int afterStmt = (*iter)->getFollowsAfter();
		bool hasAfterStmt = (afterStmt != -1);
		if (hasAfterStmt) {
			results.insert(lexical_cast<string>((*iter)->getStmtNum()));
		}
	}

	return results;
}

// Follows(s1, s2) || Follows(s, if) || Follows(s, w) || Follows(s, a) || Follows(s, c)
// Follows(if1, if2) || Follows(if, s) || Follows(if, w) || Follows(if, c) || Follows(if, a)
// Follows(w1, w2) || Follows(w, s) || Follows(w, c) || Follows(w, a) || Follows(w, if)
// Follows(c1, c2) || Follows(c, w) || Follows(c, s) || Follows(c, if) || Follows(c, a)
// Follows(a1, a2) || Follows(a, s) || Follows(a, c) || Follows(a, if) || Follows(a, w)
unordered_set<vector<string>> FollowsClause::getAllS1AndS2(){
	unordered_set<vector<string>> results;
	if(firstArg == secondArg) {
		return results;
	}

	if(firstArgType == ARG_STATEMENT || firstArgType == ARG_PROGLINE) {
		results = evalFirstArgStmt();
	} else {
		results = evalFirstArg();
	}

	return results;
}

unordered_set<vector<string>> FollowsClause::evalFirstArgStmt() {
	unordered_set<vector<string>> results;
	unordered_set<Statement*> setToBeEvaluated;

	if(secondArgType == ARG_STATEMENT || secondArgType == ARG_PROGLINE) {
		setToBeEvaluated = stmtTable->getAllStmts();
	} else if(secondArgType == ARG_IF) {
		setToBeEvaluated = stmtTable->getIfStmts();
	} else if(secondArgType == ARG_WHILE) {
		setToBeEvaluated = stmtTable->getWhileStmts();
	} else if(secondArgType == ARG_CALL) {
		setToBeEvaluated = stmtTable->getCallStmts();
	} else {
		setToBeEvaluated = stmtTable->getAssgStmts();
	}

	for (auto afterStmt = setToBeEvaluated.begin(); afterStmt != setToBeEvaluated.end(); ++afterStmt) {
		int thisStmt = (*afterStmt)->getStmtNum();
		int beforeStmt = (*afterStmt)->getFollowsBefore();
		bool hasBeforeStmt = (beforeStmt != -1);
		if (hasBeforeStmt) {
			vector<string> pair;
			pair.push_back(lexical_cast<string>(beforeStmt));
			pair.push_back(lexical_cast<string>(thisStmt));
			results.insert(pair);
		}
	}
	return results;
}

unordered_set<vector<string>> FollowsClause::evalFirstArg() {
	unordered_set<vector<string>> results;
	unordered_set<Statement*> setToBeEvaluated;
	
	if(firstArgType == ARG_IF) {
		setToBeEvaluated = stmtTable->getIfStmts();
	} else if(firstArgType == ARG_CALL) {
		setToBeEvaluated = stmtTable->getCallStmts();
	} else if(firstArgType == ARG_WHILE) {
		setToBeEvaluated = stmtTable->getWhileStmts();
	} else {
		setToBeEvaluated = stmtTable->getAssgStmts();
	}

	for (auto afterStmt = setToBeEvaluated.begin(); afterStmt != setToBeEvaluated.end(); ++afterStmt) {
		int thisStmt = (*afterStmt)->getStmtNum();
		int beforeStmt = (*afterStmt)->getFollowsBefore();
		bool hasBeforeStmt = (beforeStmt != -1);
		if (hasBeforeStmt) {
			vector<string> pair;
			if(isNeededArgType(secondArgType, thisStmt)) {
				pair.push_back(lexical_cast<string>(beforeStmt));
				pair.push_back(lexical_cast<string>(thisStmt));
			}
			results.insert(pair);
		}
	}
	return results;
}

bool FollowsClause::isNeededArgType(string type, int stmtNum) {
	Statement* stmt = stmtTable->getStmtObj(stmtNum);

	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return true;
	}
	
	if(stmt->getType() == ASSIGN_STMT_ && type == ARG_ASSIGN) {
		return true;
	}

	if(stmt->getType() == WHILE_STMT_ && type == ARG_WHILE) {
		return true;
	}

	if(stmt->getType() == CALL_STMT_ && type == ARG_CALL) {
		return true;
	}

	if(stmt->getType() == IF_STMT_ && type == ARG_IF) {
		return true;
	}

	return false;
}
