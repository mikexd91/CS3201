/*
 * NextBipClause.cpp
 *
 *  Created on: 2 Nov, 2015
 *      Author: Leon
 */

#include "NextBipClause.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace stringconst;
using namespace boost;

NextBipClause::NextBipClause()
	: SuchThatClause(NEXTBIP_) {
	stmtTable = StmtTable::getInstance();
}

NextBipClause::~NextBipClause(void) {

}

bool NextBipClause::isValid(void) {
	bool firstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE)
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF)
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);
	bool secondArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE) 
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF) 
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);

	return firstArg && secondArg;
}

// NextBip(3, 4)
bool NextBipClause::evaluateS1FixedS2Fixed(string first, string second) {
	Statement* firstStmt = stmtTable->getStmtObj(atoi(first.c_str()));
	unordered_set<int> firstStmtNext = firstStmt->getNextBip();

	if(firstStmtNext.find(atoi(second.c_str())) != firstStmtNext.end()) {
		return true;
	}

	return false;
}

// NextBip(_, _)
bool NextBipClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> allStmts = stmtTable->getAllStmts();

	BOOST_FOREACH(auto i, allStmts) {
		if(!i->getNextBip().empty()) {
			return true;
		}
	}

	return false;
}

// NextBip(_, 1)
bool NextBipClause::evaluateS1GenericS2Fixed(string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	if(!stmt->getPrevBip().empty()) {
		return true;
	}

	return false;
}

// NextBip(1, _)
bool NextBipClause::evaluateS1FixedS2Generic(string s1) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	if(!stmt->getNextBip().empty()) {
		return true;
	}

	return false;
}

// NextBip(1, s) || NextBip(1, if) || NextBip(1, w) || NextBip(1, a) || NextBip(1, call)
unordered_set<string> NextBipClause::getAllS2WithS1Fixed(string s1) {
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	unordered_set<int> nextSet = stmt->getNextBip();

	BOOST_FOREACH(auto i, nextSet) {
		string nextStmt = lexical_cast<string>(i);

		if(isNeededArgType(secondArgType, i)) {
			results.insert(nextStmt);
		}
	}

	return results;
}

// NextBip(_, s) || NextBip(_, if) || NextBip(_, w) || NextBip(_, c) || NextBip(_, a)
unordered_set<string> NextBipClause::getAllS2() {
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

	BOOST_FOREACH(auto i, setToEvaluate) {
		if(!i->getPrevBip().empty()) {
			results.insert(lexical_cast<string>(i->getStmtNum()));
		}
	}

	return results;
}

// NextBip(s, 2) || NextBip(if, 2) || NextBip(w, 2) || NextBip(a, 2) || NextBip(c, 2)
unordered_set<string> NextBipClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));
	unordered_set<int> setToEvaluate = stmt->getPrevBip();

	BOOST_FOREACH(auto i, setToEvaluate) {
		string prevStmt = lexical_cast<string>(i);

		if(isNeededArgType(firstArgType, i)) {
			results.insert(prevStmt);
		}
	}

	return results;
}

// NextBip(s, _) || NextBip(if, _) || NextBip(w, _) || NextBip(a, _) || NextBip(c, _)
unordered_set<string> NextBipClause::getAllS1() {
	unordered_set<string> results;
	unordered_set<Statement*> setToBeEvaluated;

	if(firstArgType == ARG_STATEMENT || firstArgType == ARG_PROGLINE) {
		setToBeEvaluated = stmtTable->getAllStmts();
	} else if(firstArgType == ARG_IF) {
		setToBeEvaluated = stmtTable->getIfStmts();
	} else if(firstArgType == ARG_WHILE) {
		setToBeEvaluated = stmtTable->getWhileStmts();
	} else if(firstArgType == ARG_CALL) {
		setToBeEvaluated = stmtTable->getCallStmts();
	} else {
		setToBeEvaluated = stmtTable->getAssgStmts();
	}

	BOOST_FOREACH(auto i, setToBeEvaluated) {
		if(!i->getNextBip().empty()) {
			results.insert(lexical_cast<string>(i->getStmtNum()));
		}
	}

	return results;
}

// NextBip(s1, s2) || NextBip(s, if) || NextBip(s, w) || NextBip(s, a) || NextBip(s, c)
// NextBip(if1, if2) || NextBip(if, s) || NextBip(if, w) || NextBip(if, c) || NextBip(if, a)
// NextBip(w1, w2) || NextBip(w, s) || NextBip(w, c) || NextBip(w, a) || NextBip(w, if)
// NextBip(c1, c2) || NextBip(c, w) || NextBip(c, s) || NextBip(c, if) || NextBip(c, a)
// NextBip(a1, a2) || NextBip(a, s) || NextBip(a, c) || NextBip(a, if) || NextBip(a, w)
unordered_set<vector<string>> NextBipClause::getAllS1AndS2() {
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

// NextBip(s1, s2) || NextBip(s, if) || NextBip(s, w) || NextBip(s, a) || NextBip(s, c)
unordered_set<vector<string>> NextBipClause::evalFirstArgStmt() {
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

	BOOST_FOREACH(auto i, setToBeEvaluated) {
		unordered_set<int> prevStmts = i->getPrevBip();

		BOOST_FOREACH(auto j, prevStmts) {
			vector<string> pair;
			pair.push_back(lexical_cast<string>(j));
			pair.push_back(lexical_cast<string>(i->getStmtNum()));
			results.insert(pair);
		}
	}

	return results;
}

// NextBip(if1, if2) || NextBip(if, s) || NextBip(if, w) || NextBip(if, c) || NextBip(if, a)
// NextBip(w1, w2) || NextBip(w, s) || NextBip(w, c) || NextBip(w, a) || NextBip(w, if)
// NextBip(c1, c2) || NextBip(c, w) || NextBip(c, s) || NextBip(c, if) || NextBip(c, a)
// NextBip(a1, a2) || NextBip(a, s) || NextBip(a, c) || NextBip(a, if) || NextBip(a, w)
unordered_set<vector<string>> NextBipClause::evalFirstArg() {
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

	BOOST_FOREACH(auto i, setToBeEvaluated) {
		unordered_set<int> nextStmt = i->getNextBip();

		BOOST_FOREACH(auto j, nextStmt) {
			vector<string> pair;
			
			if(isNeededArgType(secondArgType, j)) {
				pair.push_back(lexical_cast<string>(i->getStmtNum()));
				pair.push_back(lexical_cast<string>(j));
				results.insert(pair);
			}
		}
	}

	return results;
}

bool NextBipClause::isNeededArgType(string type, int stmtNum) {
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