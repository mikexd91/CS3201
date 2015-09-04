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
#include "boost\foreach.hpp"

using namespace stringconst;
using namespace boost;

PatternAssgClause::PatternAssgClause(const string& syn) 
	: PatternClause() {
	/*firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
	secondArgType = ARG_GENERIC;
	secondArgFixed = false;*/
} 

PatternAssgClause::PatternAssgClause(const string& syn, const string& var, const string& expr) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
	secondArg = var;
	// parser must set vartype and varfixed.
	this->_expr = expr;
}

PatternAssgClause::~PatternAssgClause(void) {
}

string PatternAssgClause::getExpression() {
	return _expr;
}

void PatternAssgClause::setExpression(string expr) {
	_expr = expr; 
}

bool PatternAssgClause::isExprWild() {
	return getExpression() == stringconst::STRING_EMPTY;
}

bool PatternAssgClause::isVarWild() {
	return this->getVarType() == stringconst::ARG_GENERIC;
}

bool PatternAssgClause::isValid() {
	string varType = this->getVarType();
	bool checkVar = (varType == stringconst::ARG_VARIABLE)
		|| (varType == stringconst::ARG_GENERIC);
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
	cout << "s1fs2f";
	return false;
}

//e.g. Pattern _(_, ?)
bool PatternAssgClause::evaluateS1GenericS2Generic() {
	// always return false because s1 cannot be generic!
	cout << "s1gs2g";
	return false;
}

//e.g. Pattern _("a", ?)
bool PatternAssgClause::evaluateS1GenericS2Fixed(string a) {
	// always return false because s1 cannot be generic!
	cout << "s1gs2f";
	return false;
}

//e.g. Pattern "a"(_, ?)
bool PatternAssgClause::evaluateS1FixedS2Generic(string a) {
	// always return false because s1 cannot be fixed!
	cout << "s1fs2g";
	return false;
}

//e.g. Pattern "a"(a, ?)
unordered_set<string> PatternAssgClause::getAllS2WithS1Fixed(string a) {
	// always return empty set because s1 cannot be fixed!
	cout << "s1fs2n";
	return unordered_set<string>();
}

//e.g. Pattern _(a, ?)
unordered_set<string> PatternAssgClause::getAllS2() {
	// always return empty set because s1 cannot be underscore!
	cout << "s2n";
	return unordered_set<string>();
}

//e.g. Pattern a("a", ?)
unordered_set<string> PatternAssgClause::getAllS1WithS2Fixed(string var) {
	cout << "s1ns2f";
	// TODO
	// choices:
	//	var fixed expr wild
	//	var fixed expr fixed
	if (isExprWild()) {
		return evaluateVarFixedExprWild();
	} else {
		return evaluateVarFixedExprFixed();
	}
}

//e.g. Pattern a(_, ?)
unordered_set<string> PatternAssgClause::getAllS1() {
	cout << "s1n";
	// TODO
	// choices:
	//	var wild expr wild
	//	var wild expr fixed
	if (isExprWild()) {
		// varwild exprwild
		return evaluateVarWildExprWild();
	} else {
		// varwild expr
		return evaulateVarWildExprFixed();
	}
}

//e.g. Pattern a(a, ?)
Results::ResultsTable* PatternAssgClause::getAllS1AndS2() {
	cout << "s1ns2n";
	// TODO
	// choices:
	//	var syn expr wild
	//	var syn expr fixed
	if (isExprWild()) {
		// varsyn exprwild
		return evaluateVarSynExprWild();
	} else {
		// varsyn exprfixed
		return evaluateVarSynExprFixed();
	}
}
// ---- end new stuff --------------------------


unordered_set<string> PatternAssgClause::evaluateVarWildExprWild() {
	// return all assg stmts because they match _ _

	unordered_set<string> resultsSet = unordered_set<string>();
	
	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> assgStmts = stable->getAssgStmts();
	BOOST_FOREACH(Statement* stmt, assgStmts) {
		resultsSet.insert(boost::lexical_cast<string>(stmt->getStmtNum()));
	}

	return resultsSet;
}

unordered_set<string> PatternAssgClause::evaulateVarWildExprFixed() {
	// return all a that match expr

	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> allAssg = stable->getAssgStmts();
	unordered_set<string> resSet = unordered_set<string>();

	// go through all assgs
	// if match expr then insert
	BOOST_FOREACH(Statement* assg, allAssg) {
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchExpr(assgNode, getExpression())) {
			string stmtNumStr = lexical_cast<string>(assg->getStmtNum());
			resSet.insert(stmtNumStr);
		}
	}
	return resSet;
}

unordered_set<string> PatternAssgClause::evaluateVarFixedExprWild() {
	// return all a using var

	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> allAssg = stable->getAssgStmts();
	unordered_set<string> resSet = unordered_set<string>();

	// go through all assgs
	// if match expr then insert
	BOOST_FOREACH(Statement* assg, allAssg) {
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchVar(assgNode, getVar())) {
			string stmtNumStr = lexical_cast<string>(assg->getStmtNum());
			resSet.insert(stmtNumStr);
		}
	}
	return resSet;
}

unordered_set<string> PatternAssgClause::evaluateVarFixedExprFixed() {
	// return all a using var that match expr

	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> allAssg = stable->getAssgStmts();
	unordered_set<string> resSet = unordered_set<string>();

	// go through all assgs
	// if match expr then insert
	BOOST_FOREACH(Statement* assg, allAssg) {
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		if (matchVar(assgNode, getVar()) && matchExpr(assgNode, getExpression())) {
			string stmtNumStr = lexical_cast<string>(assg->getStmtNum());
			resSet.insert(stmtNumStr);
		}
	}

	return resSet;
}

Results::ResultsTable* PatternAssgClause::evaluateVarSynExprWild() {
	// for all a and all var, return <a, var> such that matchvar(a, var)

	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> allAssg = stable->getAssgStmts();
	VarTable* vtable = VarTable::getInstance();
	vector<string>* varNames = vtable->getAllVarNames();//->getAllVarNames();
	Results::ResultsTable* res = new Results::ResultsTable();

	// go through all assgs
	BOOST_FOREACH(Statement* assg, allAssg) {
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		// go through all vars
		for (size_t j = 0; j < varNames->size(); j++) {
			string var = varNames->at(j);
			if (matchVar(assgNode, var)) {
				string stmtNumStr = lexical_cast<string>(assgNode->getStmtNum());
				// TODO add pair result;
				Results::Row* pair = new Results::Row();
				(*pair)[getSynonym()] = stmtNumStr;
				(*pair)[getVar()] = var;
				res->insert(pair);

				//res->addPairResult(stmtNumStr, var);
			}
		}
	}

	return res;
}

Results::ResultsTable* PatternAssgClause::evaluateVarSynExprFixed() {
	// for all a and all var, return <a, var> such that matchvar(a, var) and matchexpr(a, expr)

	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> allAssg = stable->getAssgStmts();
	VarTable* vtable = VarTable::getInstance();
	vector<string>* varNames = vtable->getAllVarNames();//->getAllVarNames();
	Results::ResultsTable* res = new Results::ResultsTable();

	// go through all assgs
	BOOST_FOREACH(Statement* assg, allAssg) {
		AssgNode* assgNode = (AssgNode*) assg->getTNodeRef();
		// go through all vars
		for (size_t j = 0; j < varNames->size(); j++) {
			string var = varNames->at(j);
			if (matchVar(assgNode, var) && matchExpr(assgNode, getExpression())) {
				string stmtNumStr = lexical_cast<string>(assgNode->getStmtNum());
				// TODO add pair result;
				Results::Row* pair = new Results::Row();
				(*pair)[getSynonym()] = stmtNumStr;
				(*pair)[getVar()] = var;
				res->insert(pair);
			}
		}
	}

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
		unordered_set<Statement*> allAssg = stable->getAssgStmts();

		set<Statement*>::iterator assgIter;
		BOOST_FOREACH(Statement* assg, allAssg) {
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