#include "ParentStarClause.h"
#include "Utils.h"
#include <iostream>

#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"

using namespace std;
using namespace boost;
using namespace stringconst;

ParentStarClause::ParentStarClause(void):SuchThatClause(PARENTSTAR_){
	stmtTable = StmtTable::getInstance();
}

ParentStarClause::~ParentStarClause(void){
}

bool ParentStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC) || (firstType == stringconst::ARG_IF);
	bool secondArg = (secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC) || (secondType == stringconst::ARG_CALL) || (secondType == stringconst::ARG_IF);
	return firstArg && secondArg;
}


//e.g. Parent*(string,string)
bool ParentStarClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	return isParentStar(s1, s2);
};
//e.g. Parent*(_,_)
bool ParentStarClause::evaluateS1GenericS2Generic() {
	//get all while statements
	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	//check if while stmt has children
	BOOST_FOREACH(Statement* whileStmt, whileStmts) {
		if (!whileStmt->getChildrenStar().empty()) {
			return true;
		}
	}
	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	BOOST_FOREACH(Statement* ifStmt, ifStmts) {
		if (!ifStmt->getChildrenStar().empty()) {
			return true;
		}
	}
	return false;
};
//e.g. Parent(_,string)
bool ParentStarClause::evaluateS1GenericS2Fixed(string s2){
	Statement::ParentStarSet parents = getParentStar(stoi(s2), stringconst::ARG_STATEMENT);
	return !parents.empty();
};
//Parent(string,_)
bool ParentStarClause::evaluateS1FixedS2Generic(string s1) {
	Statement::ChildrenStarSet  children =  getChildrenStar(stoi(s1), stringconst::ARG_STATEMENT);
	return !children.empty();
}

//Parent(string,s2)
unordered_set<string> ParentStarClause::getAllS2WithS1Fixed(string s1) {
	string argType = this->secondArgType;
	Statement::ChildrenSet children = getChildrenStar(stoi(s1), argType);
	unordered_set<string> stmtNumSet;
	BOOST_FOREACH(int child, children) {
		stmtNumSet.insert(boost::lexical_cast<string>(child));
	}
	return stmtNumSet;
}

//Parent(_,s2)
unordered_set<string> ParentStarClause::getAllS2() {
	unordered_set<string> stmtNumSet;
	//get all while statements
	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	//check if while stmt has children
	insertChildrenStarIntoStmtNum(stmtNumSet, whileStmts, this->secondArgType);
	insertChildrenStarIntoStmtNum(stmtNumSet, ifStmts, this->secondArgType);
	return stmtNumSet;
}
//Parent(s1,string)
unordered_set<string> ParentStarClause::getAllS1WithS2Fixed(string s2) {
	unordered_set<int> parents = getParentStar(stoi(s2), firstArgType);
	unordered_set<string> stmtNumSet;
	if (!parents.empty()) {
		for (auto iter = parents.begin(); iter != parents.end(); iter++) {
			stmtNumSet.insert(boost::lexical_cast<string>(*iter));
		}
	}
	return stmtNumSet;
}

//Parent(s1,__)
unordered_set<string> ParentStarClause::getAllS1() {
	unordered_set<string> stmtNumSet;
	//get all while statements
	if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_WHILE) {
		unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
		insertParentStarIntoStmtNum(stmtNumSet, whileStmts, this->secondArgType);
	}
	if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_IF) {
		unordered_set<Statement*>ifStmts = stmtTable->getIfStmts();
		insertParentStarIntoStmtNum(stmtNumSet, ifStmts, this->secondArgType);
	}
	return stmtNumSet;
}
//Parent(s1,s2)
unordered_set<vector<string>> ParentStarClause::getAllS1AndS2() {
	unordered_set<vector<string>> results = unordered_set<vector<string>>();
	//handle case where first and second args are the same -> they should not be
	if (firstArg != secondArg) {
		if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_WHILE) {
			//get all while statements
			unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
			//check if while stmt has children
			insertParentStarAndChildrenStarIntoResult(results, whileStmts);
		}
		if (firstArgType == stringconst::ARG_STATEMENT || firstArgType == stringconst::ARG_IF) {
			unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
			insertParentStarAndChildrenStarIntoResult(results, ifStmts);
		}
	}
	return results;
}

bool ParentStarClause::isParentStar(string stmt1, string stmt2) {
	//a statement cannot be a parent star of itself
	if (stmt1 == stmt2) {
		return false;
	}
	int stmtNum1 = atoi(stmt1.c_str());
	int stmtNum2 = atoi(stmt2.c_str());
	Statement::ParentStarSet stmts = getParentStar(stmtNum2, stringconst::ARG_STATEMENT);
	return stmts.find(stmtNum1) != stmts.end(); 
}

//get all parent star of stmt num that are of argType
Statement::ParentStarSet ParentStarClause::getParentStar(int stmtNum, string argType) {
	NodeType nodeType = Utils::convertArgTypeToNodeType(argType);
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == nullptr || (nodeType != NULL_ && stmtObj->getType() != nodeType)) {
		return Statement::ParentStarSet();
	} else {
		 return stmtObj->getParentStar();
	}
}

Statement::ChildrenSet ParentStarClause::getChildrenStar(int stmtNum, string stmtArgType) {
	Statement* stmtObj = stmtTable->getStmtObj(stmtNum);
	if (stmtObj == nullptr) {
		return unordered_set<int>();
	} else {
		Statement::ChildrenSet stmtSet = stmtObj->getChildrenStar();
		return Utils::filterStatements(stmtSet, Utils::convertArgTypeToNodeType(stmtArgType));
	}
}

void ParentStarClause::insertParentStarIntoStmtNum(unordered_set<string> &stmtNumSet, unordered_set<Statement*> stmts, string argType){
	BOOST_FOREACH(Statement* stmt, stmts) {
		Statement::ChildrenSet children = getChildrenStar(stmt->getStmtNum(), argType);
		if(!children.empty()) {
			stmtNumSet.insert(boost::lexical_cast<string>(stmt->getStmtNum()));
		}
	}
}

void ParentStarClause::insertParentStarAndChildrenStarIntoResult(unordered_set<vector<string>> &results, unordered_set<Statement*> stmts){
	BOOST_FOREACH(Statement* stmt, stmts) {
		Statement::ChildrenSet children = getChildrenStar(stmt->getStmtNum(), this->secondArgType);
		BOOST_FOREACH(int child, children) {
			vector<string> pair = vector<string>();
			int stmtNum = stmt->getStmtNum();
			pair.push_back(boost::lexical_cast<string>(stmt->getStmtNum()));
			pair.push_back(boost::lexical_cast<string>(child));
			results.insert(pair);
		}
	}
}


void ParentStarClause::insertChildrenStarIntoStmtNum(unordered_set<string> &stmtNumSet, unordered_set<Statement*> stmts, string argType){
	BOOST_FOREACH(Statement* stmt, stmts) {
		Statement::ChildrenSet children = getChildrenStar(stmt->getStmtNum(), argType);
		BOOST_FOREACH(int child, children) {
			stmtNumSet.insert(boost::lexical_cast<string>(child));
		}
	}
}
