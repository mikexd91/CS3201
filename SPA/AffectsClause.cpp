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
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
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
	Procedure* proc1 = stmt1->getProc();
	Procedure* proc2 = stmt2->getProc();
	if (stmt1->getType() != ASSIGN_STMT_ || stmt2->getType() != ASSIGN_STMT_ || proc1 != proc2) {
		return false;
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	unordered_set<string> uses2 = stmt2->getUses();
	string modifyingVar;
	if (modifies1.size() != 1 || uses2.empty()) {
		//error
		if (modifies1.size() != 1) {
			cout << "Assignment statements should only have 1 modify variable";	
		}
		return false;
	} else {
		modifyingVar = *modifies1.begin();
		if (uses2.find(modifyingVar) == uses2.end()) {
			//stmt2 does not use a variable that stmt1 modifies.
			return false;
		}
	}
	//if both are in same procedure
	//check if stmt2 next* stmt1
	CFGIterator iterator = CFGIterator(stmt1->getGNodeRef());
	GNode* currentNode = iterator.getNextNode();
	while (!currentNode->isNodeType(END_)){
		if (currentNode->isNodeType(ASSIGN_)) {
			AssgGNode* assgNode = static_cast<AssgGNode*>(currentNode);
			int startNum;
			if (iterator.isStart()) {
				startNum = stmtNum1+1;
			} else {
				startNum = assgNode->getStartStmt();
			}
			for (int i =startNum; i <= assgNode->getEndStmt(); i++) {
				if (i == stmtNum2) {
					return true;
				} else {
					Statement* assgStmt = stmtTable->getStmtObj(i);
					//if there is a statement that modifies the variable
					if (assgStmt->getModifies().find(modifyingVar) != assgStmt->getModifies().end()) {
						if(!toContinueForFixed(iterator)) {
							//break
							return false;
						}
					}
				}
			}
		} else if (currentNode->isNodeType(WHILE_)) {
			WhileGNode* whileNode = static_cast<WhileGNode*>(currentNode);
			bool isWithinWhile = stmtNum2 >= whileNode->getStartStmt() && stmtNum2 <= whileNode->getEndStmt();
			if (!isWithinWhile) {
				iterator.skipWhileLoop(whileNode);
			}
			
		} /*else if (currentNode->isNodeType(CALL_)) {
			//check if called procedure modifies var
			CallGNode* callNode = static_cast<CallGNode*>(currentNode);
			Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
			Procedure* proc = procTable->getProcObj(callStmt->getCalls());
			if (proc->getModifies().find(modifyingVar) != proc->getModifies().end()) {
				if (!toContinueForFixed(iterator)) {
					return false;
				}
			}
		}
		*/
		currentNode = iterator.getNextNode();
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
	//check modifies/uses aspects of firstArg and secondArg
	int stmtNum1 = boost::lexical_cast<int>(s1);
	Statement* stmt1 = stmtTable->getStmtObj(stmtNum1);
	if (stmt1->getType() != ASSIGN_STMT_) {
		return false;
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	string modifyingVar;
	if (modifies1.size() != 1) {
		//error
		if (modifies1.size() != 1) {
			cout << "Assignment statements should only have 1 modify variable";	
		}
		return false;
	} else {
		modifyingVar = *modifies1.begin();
	}

	//if both are in same procedure
	//check if stmt2 next* stmt1
	CFGIterator iterator = CFGIterator(stmt1->getGNodeRef());
	GNode* currentNode = iterator.getNextNode();
	while (!currentNode->isNodeType(END_)){
		if (currentNode->isNodeType(ASSIGN_)) {
			AssgGNode* assgNode = static_cast<AssgGNode*>(currentNode);
			int startNum;
			if (iterator.isStart()) {
				startNum = stmtNum1+1;
			} else {
				startNum = assgNode->getStartStmt();
			}
			for (int i =startNum; i <= assgNode->getEndStmt(); i++) {
				Statement* assgStmt = stmtTable->getStmtObj(i);
				if (assgStmt->getUses().find(modifyingVar) != assgStmt->getUses().end()) {
					return true;
				} else {
					Statement* assgStmt = stmtTable->getStmtObj(i);
					//if there is a statement that uses the variable
					if (assgStmt->getModifies().find(modifyingVar) != assgStmt->getModifies().end()) {
						//if we should consider else stmt (consider both branches) -> consider else branch
						if (!toContinue(iterator)) {
							return false;
						}
					}
				}
			}
			
		} /*else if (currentNode->isNodeType(CALL_)) {
			//check if called procedure modifies var
			CallGNode* callNode = static_cast<CallGNode*>(currentNode);
			Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
			Procedure* proc = procTable->getProcObj(callStmt->getCalls());
			if (proc->getModifies().find(modifyingVar) != proc->getModifies().end()) {
				if (!toContinue(iterator)) {
					return false;
				}
			}
		}
		*/

		currentNode = iterator.getNextNode();
	}
	return false;
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsClause::getAllS2WithS1Fixed(string s1) {
	//check modifies/uses aspects of firstArg and secondArg
	unordered_set<string> results = unordered_set<string>();
	int stmtNum1 = boost::lexical_cast<int>(s1);
	Statement* stmt1 = stmtTable->getStmtObj(stmtNum1);
	if (stmt1->getType() != ASSIGN_STMT_) {
		return unordered_set<string>();
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	string modifyingVar;
	if (modifies1.size() != 1) {
		//error
		cout << "Assignment statements should only have 1 modify variable";	
		return results;
	} else {
		modifyingVar = *modifies1.begin();
	}

	//if both are in same procedure
	//check if stmt2 next* stmt1
	CFGIterator iterator = CFGIterator(stmt1->getGNodeRef());
	GNode* currentNode = iterator.getNextNode();
	while (!currentNode->isNodeType(END_)){
		if (currentNode->isNodeType(ASSIGN_)) {
			AssgGNode* assgNode = static_cast<AssgGNode*>(currentNode);
			int startNum;
			if (iterator.isStart()) {
				startNum = stmtNum1+1;
			} else {
				startNum = assgNode->getStartStmt();
			}
			for (int i =startNum; i <= assgNode->getEndStmt(); i++) {
				Statement* assgStmt = stmtTable->getStmtObj(i);
				if (assgStmt->getUses().find(modifyingVar) != assgStmt->getUses().end()) {
					results.insert(lexical_cast<string>(assgStmt->getStmtNum()));
				}
				if (assgStmt->getModifies().find(modifyingVar) != assgStmt->getModifies().end()) {
					if(!toContinue(iterator)) {
						return results;
					}
				}
			}
			
		}/* else if (currentNode->isNodeType(CALL_)) {
			//check if called procedure modifies var
			CallGNode* callNode = static_cast<CallGNode*>(currentNode);
			Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
			Procedure* proc = procTable->getProcObj(callStmt->getCalls());
			if (proc->getModifies().find(modifyingVar) != proc->getModifies().end()) {
				if (!toContinue(iterator)) {
					return results;
				}
			}
		}
		*/
		currentNode = iterator.getNextNode();
	}
	return results;
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

bool AffectsClause::toContinue(CFGIterator iterator) {
	if (iterator.isInIfContainer()) {
		IfGNode* ifNode = iterator.getCurrentIfNode();
		if (iterator.toConsiderElseStmt()) {
			//if we should consider else stmt (consider both branches) -> consider else branch
			iterator.skipThenStmt(ifNode);
		} else {
			//we have considered both then and else stmt, let's see whether we should proceed with evaluation
			iterator.skipElseStmt(ifNode);
		}
		return true;
	} else if (iterator.isInWhileLoop()) {
		//if the bad assg stmt is in a while loop, skip the while loop
		//as there may be a path outside it that does not affect
		WhileGNode* whileNode = iterator.getCurrentWhileNode();
		iterator.skipWhileLoop(whileNode);
		return true;
	} else {
		//there is no else branch, and we are not in a while loop
		return false;
	}
}

bool AffectsClause::toContinueForFixed(CFGIterator iterator) {
	if (iterator.isInIfContainer()) {
		IfGNode* ifNode = iterator.getCurrentIfNode();
		if (iterator.toConsiderElseStmt()) {
			//if we should consider else stmt (consider both branches) -> consider else branch
			iterator.skipThenStmt(ifNode);
		} else {
			//we have considered both then and else stmt, let's see whether we should proceed with evaluation
			iterator.skipElseStmt(ifNode);
		}
		return true;
	} else {
		//2 possibilities:
		//1. in an ordinary set of assg stmt -> no other path
		//2. while loop will only occur if the start stmt is in a while loop, or if our end stmt is inside the while loop,
		//it will fail either way
		return false;
	}
}




