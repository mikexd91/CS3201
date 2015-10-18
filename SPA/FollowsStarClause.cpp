#include "FollowsStarClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace stringconst;
using namespace boost;
using namespace std;

FollowsStarClause::FollowsStarClause(void):SuchThatClause(FOLLOWSSTAR_){
	stmtTable = StmtTable::getInstance();
}

FollowsStarClause::~FollowsStarClause(void){
}

bool FollowsStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) 
		|| (firstType == stringconst::ARG_IF)
		|| (firstType == stringconst::ARG_ASSIGN) 
		|| (firstType == stringconst::ARG_WHILE)
		|| (firstType == stringconst::ARG_CALL)
		|| (firstType == stringconst::ARG_PROGLINE) 
		|| (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) 
		|| (secondType == stringconst::ARG_IF)
		|| (secondType == stringconst::ARG_ASSIGN) 
		|| (secondType == stringconst::ARG_WHILE) 
		|| (secondType == stringconst::ARG_CALL)
		|| (secondType == stringconst::ARG_PROGLINE) 
		|| (secondType == stringconst::ARG_GENERIC);
	return (firstArg && secondArg);
}

bool FollowsStarClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	int stmtNum1 = atoi(s1.c_str());
	int stmtNum2 = atoi(s2.c_str());
	Statement* stmtObj1 = stmtTable->getStmtObj(stmtNum1);
	unordered_set<int> followsStarSet = stmtObj1->getFollowsStarAfter();
	bool isExist = followsStarSet.find(stmtNum2) != followsStarSet.end();
	return isExist;
};

// e.g. Follows*(_,_)
bool FollowsStarClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter = stmtSet.begin(); iter != stmtSet.end(); ++iter) {
		Statement* stmtPtr = *iter;
		unordered_set<int> followsStarAfterSet = stmtPtr->getFollowsStarAfter();
		unordered_set<int> followsStarBeforeSet = stmtPtr->getFollowsStarBefore();
		if (followsStarAfterSet.size() > 0 || followsStarBeforeSet.size() > 0) {
			return true;
		}
	}
	return false;
};

// e.g. Follows*(_,s2)
bool FollowsStarClause::evaluateS1GenericS2Fixed(string s2){
	int stmtNum = atoi(s2.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> followsStarBeforeSet = stmtObj->getFollowsStarBefore();
	if (followsStarBeforeSet.size() > 0) {
		return true;
	} else {
		return false;
	}
};

// e.g. Follows*(s1,_)
bool FollowsStarClause::evaluateS1FixedS2Generic(string s1) {
	int stmtNum = atoi(s1.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> followsStarAfterSet = stmtObj->getFollowsStarAfter();
	if (followsStarAfterSet.size() > 0) {
		return true;
	} else {
		return false;
	}
}

// Follows*(1, s) || Follows*(1, if) || Follows*(1, w) || Follows*(1, a) || Follows*(1, call)
unordered_set<string> FollowsStarClause::getAllS2WithS1Fixed(string s1) {
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	unordered_set<int> afterSet = stmt->getFollowsStarAfter();
	
	for (auto iter = afterSet.begin(); iter != afterSet.end(); ++iter) {
		string thisStmt = lexical_cast<string>(*iter);
		if(isNeededArgType(secondArgType, *iter)) {
			results.insert(thisStmt);
		}
	}
	return results;
}

// Follows*(_, s) || Follows*(_, if) || Follows*(_, w) || Follows*(_, c) || Follows*(_, a)
unordered_set<string> FollowsStarClause::getAllS2() {
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
		unordered_set<int> beforeSet = (*iter)->getFollowsStarBefore();
		if (!beforeSet.empty()) {
			results.insert(lexical_cast<string>((*iter)->getStmtNum()));
		}
	}
	return results;
}

// Follows*(s, 2) || Follows*(if, 2) || Follows*(w, 2) || Follows*(a, 2) || Follows*(c, 2)
unordered_set<string> FollowsStarClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> res;
	int stmtNum = atoi(s2.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> setToEvaluate = stmtObj->getFollowsStarBefore();

	for (auto iter = setToEvaluate.begin(); iter != setToEvaluate.end(); ++iter) {
		int beforeStmt = *iter;
		if(beforeStmt != -1 && isNeededArgType(firstArgType, beforeStmt)) {
			res.insert(lexical_cast<string>(beforeStmt));
		}
	}
	return res;
}

// Follows*(s, _) || Follows*(if, _) || Follows*(w, _) || Follows*(a, _) || Follows*(c, _)
unordered_set<string> FollowsStarClause::getAllS1() {
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

	for (auto iter = setToEvaluate.begin(); iter != setToEvaluate.end(); ++iter) {
		unordered_set<int> afterSet = (*iter)->getFollowsStarAfter();
		if (!afterSet.empty()) {
			results.insert(lexical_cast<string>((*iter)->getStmtNum()));
		}
	}
	return results;
	
}

// Follows*(s1, s2) || Follows*(s, if) || Follows*(s, w) || Follows*(s, a) || Follows(s, c)
// Follows*(if1, if2) || Follows*(if, s) || Follows*(if, w) || Follows*(if, c) || Follows(if, a)
// Follows*(w1, w2) || Follows*(w, s) || Follows*(w, c) || Follows*(w, a) || Follows(w, if)
// Follows*(c1, c2) || Follows*(c, w) || Follows*(c, s) || Follows*(c, if) || Follows(c, a)
// Follows*(a1, a2) || Follows*(a, s) || Follows*(a, c) || Follows*(a, if) || Follows(a, w)
unordered_set<vector<string>> FollowsStarClause::getAllS1AndS2() {
	unordered_set<vector<string>> result;
	if (firstArg == secondArg) {
		return result;
	}

	unordered_set<Statement*> firstArgTypeSet = getSetFromArgType(firstArgType);
	for (auto iter = firstArgTypeSet.begin(); iter != firstArgTypeSet.end(); ++iter) {
		unordered_set<int> followsStarAfterSet = (*iter)->getFollowsStarAfter();
		if (!followsStarAfterSet.empty()) {
			for (auto iter2 = followsStarAfterSet.begin(); iter2 != followsStarAfterSet.end(); ++iter2) {
				if (isNeededArgType(secondArgType, *iter2)) {
					vector<string> pair;
					pair.push_back(lexical_cast<string>((*iter)->getStmtNum()));
					pair.push_back(lexical_cast<string>(*iter2));
					result.insert(pair);
				}
			}
		}
	}

	return result;
}

unordered_set<Statement*> FollowsStarClause::getSetFromArgType(string type) {
	unordered_set<Statement*> argTypeSet;
	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		argTypeSet = stmtTable->getAllStmts();
	} else if(type == ARG_IF) {
		argTypeSet = stmtTable->getIfStmts();
	} else if(type == ARG_WHILE) {
		argTypeSet = stmtTable->getWhileStmts();
	} else if(type == ARG_CALL) {
		argTypeSet = stmtTable->getCallStmts();
	} else {
		argTypeSet = stmtTable->getAssgStmts();
	}
	return argTypeSet;
}

bool FollowsStarClause::isNeededArgType(string type, int stmtNum) {
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