/*
 * NextClause.cpp
 *
 *  Created on: 3 Oct, 2015
 *      Author: Leon
 */

#include "NextClause.h"
#include "boost/foreach.hpp"

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
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN);
	bool secondArg = (firstArgType == ARG_GENERIC)
			|| (firstArgType == ARG_WHILE) || (firstArgType == ARG_PROGLINE)
			|| (firstArgType == ARG_IF) || (firstArgType == ARG_STATEMENT)
			|| (firstArgType == ARG_ASSIGN);

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

	}
	return false;
}

bool NextClause::isNext(string firstArg, string secondArg) {
	Statement* firstStmt = stmtTable->getStmtObj(atoi(firstArg.c_str()));
	unordered_set<int> firstStmtNext = firstStmt->getNext();

	if(firstStmtNext.find(atoi(secondArg.c_str())) != firstStmtNext.end()) {
		return true;
	}

	return false;
}
