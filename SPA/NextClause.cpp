/*
 * NextClause.cpp
 *
 *  Created on: 3 Oct, 2015
 *      Author: Leon
 */

#include "NextClause.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace stringconst;
using namespace boost;

NextClause::NextClause() :
		SuchThatClause(NEXT_) {
	stmtTable = StmtTable::getInstance();
}

NextClause::~NextClause(void) {

}

bool NextClause::isValid(void) {
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

// Next(3, 4)
bool NextClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	return isNext(firstArg, secondArg);
}

// Next(_, _)
bool NextClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> allStmts = stmtTable->getAllStmts();

	BOOST_FOREACH(auto i, allStmts) {
		if(!i->getNext().empty()) {
			return true;
		}
	}

	return false;
}

// Next(_, 1)
bool NextClause::evaluateS1GenericS2Fixed(string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	if(!stmt->getPrev().empty()) {
		return true;
	}

	return false;
}

// Next(1, _)
bool NextClause::evaluateS1FixedS2Generic(string s1) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	if(!stmt->getNext().empty()) {
		return true;
	}

	return false;
}

// Next(1, s) || Next(1, if) || Next(1, w) || Next(1, a) || Next(1, call)
unordered_set<string> NextClause::getAllS2WithS1Fixed(string s1) {
	// TODO
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	unordered_set<int> nextSet = stmt->getNext();

	BOOST_FOREACH(auto i, nextSet) {
		string nextStmt = lexical_cast<string>(i);

		if(isNeededArgType(secondArgType, i)) {
			results.insert(nextStmt);
		}
	}

	return results;
}

// Next(_, s) || Next(_, if) || Next(_, w) || Next(_, c) || Next(_, a)
unordered_set<string> NextClause::getAllS2() {
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
		if(!i->getPrev().empty()) {
			results.insert(lexical_cast<string>(i->getStmtNum()));
		}
	}

	return results;
}

// Next(s, 2) || Next(if, 2) || Next(w, 2) || Next(a, 2) || Next(c, 2)
unordered_set<string> NextClause::getAllS1WithS2Fixed(string s2) {
	// TODO
	unordered_set<string> results;
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));
	unordered_set<int> setToEvaluate = stmt->getPrev();

	BOOST_FOREACH(auto i, setToEvaluate) {
		string prevStmt = lexical_cast<string>(i);

		if(isNeededArgType(firstArgType, i)) {
			results.insert(prevStmt);
		}
	}

	return results;
}

// Next(s, _) || Next(if, _) || Next(w, _) || Next(a, _) || Next(c, _)
unordered_set<string> NextClause::getAllS1() {
	// TODO
	unordered_set<string> result;
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
		if(!i->getNext().empty()) {
			result.insert(lexical_cast<string>(i->getStmtNum()));
		}
	}

	return result;
}

// Next(s1, s2) || Next(s, if) || Next(s, w) || Next(s, a) || Next(s, c)
// Next(if1, if2) || Next(if, s) || Next(if, w) || Next(if, c) || Next(if, a)
// Next(w1, w2) || Next(w, s) || Next(w, c) || Next(w, a) || Next(w, if)
// Next(c1, c2) || Next(c, w) || Next(c, s) || Next(c, if) || Next(c, a)
// Next(a1, a2) || Next(a, s) || Next(a, c) || Next(a, if) || Next(a, w)
unordered_set<vector<string>> NextClause::getAllS1AndS2() {
	// TODO
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

bool NextClause::isNext(string first, string second) {
	Statement* firstStmt = stmtTable->getStmtObj(atoi(first.c_str()));
	unordered_set<int> firstStmtNext = firstStmt->getNext();

	if(firstStmtNext.find(atoi(second.c_str())) != firstStmtNext.end()) {
		return true;
	}

	return false;
}

// Next(s1, s2) || Next(s, if) || Next(s, w) || Next(s, a) || Next(s, c)
unordered_set<vector<string>> NextClause::evalFirstArgStmt() {
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

	// i represents second syn
	BOOST_FOREACH(auto i, setToBeEvaluated) {
		unordered_set<int> prevStmts = i->getPrev();
		
		// j represents first syn
		BOOST_FOREACH(auto j, prevStmts) {
			vector<string> pair;
			pair.push_back(lexical_cast<string>(j));
			pair.push_back(lexical_cast<string>(i->getStmtNum()));
			results.insert(pair);
		}
	}

	return results;
}

// Next(if1, if2) || Next(if, s) || Next(if, w) || Next(if, c) || Next(if, a)
// Next(w1, w2) || Next(w, s) || Next(w, c) || Next(w, a) || Next(w, if)
// Next(c1, c2) || Next(c, w) || Next(c, s) || Next(c, if) || Next(c, a)
// Next(a1, a2) || Next(a, s) || Next(a, c) || Next(a, if) || Next(a, w)
unordered_set<vector<string>> NextClause::evalFirstArg() {
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

	// i represents first syn
	BOOST_FOREACH(auto i, setToBeEvaluated) {
		unordered_set<int> nextStmt = i->getNext();

		// j represents second syn
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

bool NextClause::isNeededArgType(string type, int stmtNum) {
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