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

unordered_set<string> FollowsStarClause::getAllS2WithS1Fixed(string s1) {
	unordered_set<string> res = unordered_set<string>();
	int stmtNum = atoi(s1.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> followsStarAfterSet = stmtObj->getFollowsStarAfter();

	for (auto iter = followsStarAfterSet.begin(); iter != followsStarAfterSet.end(); ++iter) {
		res.insert(boost::lexical_cast<string>(*iter));
	}
	return res;
}

unordered_set<string> FollowsStarClause::getAllS2() {
	unordered_set<string> res = unordered_set<string>();
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter = stmtSet.begin(); iter != stmtSet.end(); ++iter) {
		Statement* stmtPtr = *iter;
		unordered_set<int> followsStarAfterSet = stmtPtr->getFollowsStarAfter();
		for (auto j = followsStarAfterSet.begin(); j != followsStarAfterSet.end(); ++j) {
			res.insert(boost::lexical_cast<string>(*j));
		}
	}
	return res;
}

unordered_set<string> FollowsStarClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> res = unordered_set<string>();
	int stmtNum = atoi(s2.c_str());
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> followsStarBeforeSet = stmtObj->getFollowsStarBefore();

	for (auto iter = followsStarBeforeSet.begin(); iter != followsStarBeforeSet.end(); ++iter) {
		res.insert(boost::lexical_cast<string>(*iter));
	}
	return res;
}

unordered_set<string> FollowsStarClause::getAllS1() {
	unordered_set<string> res = unordered_set<string>();
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter = stmtSet.begin(); iter != stmtSet.end(); ++iter) {
		Statement* stmtPtr = *iter;
		unordered_set<int> followsStarBeforeSet = stmtPtr->getFollowsStarBefore();
		for (auto j = followsStarBeforeSet.begin(); j != followsStarBeforeSet.end(); ++j) {
			res.insert(boost::lexical_cast<string>(*j));
		}
	}
	return res;
}

unordered_set<vector<string>> FollowsStarClause::getAllS1AndS2() {
	unordered_set<vector<string>> result = unordered_set<vector<string>>();
	unordered_set<Statement*> stmtSet = stmtTable->getAllStmts();
	for (auto iter1 = stmtSet.begin(); iter1 != stmtSet.end(); ++iter1) {
		Statement* stmtPtr = *iter1;
		unordered_set<int> followsStarAfterSet = stmtPtr->getFollowsStarAfter();
		for (auto iter2 = followsStarAfterSet.begin(); iter2 != followsStarAfterSet.end(); ++iter2) {
			vector<string> values = vector<string>();
			string s1 = boost::lexical_cast<string>(stmtPtr->getStmtNum());
			values.push_back(s1);
			values.push_back(boost::lexical_cast<string>(*iter2));
			result.insert(values);
		}
	}
	return result;
}
