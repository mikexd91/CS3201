#include "PatternAssgClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "OpNode.h"

#include <set>
#include <stack>
#include <iostream>
#include "boost\lexical_cast.hpp"
#include "boost\algorithm\string.hpp"
#include "boost\unordered_map.hpp"
#include "boost\unordered\unordered_map.hpp"

using namespace stringconst;
using namespace boost;

PatternAssgClause::PatternAssgClause(const string& syn) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
	secondArgType = ARG_VARIABLE;
	secondArgFixed = false;
} 

PatternAssgClause::PatternAssgClause(const string& syn, const string& var, const string& expr) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
	secondArg = var;
	secondArgType = ARG_VARIABLE;
	secondArgFixed = false;
	this->_expr = expr;
	//cout << "setting " << syn << endl;
}

PatternAssgClause::~PatternAssgClause(void) {
}

string PatternAssgClause::getExpression() {
	return _expr;
}

void PatternAssgClause::setExpression(string expr) {
	_expr = expr;
}

bool PatternAssgClause::isValid() {
	string varType = this->getSecondArgType();
	bool checkVar = (varType == stringconst::ARG_VARIABLE);
	bool valid = checkVar;
	return valid;
}

//Results PatternAssgClause::evaluate(Results* res) {
//	// get the synonym to get stmt num of
//	string synonym = getSynonym();
//	unordered_set<string> synValues = res->selectSyn(synonym);
//
//	// get all the assg stmt nums
//	vector<int> assgNums = getAssgNums(*res, synonym);
//
//	VarTable* vtable = VarTable::getInstance();
//	vector<string> varNames = *vtable->getAllVarNames();
//
//	// var:wildcard
//		// expr:wildcard => return all a
//		// expr:notwild => return all a that match expr
//
//	// var:fixed
//		// expr:wildcard => return a using var
//		// expr:notwild	=> return a using var that match expr
//
//	// var:syn
//		// expr:wildcard => return for each
//		// expr:notwild
//
//	bool exprWildcard = getExpression() == stringconst::STRING_EMPTY;
//	bool varWildcard = getVar() == stringconst::STRING_EMPTY;
//	bool varFixed = getVarFixed();
//
//	/*cout << getExpression() << endl;
//	cout << getVar() << endl;
//	cout << getVarFixed() << endl;*/
//
//	if (varWildcard) {
//		if (exprWildcard) {
//			//cout << "varwildexprwild" << endl;
//			return *evaluateVarWildExprWild(assgNums, res);
//		} else {
//			//cout << "varwildexpr" << endl;
//			return evaulateVarWildExpr(assgNums, getExpression(), *res);
//		}
//	} else if (varFixed) {
//		if (exprWildcard) {
//			//cout << "varfixedexprwild" << endl;
//			return evaluateVarFixedExprWild(assgNums, *res);
//		} else {
//			//cout << "varfixedexpr" << endl;
//			return evaluateVarFixedExpr(assgNums, getExpression(), *res);
//		}
//	} else {
//		if (exprWildcard) {
//			//cout << "varexprwild" << endl;
//			return evaluateVarExprWild(assgNums, varNames, *res);
//		} else {
//			//cout << "varexpr" << endl;
//			return evaluateVarExpr(assgNums, varNames, getExpression(), *res);
//		}
//	}
//}


// ---- new stuff -----------------------------
// generic == underscore
// fixed == "1"
// allS1 == synonym

//e.g. Pattern "a"("a", ?)
bool PatternAssgClause::evaluateS1FixedS2Fixed(string a, string b) {
	// always return false because s1 cannot be fixed!
	return false;
}

//e.g. Pattern _(_, ?)
bool PatternAssgClause::evaluateS1GenericS2Generic() {
	// always return false because s1 cannot be generic!
	return false;
}

//e.g. Pattern _("a", ?)
bool PatternAssgClause::evaluateS1GenericS2Fixed(string a) {
	// always return false because s1 cannot be generic!
	return false;
}

//e.g. Pattern "a"(_, ?)
bool PatternAssgClause::evaluateS1FixedS2Generic(string a) {
	// always return false because s1 cannot be fixed!
	return false;
}

//e.g. Pattern "a"(a, ?)
unordered_set<string> PatternAssgClause::getAllS2WithS1Fixed(string a) {
	// always return empty set because s1 cannot be fixed!
	return unordered_set<string>();
}

//e.g. Pattern _(a, ?)
unordered_set<string> PatternAssgClause::getAllS2() {
	// always return empty set because s1 cannot be underscore!
	return unordered_set<string>();
}

//e.g. Pattern a("a", ?)
unordered_set<string> PatternAssgClause::getAllS1WithS2Fixed(string a) {
	// TODO
	return unordered_set<string>();
}

//e.g. Pattern a(_, ?)
unordered_set<string> PatternAssgClause::getAllS1() {
	// TODO
	return unordered_set<string>();
}

//e.g. Pattern a(a, ?)
Results::ResultsTable* PatternAssgClause::getAllS1AndS2() {
	// TODO
	return nullptr;
}
// ---- end new stuff --------------------------


Results* PatternAssgClause::evaluateVarWildExprWild(vector<int>& assgNums, Results* res) {
	// return all assg stmts because they match _ _
	cout << "im here";
	
	// simply insert all assgs
	for (size_t i = 0; i < assgNums.size(); i++) {
		long long stmtNum = assgNums.at(i);
		cout << stmtNum;
		string stmtNumStr = to_string(stmtNum);
		res->insertResult(getSynonym(), stmtNumStr);
	}

	res->push();
	return res;
}

Results PatternAssgClause::evaulateVarWildExpr(vector<int>& assgNums, string expr, Results res) {
	// return all a that match expr

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	// if match expr then insert
	for (size_t i = 0; i < assgNums.size(); i++) {
		int stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchExpr(assgNode, getExpression())) {
			string stmtNumStr = lexical_cast<string>(stmtNum);
			res.insertResult(getSynonym(), stmtNumStr);
		}
	}

	res.push();
	return res;
}

Results PatternAssgClause::evaluateVarFixedExprWild(vector<int>& assgNums, Results res) {
	// return all a using var

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	// if match expr then insert
	for (size_t i = 0; i < assgNums.size(); i++) {
		int stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchVar(assgNode, getVar())) {
			string stmtNumStr = lexical_cast<string>(stmtNum);
			res.insertResult(getSynonym(), stmtNumStr);
		}
	}

	res.push();
	return res;
}

Results PatternAssgClause::evaluateVarFixedExpr(vector<int>& assgNums, string expr, Results res) {
	// return all a using var that match expr

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	// if match expr then insert
	for (size_t i = 0; i < assgNums.size(); i++) {
		int stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchVar(assgNode, getVar()) && matchExpr(assgNode, getExpression())) {
			string stmtNumStr = lexical_cast<string>(stmtNum);
			res.insertResult(getSynonym(), stmtNumStr);
		}
	}

	res.push();
	return res;
}

Results PatternAssgClause::evaluateVarExprWild(vector<int>& assgNums, vector<string>& varNames, Results res) {
	// for all a and all var, return <a, var> such that matchvar(a, var)

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	for (size_t i = 0; i < assgNums.size(); i++) {
		int stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		// go through all vars
		for (size_t j = 0; j < varNames.size(); j++) {
			string var = varNames.at(j);
			if (matchVar(assgNode, var)) {
				string stmtNumStr = lexical_cast<string>(stmtNum);
				// TODO add pair result;
				unordered_map<string, string>* pair = new unordered_map<string, string>();
				(*pair)[getSynonym()] = stmtNumStr;
				(*pair)[getVar()] = var;
				res.insertMultiResult(pair);

				//res->addPairResult(stmtNumStr, var);
			}
		}
	}

	res.push();
	return res;
}

Results PatternAssgClause::evaluateVarExpr(vector<int>& assgNums, vector<string>& varNames, string expr, Results res) {
	// for all a and all var, return <a, var> such that matchvar(a, var) and matchexpr(a, expr)

	StmtTable* stable = StmtTable::getInstance();

	// go through all assgs
	for (size_t i = 0; i < assgNums.size(); i++) {
		int stmtNum = assgNums.at(i);
		Statement* assg = stable->getStmtObj(stmtNum);
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		// go through all vars
		for (size_t j = 0; j < varNames.size(); j++) {
			string var = varNames.at(j);
			if (matchVar(assgNode, var) && matchExpr(assgNode, getExpression())) {
				string stmtNumStr = lexical_cast<string>(stmtNum);
				// TODO add pair result;
				unordered_map<string, string>* pair = new unordered_map<string, string>();
				(*pair)[getSynonym()] = stmtNumStr;
				(*pair)[getVar()] = var;
				res.insertMultiResult(pair);

				//res->addPairResult(stmtNumStr, var);
			} 
		}
	}

	res.push();
	return res;
}

vector<int> PatternAssgClause::getAssgNums(Results res, string synonym) {
	vector<int> assgNums = vector<int>();

	if (res.hasResults(synonym)) {
		cout << "has results";
		// get the result set and convert that to assg statements
		unordered_set<string> synValues = res.selectSyn(synonym);

		for (unordered_set<string>::iterator iter = synValues.begin(); iter != synValues.end(); iter++) {
			int stmtNum = stoi(*iter);
			assgNums.push_back(stmtNum);
		}
	} else {
		cout << "has no results";
		// get all assignment statements
		StmtTable* stable = StmtTable::getInstance();
		set<Statement*> allAssg = stable->getAssgStmts();

		set<Statement*>::iterator assgIter;
		for (assgIter = allAssg.begin(); assgIter != allAssg.end(); assgIter++) {
			Statement* assg = *assgIter;
			assgNums.push_back(assg->getStmtNum());
		}
		//debug
		assgNums.push_back(2);
	}

	return assgNums;
}

bool PatternAssgClause::matchVar(AssgNode* assgnode, string var) {

	// match var based on varnode name
	VarNode* varnode = assgnode->getVarNode();

	return varnode->getName() == var;
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
	boost::split(rpnarr, rpn, boost::is_any_of(" "));


	// this is buggy. it breaks a many-letter var into all its letters.
	// use string split instead
	//for (size_t i = 0; i < rpn.length(); i++) {
	//	string token = rpn.substr(i, 1);
	//	if (token != " ") {
	//		//tempstack.push(token);
	//		rpnarr.push_back(token);
	//	}
	//}

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