#include "AffectsClause.h"
#include "CFG.h"
#include "AssgGNode.h"
#include "CallGNode.h"
#include "DummyGNode.h"
#include "IfGNode.h"
#include "WhileGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsClause::AffectsClause(void):SuchThatClause(AFFECTS_) {
	cfg = CFG::getInstance();
	stmtTable = StmtTable::getInstance();
	nodeStack = stack<GNodeContainer>();
}

AffectsClause::~AffectsClause(void){
	
}

//to add if statements
bool AffectsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE);
	bool secondArg = (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool AffectsClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	//check modifies/uses aspects of firstArg and secondArg
	int stmtNum1 = boost::lexical_cast<int>(firstArg);
	int stmtNum2 = boost::lexical_cast<int>(secondArg);
	Statement* stmt1 = stmtTable->getStmtObj(stmtNum1);
	Statement* stmt2 = stmtTable->getStmtObj(stmtNum2);
	if (stmt1->getType() != ASSIGN_STMT_ || stmt2->getType() != ASSIGN_STMT_) {
		return false;
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	unordered_set<string> uses2 = stmt2->getUses();
	if (modifies1.size() != 1 || uses2.empty()) {
		//error
		if (modifies1.size() != 1) {
			cout << "Assignment statements should only have 1 modify variable";	
		}
		return false;
	} else {
		string modifyingVar = *modifies1.begin();
		if (uses2.find(modifyingVar) == uses2.end()) {
			//stmt2 does not use a variable that stmt1 modifies.
			return false;
		}
	}
	//now we know that stmt 1 modifies a variable that stmt 2 uses
	//let's check if next* applies
	//check if both are from the same procedure

	//if both are not in same procedure
	//check whether firstArg proc calls* secondArg proc
	//if calls* -> check whether firstArg modifies a statement that secondArg uses
	//else -> return false

	//if both are in same procedure
	//check if stmt2 next* stmt1
	GNode* nextNode = stmt1->getGNodeRef();
	while (nextNode->getNodeType() != END_) {
		switch(nextNode->getNodeType()) {
		case ASSIGN_:
			AssgGNode* assgNode;
			assgNode = static_cast<AssgGNode*>(nextNode);
			if ( assgNode->getEndStmt() >= stmtNum2) {
				//stmt is accessible
				return true;
			} 
			nextNode = assgNode->getChild();
			break;
		case WHILE_:
			WhileGNode* whileNode;
			whileNode = static_cast<WhileGNode*>(nextNode);
			if (nodeStack.empty() || nodeStack.top().node != whileNode) {
				//seeing whileNode for the first time
				GNodeContainer nodeContainer = GNodeContainer(whileNode, 1);
				nodeStack.push(nodeContainer);
				nextNode = whileNode->getBeforeLoopChild();
			} else {
				//encountering whileNode again
				nodeStack.pop();
				nextNode = whileNode->getAfterLoopChild();
			}
			break;
		case IF_:
			IfGNode* ifNode;
			ifNode = static_cast<IfGNode*>(nextNode);
			if (nodeStack.empty() || nodeStack.top().node != ifNode) {
				//seeing ifNode for the first time
				GNodeContainer nodeContainer = GNodeContainer(ifNode, 1);
				nodeStack.push(nodeContainer);
				nextNode = ifNode->getThenChild();
			} else if (nodeStack.top().count == 0) {
				//encountering ifNode again, but has not traversed else
				nextNode = ifNode->getElseChild();
				nodeStack.top().count--;
			} else {
				//throw exception, should not happen
			}
			break;
		case CALL_:
			CallGNode* callNode;
			callNode = static_cast<CallGNode*>(nextNode);
			nextNode = callNode;
		case DUMMY_:
			//only for if nodes
			DummyGNode* dummyNode;
			dummyNode = static_cast<DummyGNode*>(nextNode);
			if (nodeStack.empty() || nodeStack.top().node != ifNode) {
				//node is in middle of path
				//just continue on, as the other then/else branch is not on the next* path
			} else if (nodeStack.top().count == 0) {
				//iterated through both then and else, pop off and not consider it again
				nodeStack.pop();
				nextNode = dummyNode->getChildren().at(0);
			} else {
				nodeStack.top().count--;
				//count is now 0, to iterate through else
				nextNode = nodeStack.top().node;
			}
			break;
		}
	}

	return false;
}

//e.g. Parent(_,_)
bool AffectsClause::evaluateS1GenericS2Generic() {
	return false;
}

//e.g. Parent(_,2)
bool AffectsClause::evaluateS1GenericS2Fixed(string s2) {
	return false;
}

//e.g. Parent(2,_)
bool AffectsClause::evaluateS1FixedS2Generic(string s1){
	return true;
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsClause::getAllS2WithS1Fixed(string s1) {
	return unordered_set<string>();
}

//e.g. Parent(_, s2)
//get all children
unordered_set<string> AffectsClause::getAllS2() {
	return unordered_set<string>();
}

//e.g. Parent(s1,2)
//get parent of string
unordered_set<string> AffectsClause::getAllS1WithS2Fixed(string s2) {
	return unordered_set<string>();
}

//e.g. Parent(s1,_)
//get all parents
unordered_set<string> AffectsClause::getAllS1() {
	return unordered_set<string>();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsClause::getAllS1AndS2() {
	return unordered_set<vector<string>>();
}


