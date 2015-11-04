/*
 * NextStarBipClause.cpp
 *
 *  Created on: 2 Nov, 2015
 *      Author: Leon
 */

#include "NextStarBipClause.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace stringconst;
using namespace boost;

NextStarBipClause::NextStarBipClause()
	: SuchThatClause(NEXTSTARBIP_) {
	stmtTable = StmtTable::getInstance();
	cfg = CFGbip::getInstance();
	match = false;
}

NextStarBipClause::~NextStarBipClause() {

}

bool NextStarBipClause::isValid(void) {
	bool firstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE)
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF)
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);
	bool secondArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE) 
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF) 
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);

	return firstArg && secondArg;
}

// NextBip*(4, 2)
bool NextStarBipClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	return false;
}

// NextBip*(_, _)
bool NextStarBipClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> stmts = stmtTable->getAllStmts();

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNextBip().empty()) {
			return true;
		}
	}

	return false;
}

// NextBip*(_, 2)
bool NextStarBipClause::evaluateS1GenericS2Fixed(string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	if(!stmt->getPrevBip().empty()) {
		return true;
	}

	return false;
}

// NextBip*(2, _)

bool NextStarBipClause::evaluateS1FixedS2Generic(string s1) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	if(!stmt->getNextBip().empty()) {
		return true;
	}

	return false;
}

// NextBip*(3, s) || NextBip*(3, a) || NextBip*(3, c) || NextBip*(3, if) || NextBip*(3, w)
unordered_set<string> NextStarBipClause::getAllS2WithS1Fixed(string s1) {
	results.clear();
	return results;
}

// NextBip*(_, s) || NextBip*(_, a) || NextBip*(_, c) || NextBip*(_, if) || NextBip*(_, w)
unordered_set<string> NextStarBipClause::getAllS2() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(secondArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getPrevBip().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

// NextBip*(s, 3) || NextBip*(a, 3) || NextBip*(c, 3) || NextBip*(if, 3) || NextBip*(w, 3)
unordered_set<string> NextStarBipClause::getAllS1WithS2Fixed(string s2) {
	results.clear();
	return results;
}

// NextBip*(s, _) || NextBip*(a, _) || NextBip*(c, _) || NextBip*(if, _) || NextBip*(w, 3)
unordered_set<string> NextStarBipClause::getAllS1() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(firstArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNextBip().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

unordered_set<vector<string>> NextStarBipClause::getAllS1AndS2() {
	resultsPair.clear();
	return resultsPair;
}

void NextStarBipClause::dfsFind(Statement* stmt, string str, vector<string> visited, stack<int> entrance) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();

	if(match) {
		return;
	}

	if(currStmt == str) {
		match = true;
		return;
	}

	if(!contains(visited, currStmt)) {
		visited.push_back(currStmt);

		if(bipNode->getNodeType() == CALL_) {
			ProcGNode* procChild = (ProcGNode*) bipNode->getChildren().at(0);
			int indexOfEntrance = getNodePosition(procChild->getParents(), bipNode);
			entrance.push(indexOfEntrance);
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFind(nextStmt, str, visited, entrance);
			}
		} else if(bipNode->getNodeType() == WHILE_) {
			if(bipNode->getChildren().at(1)->getNodeType() == END_) {
				int firstChildStmtNum = bipNode->getChildren().at(0)->getStartStmt();
				dfsFind(stmtTable->getStmtObj(firstChildStmtNum), str, visited, entrance);

				EndGNode* endChild = (EndGNode*) bipNode->getChildren().at(1);

				if(!entrance.empty()) {
					GNode* exit = endChild;
					bool flag = true;

					while(flag) {
						int indexOfExit = entrance.top();
						entrance.pop();
						exit = exit->getChildren().at(indexOfExit);

						if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
							flag = false;
							break;
						}

						if(exit->getNodeType() == END_ && entrance.empty()) {
							exit = NULL;
							flag = false;
							break;
						}

						if(exit->getNodeType() == END_ && !entrance.empty()) {
							continue;
						}

						if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
							exit = traverseDummyToGetNonDummy(exit);
							flag = false;
							break;
						}

						if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
							exit = traverseDummyToGetAnything(exit);
							if(exit->getNodeType() != END_) {
								flag = false;
								break;
							}
						}
					}

					if(exit != NULL) {
						dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
					}
				}
			}

			if(bipNode->getChildren().at(1)->getNodeType() == DUMMY_) {
				int firstChildStmtNum = bipNode->getChildren().at(0)->getStartStmt();
				dfsFind(stmtTable->getStmtObj(firstChildStmtNum), str, visited, entrance);

				DummyGNode* dumChild = (DummyGNode*) bipNode->getChildren().at(1);
				GNode* ultimateChild = traverseDummyToGetAnything(dumChild);

				if(ultimateChild->getNodeType() == END_) {
					if(!entrance.empty()) {
						GNode* exit;
						bool flag = true;

						while(flag) {
							int indexOfExit = entrance.top();
							entrance.pop();
						}
					}
				}
			}


		} else {

		}
	}
}

GNode* NextStarBipClause::traverseDummyToGetNonDummy(GNode* ref) {
	GNode* child = ref;

	while(true) {
		child = child->getChildren().at(0);
		
		if(child->getNodeType() != END_ && child->getNodeType() != DUMMY_) {
			return child;
		}

		if(child->getNodeType() == DUMMY_) {
			continue;
		}

		if(child->getNodeType() == END_) {
			return NULL;
		}
	}

	return NULL;
}

GNode* NextStarBipClause::traverseDummyToGetAnything(GNode* ref) {
	GNode* child = ref;

	while(true) {
		child = child->getChildren().at(0);

		if(child->getNodeType() == DUMMY_) {
			continue;
		} else {
			break;
		}
	}

	return child;
}

GNode* NextStarBipClause::getEndNodeChild(GNode* ref) {
	bool flag = true;
	GNode* child = ref;

	while(flag) {
		switch(child->getNodeType()) {
			case DUMMY_:
				child = child->getChildren().at(0);
				break;
			case END_:
				flag = false;
				break;
			default:
				child = NULL;
				flag = false;
				break;
		}
	}

	return child;
}

bool NextStarBipClause::contains(vector<string> arr, string item) {
	if(find(arr.begin(), arr.end(), item) != arr.end()) {
		return true;
	}

	return false;
}

bool NextStarBipClause::hasEndNode(vector<GNode*> children) {
	BOOST_FOREACH(auto child, children) {
		if(child->getNodeType() == END_) {
			return true;
		}
	}

	return false;
}

int NextStarBipClause::getNodePosition(vector<GNode*> arr, GNode* item) {
	int pos = find(arr.begin(), arr.end(), item) - arr.begin();

	return pos;
}

unordered_set<Statement*> NextStarBipClause::getNeededStmts(string type) {
	unordered_set<Statement*> stmts;
	
	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return stmtTable->getAllStmts();
	}

	if(type == ARG_IF) {
		return stmtTable->getIfStmts();
	}

	if(type == ARG_WHILE) {
		return stmtTable->getWhileStmts();
	}

	if(type == ARG_CALL) {
		return stmtTable->getCallStmts();
	}

	if(type == ARG_ASSIGN) {
		return stmtTable->getAssgStmts();
	}

	return stmts;
}

bool NextStarBipClause::isNeededArgType(string type, int stmtNum) {
	Statement* stmt = stmtTable->getStmtObj(stmtNum);

	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return true;
	}
	
	if(stmt->getType() == ASSIGN_STMT_ && type == ARG_ASSIGN) {
		return true;
	}

	if(stmt->getType() == WHILE_STMT_ && type == ARG_WHILE) {
		return true;
	}

	if(stmt->getType() == CALL_STMT_ && type == ARG_CALL) {
		return true;
	}

	if(stmt->getType() == IF_STMT_ && type == ARG_IF) {
		return true;
	}

	return false;
}