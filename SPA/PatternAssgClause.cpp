#include "PatternAssgClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "OpNode.h"

#include <set>
#include <stack>
#include <iostream>

using namespace std;
using namespace stringconst;

PatternAssgClause::PatternAssgClause(const string& syn) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
	//cout << "setting " << syn << endl;
}

PatternAssgClause::~PatternAssgClause(void) {
}

bool PatternAssgClause::isValid() {
	// TODO drew pls
	return true;
}

Results PatternAssgClause::evaluate() {
	// get all the assg stmt nums
	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allAssg = stable->getAssgStmts();

	vector<int> assgNums = vector<int>();

	set<Statement*>::iterator assgIter;
	for (assgIter = allAssg.begin(); assgIter != allAssg.end(); assgIter++) {
		Statement* assg = *assgIter;
		//cout << assg->getStmtNum() << endl;
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

	cout << getExpression() << endl;
	cout << getVar() << endl;
	cout << getVarFixed() << endl;

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
	// return all a
	Results* res = new Results();
	res->setFirstClauseSyn(getSynonym());
	
	// simply insert all assgs
	for (int i = 0; i < assgNums.size(); i++) {
		long long stmtNum = assgNums.at(i);
		string stmtNumStr = to_string(stmtNum);
		res->addSingleResult(stmtNumStr);
	}

	return *res;
}

Results PatternAssgClause::evaulateVarWildExpr(vector<int> assgNums, string expr) {
	// return all a that match expr
	Results* res = new Results();
	res->setFirstClauseSyn(getSynonym());

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	// if match expr then insert
	for (int i = 0; i < assgNums.size(); i++) {
		long long stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchExpr(assgNode, getExpression())) {
			string stmtNumStr = to_string(stmtNum);
			res->addSingleResult(stmtNumStr);
		}
	}

	return *res;
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

bool PatternAssgClause::matchExpr(AssgNode* assg, string expr) {

	// match expr based on the assgnode
	if (assg == NULL) {
		return false;
	}
	int i = 0;

	// stuff the rpn into the stack
	string rpn = expr.substr(2, expr.length() - 4);
	//std::stack<string> tempstack = stack<string>();
	//std::stack<string> rpnstack = stack<string>();
	std::vector<string> rpnarr = vector<string>();
	for (int i = 0; i < rpn.length(); i++) {
		string token = rpn.substr(i, 1);
		if (token != " ") {
			//tempstack.push(token);
			rpnarr.push_back(token);
		}
	}

	// do dfs
	set<TNode*> visited;
	stack<TNode*> nodestack = stack<TNode*>();
	TNode* exprnode = assg->getExprNode();
	//cout << exprnode->getName() << endl;
	nodestack.push(exprnode);
	TNode* nextnode = nodestack.top();
	while (nextnode != NULL && i < 9999) {
		i++;
		if (nextnode->getNodeType() == OPERATOR_) {
			OpNode* op = (OpNode*) nextnode;
			nodestack.push(op->getRightNode());
			nodestack.push(op->getLeftNode());
			nextnode = nodestack.top();
			//cout << nextnode->getName() << endl;
		} else if (nextnode->getNodeType() == CONSTANT_) {
			nextnode = NULL;
		} else if (nextnode->getNodeType() == VARIABLE_) {
			nextnode = NULL;
		} else {
			return false;
		}
	}

	// compare rpn with dfs nodes
	// once start to match rpn must match all the way
	// then only considered subtree
	for (int compared = 0; compared < rpnarr.size() && i < 9999; compared++) {
		if (nodestack.empty()) {
			return false;
		}
		TNode* node = nodestack.top();
		string token = rpnarr.at(compared);
		//cout << "conparing" << endl;
		//cout << node->getName() << endl;
		//cout << token << endl;
		if (node->getName() == token) {
			nodestack.pop();
			// if we break a streak of matches,
			// then it's not subtree
		} else {
			// start from the front of the rpn again
			// -1 cos it will ++ on the iteration
			compared = -1;
			nodestack.pop();
		}
	}

	return true;
}
