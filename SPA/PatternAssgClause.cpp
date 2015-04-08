#include "PatternAssgClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <set>
#include <iostream>

using namespace std;
using namespace stringconst;

PatternAssgClause::PatternAssgClause(string syn) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
}

PatternAssgClause::~PatternAssgClause(void) {
}

Results PatternAssgClause::evaluate() {
	// get all the assg stmt nums
	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allAssg = stable->getAssgStmts();

	vector<int> assgNums = vector<int>();

	set<Statement*>::iterator assgIter;
	for (assgIter = allAssg.begin(); assgIter != allAssg.end(); assgIter++) {
		Statement* assg = *assgIter;
		cout << assg->getStmtNum() << endl;
		assgNums.push_back(assg->getStmtNum());
	}

	// var:wildcard
		// expr:wildcard => return all a
		// expr:notwild => return all a that match expr

	// var:fixed
		// expr:wildcard => return a using var
		// expr:notwild	=> return a using var that match expr

	// var:syn
		// expr:wildcard => return for each
		// expr:notwild

	bool exprWildcard = getExpression() == stringconst::STRING_EMPTY;
	bool varWildcard = getVar() == stringconst::STRING_EMPTY;
	bool varFixed = getVarFixed();

	if (varWildcard) {
		if (exprWildcard) {
			return evaluateVarWildExprWild(assgNums);
		} else {
			return evaulateVarWildExpr(assgNums, getExpression());
		}
	} else if (varFixed) {
		if (exprWildcard) {
			return evaluateVarFixedExprWild(assgNums);
		} else {
			return evaluateVarFixedExpr(assgNums, getExpression());
		}
	} else {
		if (exprWildcard) {
			return evaluateVarExprWild(assgNums);
		} else {
			return evaluateVarExpr(assgNums, getExpression());
		}
	}
}

Results PatternAssgClause::evaluateVarWildExprWild(vector<int> assgNums) {
	return *new Results();
}

Results PatternAssgClause::evaulateVarWildExpr(vector<int> assgNums, string expr) {
	return *new Results();
}

Results PatternAssgClause::evaluateVarFixedExprWild(vector<int> assgNums) {
	return *new Results();
}

Results PatternAssgClause::evaluateVarFixedExpr(vector<int> assgNums, string expr) {
	return *new Results();
}

Results PatternAssgClause::evaluateVarExprWild(vector<int> assgNums) {
	return *new Results();
}

Results PatternAssgClause::evaluateVarExpr(vector<int> assgNums, string expr) {
	return *new Results();
}

bool PatternAssgClause::isValid() {
	return true;
}
