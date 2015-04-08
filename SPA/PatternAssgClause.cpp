#include "PatternAssgClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <set>

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
		assgNums.push_back(assg->getStmtNum());
	}

	// check if fixed var or not
	bool fixedVar = this->getVarFixed();
	string var = getSecondArg();
	if (fixedVar) {
		string expr = getExpression();
		if (expr == stringconst::STRING_EMPTY) {
			return *new Results();
			//return evaluateFixedVarNoExpr(assgNums);
		}
	} else if (true) {
	}
	return *new Results();
}

Results PatternAssgClause::evaluateFixedVarNoExpr(vector<int>& assgNums) {
	//Results* results = new Results();
	//string var = getSecondArg();
	//for (int i = 0; i < assgNums.size(); i++) {
	//	if (match(assgNums.at(i))) {
	//		//results->addPairResult
	//	}
	//}
	return *new Results();
}

bool PatternAssgClause::matchVar(int stmtNum) {
	// find the stmt that match this var
	// then search its subtree for the expr
	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> assgStmts = stable->getAssgStmts();

	//set<Statement*>::iterator iter;
	//for (iter = assgStmts.begin(); iter != assgStmts.end(); iter++) {
	//	Statement* assg = *iter;
	//	if (assg->getStmtNum() == stmtNum) {
	//		set<string> uses = assg->getModifies();
	//		if (uses.find(var) != uses.end()) {
	//			return stmtNum((AssgNode*)(assg->getTNodeRef()), expr);
	//			//return false;
	//		} else {
	//			return false;
	//		}
	//	}
	//}

	return false;
}