/*
 * NextStarClause.cpp
 *
 *  Created on: 14 Oct, 2015
 *      Author: Leon
 */

#include "NextStarClause.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace stringconst;
using namespace boost;

NextStarClause::NextStarClause()
	: SuchThatClause(NEXTSTAR_) {
	stmtTable = StmtTable::getInstance();
	match = false;
	cfg = CFG::getInstance();
}

NextStarClause::~NextStarClause() {

}

bool NextStarClause::isValid(void) {
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

// Next*(4, 2)
bool NextStarClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	match = false;

	unordered_set<int> next = stmt->getNext();
	BOOST_FOREACH(auto s, next) {
		Statement* child = stmtTable->getStmtObj(s);
		vector<string> visited;
		dfsFind(child, s2, visited);
	}

	return match;
}

// Next*(_, _)
bool NextStarClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> stmts = stmtTable->getAllStmts();

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNext().empty()) {
			return true;
		}
	}

	return false;
}

// Next*(_, 2)
bool NextStarClause::evaluateS1GenericS2Fixed(string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	if(!stmt->getPrev().empty()) {
		return true;
	}

	return false;
}

// Next*(2, _)
bool NextStarClause::evaluateS1FixedS2Generic(string s1) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	if(!stmt->getNext().empty()) {
		return true;
	}

	return false;
}

// Next*(3, s) or Next*(3, a) or Next*(3, c) or Next*(3, if) or Next*(3, w)
unordered_set<string> NextStarClause::getAllS2WithS1Fixed(string s1) {
	results.clear();
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	unordered_set<int> stmtNext = stmt->getNext();

	BOOST_FOREACH(auto s, stmtNext) {
		Statement* child = stmtTable->getStmtObj(s);
		vector<string> visited;
		dfsFindNext(child, visited, secondArgType);
	}

	return results;
}

// Next*(_, s) or Next*(_, a) or Next*(_, c) or Next*(_, if) or Next*(_, w) 
unordered_set<string> NextStarClause::getAllS2() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(secondArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getPrev().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

// Next*(s, 3) or Next*(a, 3) or Next*(c, 3) or Next*(if, 3) or Next*(w, 3)
unordered_set<string> NextStarClause::getAllS1WithS2Fixed(string s2) {
	results.clear();
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	unordered_set<int> stmtPrev = stmt->getPrev();

	BOOST_FOREACH(auto s, stmtPrev) {
		Statement* child = stmtTable->getStmtObj(s);
		vector<string> visited;
		dfsFindPrev(child, visited, firstArgType);
	}

	return results;
}

// Next*(s, _) or Next*(a, _) or Next*(c, _) or Next*(if, _) or Next*(
unordered_set<string> NextStarClause::getAllS1() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(firstArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNext().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

unordered_set<vector<string>> NextStarClause::getAllS1AndS2() {
	resultsPair.clear();
	bool sameArg = firstArg == secondArg;

	vector<ProcGNode*> cfgArr = cfg->getAllProcedures();
	BOOST_FOREACH(auto node, cfgArr) {
		vector<string> visited;
		GNode* firstNode = node->getChild();
		int firstStmtNum = firstNode->getStartStmt();
		Statement* stmt = stmtTable->getStmtObj(firstStmtNum);
		dfsFindPair(stmt, visited, firstArgType, secondArgType, sameArg);
	}

	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	BOOST_FOREACH(auto w, whileStmts) {
		unordered_set<int> wNext = w->getNext();
		BOOST_FOREACH(auto wChild, wNext) {
			vector<string> visited;
			Statement* wChildStmt = stmtTable->getStmtObj(wChild);
			dfsFindPair(wChildStmt, visited, firstArgType, secondArgType, sameArg);
		}
	}

	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	BOOST_FOREACH(auto ifS, ifStmts) {
		unordered_set<int> ifNext = ifS->getNext();
		BOOST_FOREACH(auto ifChild, ifNext) {
			vector<string> visited;
			Statement* ifChildStmt = stmtTable->getStmtObj(ifChild);
			dfsFindPair(ifChildStmt, visited, firstArgType, secondArgType, sameArg);
		}
	}

	return resultsPair;
}

void NextStarClause::dfsFind(Statement* stmt, string str, vector<string> visited) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());

	if(match) {
		return;
	}

	if(currStmt == str) {
		match = true;
		return;
	}

	if(!contains(visited, currStmt)) {
		visited.push_back(currStmt);
		unordered_set<int> children = stmt->getNext();
		BOOST_FOREACH(auto child, children) {
			Statement* childStmt = stmtTable->getStmtObj(child);
			dfsFind(childStmt, str, visited);
		}
	}
}

void NextStarClause::dfsFindPrev(Statement* stmt, vector<string> visited, string type) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());

	if(!contains(visited, currStmt)) {
		if(isNeededArgType(type, stmt->getStmtNum())) {
			results.insert(currStmt);
		}

		visited.push_back(currStmt);
		unordered_set<int> children = stmt->getPrev();
		BOOST_FOREACH(auto child, children) {
			Statement* childStmt = stmtTable->getStmtObj(child);
			dfsFindPrev(childStmt, visited, type);
		}
	}
}

void NextStarClause::dfsFindNext(Statement* stmt, vector<string> visited, string type) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());

	if(!contains(visited, currStmt)) {
		if(isNeededArgType(type, stmt->getStmtNum())) {
			results.insert(currStmt);
		}

		visited.push_back(currStmt);
		unordered_set<int> children = stmt->getNext();
		BOOST_FOREACH(auto child, children) {
			Statement* childStmt = stmtTable->getStmtObj(child);
			dfsFindNext(childStmt, visited, type);
		}
	}
}

void NextStarClause::dfsFindPair(Statement* stmt, vector<string> visited, string type1, string type2, bool sameArg) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());

	if(contains(visited, currStmt)) {
		int pos = getPosition(visited, currStmt);
		for(auto i = pos; i < visited.size(); i++) {
			string first = visited.at(i);
			string second = currStmt;

			if(sameArg) {
				if(!(first == second)) {
					continue;
				}
			}

			if(isNeededArgType(type1, atoi(first.c_str())) && isNeededArgType(type2, atoi(second.c_str()))) {
				vector<string> pair;
				pair.push_back(first);
				pair.push_back(second);
				resultsPair.insert(pair);
			}
		}
	} else {
		for(auto i = 0; i < visited.size(); i++) {
			string first = visited.at(i);
			string second = currStmt;

			if(sameArg) {
				if(!(first == second)) {
					continue;
				}
			}

			if(isNeededArgType(type1, atoi(first.c_str())) && isNeededArgType(type2, atoi(second.c_str()))) {
				vector<string> pair;
				pair.push_back(first);
				pair.push_back(second);
				resultsPair.insert(pair);
			}
		}

		visited.push_back(currStmt);
		unordered_set<int> children = stmt->getNext();
		BOOST_FOREACH(auto child, children) {
			Statement* childStmt = stmtTable->getStmtObj(child);
			dfsFindPair(childStmt, visited, type1, type2, sameArg);
		}
	}
}

bool NextStarClause::contains(vector<string> arr, string item) {
	if(find(arr.begin(), arr.end(), item) != arr.end()) {
		return true;
	}

	return false;
}

int NextStarClause::getPosition(vector<string> arr, string item) {
	int pos = find(arr.begin(), arr.end(), item) - arr.begin();

	return pos;
}

unordered_set<Statement*> NextStarClause::getNeededStmts(string type) {
	unordered_set<Statement*> stmts;
	
	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return stmtTable->getAllStmts();
	}

	if(type == ARG_IF) {
		return stmtTable->getIfStmts();
	}

	if(type == ARG_WHILE) {
		return stmtTable->getWhileStmts();
	}

	if(type == ARG_CALL) {
		return stmtTable->getCallStmts();
	}

	if(type == ARG_ASSIGN) {
		return stmtTable->getAssgStmts();
	}

	return stmts;
}

bool NextStarClause::isNeededArgType(string type, int stmtNum) {
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