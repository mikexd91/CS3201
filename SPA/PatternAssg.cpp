#include "PatternAssg.h"
#include <iostream>
#include <stack>
#include "OpNode.h"
#include "boost\unordered_set.hpp"
//#include "boost/lockfree/stack.hpp"

//using namespace boost;
//using namespace boost::lockfree;
using boost::unordered_set;
using namespace std;

PatternAssg::PatternAssg(int stmtNum) {
	this->_stmtNum = stmtNum;
}

PatternAssg::~PatternAssg() {

}

bool PatternAssg::matchNothing() {
	return true;
}

bool PatternAssg::matchVar(string var) {
	// look for assg stmts with var on the left hand side
	// basically look for modifies var.
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> assgStmts = stable->getAssgStmts();

	unordered_set<Statement*>::iterator iter;
	for (iter = assgStmts.begin(); iter != assgStmts.end(); iter++) {
		Statement* assg = *iter;
		if (assg->getStmtNum() == _stmtNum) {
			set<string> uses = assg->getModifies();
			if (uses.find(var) != uses.end()) {
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}

bool PatternAssg::match(string var, string expr) {
	// find the stmt that match this var
	// then search its subtree for the expr
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> assgStmts = stable->getAssgStmts();

	unordered_set<Statement*>::iterator iter;
	for (iter = assgStmts.begin(); iter != assgStmts.end(); iter++) {
		Statement* assg = *iter;
		if (assg->getStmtNum() == _stmtNum) {
			set<string> uses = assg->getModifies();
			if (uses.find(var) != uses.end()) {
				return matchExpr((AssgNode*)(assg->getTNodeRef()), expr);
				//return false;
			} else {
				return false;
			}
		}
	}

	return false;
}


bool PatternAssg::matchExpr(AssgNode* assg, string expr) {
	// match expr based on the assgnode given
	if (assg == NULL) {
		return false;
	}
	int i = 0;

	// stuff the rpn into the stack
	string rpn = expr.substr(2, expr.length() - 4);
	//std::stack<string> tempstack = stack<string>();
	//std::stack<string> rpnstack = stack<string>();
	std::vector<string> rpnarr = vector<string>();
	for (size_t i = 0; i < rpn.length(); i++) {
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
	for (size_t compared = 0; compared < rpnarr.size() && i < 9999; compared++) {
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
