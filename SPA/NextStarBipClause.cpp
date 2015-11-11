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
	match = false;
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	GNode* bipNode = stmt->getGBipNodeRef();
	vector<GNode*> children = bipNode->getChildren();

	if(stmt->getStmtNum() != bipNode->getEndStmt()) {
		unordered_set<int> nextSet = stmt->getNextBip();
		BOOST_FOREACH(auto next, nextSet) {
			Statement* nextStmt = stmtTable->getStmtObj(next);
			vector<string> visited;
			stack<int> entrance;
			dfsFind(nextStmt, s2, visited, entrance);
		}
	} else {
		if(isNodeType(bipNode, CALL_)) {
			ProcGNode* calledNode = (ProcGNode*) bipNode->getChildren().at(0);
			int indexOfEntry = getNodePosition(calledNode->getParents(), bipNode);
			vector<string> visited;
			stack<int> entrance;
			entrance.push(indexOfEntry);
			dfsFind(calledNode, s2, visited, entrance);
		} else {
			BOOST_FOREACH(auto child, children) {
				vector<string> visited;
				stack<int> entrance;
				dfsFind(child, s2, visited, entrance);
			}
		}
	}
		
	return match;
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
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));
	GNode* bipNode = stmt->getGBipNodeRef();
	vector<GNode*> children = bipNode->getChildren();
	
	if(stmt->getStmtNum() != bipNode->getEndStmt()) {
		unordered_set<int> nextSet = stmt->getNextBip();
		BOOST_FOREACH(auto next, nextSet) {
			Statement* nextStmt = stmtTable->getStmtObj(next);
			vector<string> visited;
			stack<int> entrance;
			dfsFindNext(nextStmt, visited, entrance, secondArgType);
		}
	} else {
		if(isNodeType(bipNode, CALL_)) {
			ProcGNode* calledNode = (ProcGNode*) bipNode->getChildren().at(0);
			int indexOfEntry = getNodePosition(calledNode->getParents(), bipNode);
			vector<string> visited;
			stack<int> entrance;
			entrance.push(indexOfEntry);
			dfsFindNext(calledNode, visited, entrance, secondArgType);
		} else {
			BOOST_FOREACH(auto child, children) {
				vector<string> visited;
				stack<int> entrance;
				dfsFindNext(child, visited, entrance, secondArgType);
			}
		}
	}

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
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));
	GNode* bipNode = stmt->getGBipNodeRef();
	vector<GNode*> parents = bipNode->getParents();

	if(stmt->getStmtNum() != bipNode->getStartStmt()) {
		unordered_set<int> prevSet = stmt->getPrevBip();
		BOOST_FOREACH(auto prev, prevSet) {
			Statement* prevStmt = stmtTable->getStmtObj(prev);
			vector<string> visited;
			stack<int> entrance;
			dfsFindPrev(prevStmt, visited, entrance, firstArgType);
		}
	} else {
		BOOST_FOREACH(auto par, parents) {
			vector<string> visited;
			stack<int> entrance;

			if(isNodeType(par, END_)) {
				GNode* current = bipNode;

				while(true) {
					int indexOfEntry = getNodePosition(par->getChildren(), current);
					entrance.push(indexOfEntry);

					current = par;
					par = par->getParents().at(0);

					if(isNodeType(par, END_)) {
						continue;
					} else {
						break;
					}
				}
			}

			dfsFindPrev(par, visited, entrance, firstArgType);
		}
	}

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
	bool sameArg = firstArg == secondArg;
	
	vector<ProcGNode*> cfgArr = cfg->getAllProcedures();
	BOOST_FOREACH(auto proc, cfgArr) {
		vector<string> visited;
		GNode* firstChild = proc->getChild();
		stack<int> entrance;
		dfsFindAll(firstChild, visited, entrance, firstArgType, secondArgType, sameArg);
	}

	unordered_set<Statement*> callStmts = stmtTable->getCallStmts();
	BOOST_FOREACH(auto cStmt, callStmts) {
		GNode* callNode = cStmt->getGBipNodeRef();
		ProcGNode* calledNode = (ProcGNode*) callNode->getChildren().at(0);
		int indexOfEntry = getNodePosition(calledNode->getParents(), callNode);
		vector<string> visited;
		visited.push_back(lexical_cast<string>(cStmt->getStmtNum()));
		stack<int> entrance;
		entrance.push(indexOfEntry);
		dfsFindAll(calledNode, visited, entrance, firstArgType, secondArgType, sameArg);
	}

	unordered_set<Statement*> assgStmts = stmtTable->getAssgStmts();
	BOOST_FOREACH(auto aStmt, assgStmts) {
		GNode* node = aStmt->getGBipNodeRef();
		if(aStmt->getStmtNum() != node->getEndStmt()) {
			unordered_set<int> nextList = aStmt->getNextBip();
			BOOST_FOREACH(auto n, nextList) {
				vector<string> visited;
				visited.push_back(lexical_cast<string>(aStmt->getStmtNum()));
				stack<int> entrance;
				dfsFindAll(stmtTable->getStmtObj(n), visited, entrance, firstArgType, secondArgType, sameArg);
			}
		} else {
			vector<GNode*> children = node->getChildren();
			BOOST_FOREACH(auto child, children) {
				vector<string> visited;
				visited.push_back(lexical_cast<string>(aStmt->getStmtNum()));
				stack<int> entrance;
				dfsFindAll(child, visited, entrance, firstArgType, secondArgType, sameArg);
			}
		}
	}

	unordered_set<Statement*> whileStmts = stmtTable->getWhileStmts();
	BOOST_FOREACH(auto wStmt, whileStmts) {
		GNode* whileNode = wStmt->getGBipNodeRef();
		vector<GNode*> children = whileNode->getChildren();
		BOOST_FOREACH(auto child, children) {
			vector<string> visited;
			stack<int> entrance;
			dfsFindAll(child, visited, entrance, firstArgType, secondArgType, sameArg);
		}
	}

	unordered_set<Statement*> ifStmts = stmtTable->getIfStmts();
	BOOST_FOREACH(auto ifStmt, ifStmts) {
		GNode* ifNode = ifStmt->getGBipNodeRef();
		vector<GNode*> children = ifNode->getChildren();
		BOOST_FOREACH(auto child, children) {
			vector<string> visited;
			stack<int> entrance;
			dfsFindAll(child, visited, entrance, firstArgType, secondArgType, sameArg);
		}
	}

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

	if(contains(visited, currStmt) && stmt->getStmtNum() == bipNode->getStartStmt() && bipNode->getParents().at(0)->getNodeType() == PROC_) {
		Procedure* proc = stmt->getProc();
		unordered_set<int> procedureStmts = proc->getContainStmts();
		vector<string> newVisited;
		BOOST_FOREACH(auto v, visited) {
			if(procedureStmts.find(atoi(v.c_str())) == procedureStmts.end()) {
				newVisited.push_back(v);
			}
		}
		
		visited = newVisited;
	}
	
	if(!contains(visited, currStmt)) {
		visited.push_back(currStmt);

		if(stmt->getStmtNum() != bipNode->getEndStmt()) {
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFind(nextStmt, str, visited, entrance);
			}
		} else {
			vector<GNode*> children = bipNode->getChildren();
			BOOST_FOREACH(auto child, children) {
				dfsFind(child, str, visited, entrance);
			}
		}
	}
}

void NextStarBipClause::dfsFind(GNode* node, string str, vector<string> visited, stack<int> entrance) {
	if(isNodeType(node, PROC_) || isNodeType(node, DUMMY_)) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFind(child, str, visited, entrance);
		}
	}
	if(isNodeType(node, CALL_)) {
		ProcGNode* calledNode = (ProcGNode*) node->getChildren().at(0);
		int indexOfEntry = getNodePosition(calledNode->getParents(), node);
		entrance.push(indexOfEntry);

		Statement* callStmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFind(callStmt, str, visited, entrance);
	}
	if(isNodeType(node, END_) && entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFind(child, str, visited, entrance);
		}
	}
	if(isNodeType(node, END_) && !entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		GNode* exit = children.at(entrance.top());
		entrance.pop();

		dfsFind(exit, str, visited, entrance);
	}
	if(!isNodeType(node, PROC_) && !isNodeType(node, END_) && !isNodeType(node, CALL_) && !isNodeType(node, DUMMY_)) {
		Statement* stmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFind(stmt, str, visited, entrance);
	}
}

void NextStarBipClause::dfsFindNext(Statement* stmt, vector<string> visited, stack<int> entrance, string type) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();

	if(contains(visited, currStmt) && stmt->getStmtNum() == bipNode->getStartStmt() && bipNode->getParents().at(0)->getNodeType() == PROC_) {
		Procedure* proc = stmt->getProc();
		unordered_set<int> procedureStmts = proc->getContainStmts();
		vector<string> newVisited;
		BOOST_FOREACH(auto v, visited) {
			if(procedureStmts.find(atoi(v.c_str())) == procedureStmts.end()) {
				newVisited.push_back(v);
			}
		}
		
		visited = newVisited;
	}

	if(!contains(visited, currStmt)) {
		if(isNeededArgType(type, stmt->getStmtNum())) {
			results.insert(currStmt);
		}

		visited.push_back(currStmt);

		if(stmt->getStmtNum() != bipNode->getEndStmt()) {
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFindNext(nextStmt, visited, entrance, type);
			}
		} else {
			vector<GNode*> children = bipNode->getChildren();
			BOOST_FOREACH(auto child, children) {
				dfsFindNext(child, visited, entrance, type);
			}
		}
	}
}

void NextStarBipClause::dfsFindNext(GNode* node, vector<string> visited, stack<int> entrance, string type) {
	if(isNodeType(node, PROC_) || isNodeType(node, DUMMY_)) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFindNext(child, visited, entrance, type);
		}
	}
	if(isNodeType(node, CALL_)) {
		ProcGNode* calledNode = (ProcGNode*) node->getChildren().at(0);
		int indexOfEntry = getNodePosition(calledNode->getParents(), node);
		entrance.push(indexOfEntry);

		Statement* callStmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFindNext(callStmt, visited, entrance, type);
	}
	if(isNodeType(node, END_) && entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFindNext(child, visited, entrance, type);
		}
	}
	if(isNodeType(node, END_) && !entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		GNode* exit = children.at(entrance.top());
		entrance.pop();

		dfsFindNext(exit, visited, entrance, type);
	}
	if(!isNodeType(node, PROC_) && !isNodeType(node, END_) && !isNodeType(node, CALL_) && !isNodeType(node, DUMMY_)) {
		Statement* stmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFindNext(stmt, visited, entrance, type);
	}
}

void NextStarBipClause::dfsFindPrev(Statement* stmt, vector<string> visited, stack<int> entrance, string type) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();
	vector<GNode*> parents = bipNode->getParents();

	if(contains(visited, currStmt)) {
		bool isProcCycle = false;

		if(isNodeType(bipNode, WHILE_)) {
			if(isNodeType(bipNode->getChildren().at(1), END_)){
				isProcCycle = true;
			}
		} else if(isNodeType(bipNode, IF_)) {
			if(isNodeType(bipNode->getChildren().at(0), END_) || isNodeType(bipNode->getChildren().at(1), END_)) {
				isProcCycle = true;
			}
		} else if(isNodeType(bipNode, ASSIGN_) && stmt->getStmtNum() == bipNode->getEndStmt()) {
			if(isNodeType(bipNode->getChildren().at(0), END_)) {
				isProcCycle = true;
			}
		} else {
			if(isNodeType(bipNode->getChildren().at(0), END_)) {
				isProcCycle = true;
			}
		}

		if(isProcCycle) {
			Procedure* proc = stmt->getProc();
			unordered_set<int> procedureStmts = proc->getContainStmts();
			vector<string> newVisited;
			BOOST_FOREACH(auto v, visited) {
				if(procedureStmts.find(atoi(v.c_str())) == procedureStmts.end()) {
					newVisited.push_back(v);
				}
			}
		
			visited = newVisited;
		}
	}

	if(!contains(visited, currStmt)) {
		if(isNeededArgType(type, stmt->getStmtNum())) {
			results.insert(currStmt);
		}

		visited.push_back(currStmt);

		if(stmt->getStmtNum() != bipNode->getStartStmt()) {
			unordered_set<int> prevSet = stmt->getPrevBip();
			BOOST_FOREACH(auto prev, prevSet) {
				Statement* prevStmt = stmtTable->getStmtObj(prev);
				dfsFindPrev(prevStmt, visited, entrance, type);
			}
		} else {
			BOOST_FOREACH(auto par, parents) {
				if(isNodeType(par, END_)) {
					GNode* current = bipNode;

					while(true) {
						int indexOfEntry = getNodePosition(par->getChildren(), current);
						entrance.push(indexOfEntry);

						current = par;
						par = par->getParents().at(0);

						if(isNodeType(par, END_)) {
							continue;
						} else {
							break;
						}
					}
				}
				dfsFindPrev(par, visited, entrance, type);
			}
		}
	}
}

/*
 * An endNode will never enter this method, hence, there is no
 * need to check for an endNode
 */
void NextStarBipClause::dfsFindPrev(GNode* node, vector<string> visited, stack<int> entrance, string type) {
	if(isNodeType(node, PROC_) && entrance.empty()) {
		vector<GNode*> parents = node->getParents();
		BOOST_FOREACH(auto par, parents) {
			dfsFindPrev(par, visited, entrance, type);
		}
	}
	if(isNodeType(node, PROC_) && !entrance.empty()) {
		vector<GNode*> parents = node->getParents();
		GNode* exit = parents.at(entrance.top());		// definitely a call node
		entrance.pop();

		Statement* callStmt = stmtTable->getStmtObj(exit->getStartStmt());
		dfsFindPrev(callStmt, visited, entrance, type);
	}
	if(isNodeType(node, DUMMY_)) {
		vector<GNode*> parents = node->getParents();
		bool prevIsEnd = false;
		BOOST_FOREACH(auto par, parents) {
			if(prevIsEnd) {
				entrance.pop();
				prevIsEnd = false;
			}
			
			if(isNodeType(par, END_)) {
				prevIsEnd = true;
				GNode* current = node;

				while(true) {
					int indexOfEntry = getNodePosition(par->getChildren(), current);
					entrance.push(indexOfEntry);

					current = par;
					par = par->getParents().at(0);

					if(isNodeType(par, END_)) {
						continue;
					} else {
						break;
					}
				}
			}
			dfsFindPrev(par, visited, entrance, type);
		}
	}
	if(!isNodeType(node, DUMMY_) && !isNodeType(node, PROC_)) {
		Statement* stmt = stmtTable->getStmtObj(node->getEndStmt());
		dfsFindPrev(stmt, visited, entrance, type);
	}
}

void NextStarBipClause::dfsFindAll(Statement* stmt, vector<string> visited, stack<int> entrance, string type1, string type2, bool sameArg) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();
	vector<GNode*> children = bipNode->getChildren();

	if(contains(visited, currStmt) && stmt->getStmtNum() == bipNode->getStartStmt() && bipNode->getParents().at(0)->getNodeType() == PROC_) {
		Procedure* proc = stmt->getProc();
		unordered_set<int> procedureStmts = proc->getContainStmts();
		vector<string> newVisited;
		BOOST_FOREACH(auto v, visited) {
			if(procedureStmts.find(atoi(v.c_str())) == procedureStmts.end()) {
				newVisited.push_back(v);
			}
		}
		
		visited = newVisited;
	}

	if(contains(visited, currStmt)) {
		int pos = getVisitedPosition(visited, currStmt);

		for(size_t a = pos; a < visited.size(); a++) {
			for(size_t b = a; b < visited.size(); b++) {
				string first = visited.at(b);
				string second = visited.at(a);

				if(sameArg) {
					if(!(first == second)) {
						continue;
					}
				}

				if(isNeededArgType(type1, atoi(first.c_str())) && isNeededArgType(type2, atoi(second.c_str()))) {
					Statement* firstStmt = stmtTable->getStmtObj(atoi(first.c_str()));
					Statement* secStmt = stmtTable->getStmtObj(atoi(second.c_str()));
					if(firstStmt->getProc() == stmt->getProc() && secStmt->getProc() == stmt->getProc()) {
						vector<string> pair;
						pair.push_back(first);
						pair.push_back(second);
						resultsPair.insert(pair);
					}
				}
			}
		}
	} else {
		for(size_t i = 0; i < visited.size(); i++) {
			string first = visited.at(i);
			string second = currStmt;

			if(sameArg) {
				if(!(first == second)) {
					continue;
				}
			}

			if(isNeededArgType(type1, atoi(first.c_str())) && isNeededArgType(type2, atoi(second.c_str()))) {
				vector<string> pair;
				pair.push_back(first);
				pair.push_back(second);
				resultsPair.insert(pair);
			}
		}

		visited.push_back(currStmt);

		if(stmt->getStmtNum() != bipNode->getEndStmt()) {
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFindAll(nextStmt, visited, entrance, type1, type2, sameArg);
			}
		} else {
			BOOST_FOREACH(auto child, children) {
				dfsFindAll(child, visited, entrance, type1, type2, sameArg);
			}
		}
	}
}

void NextStarBipClause::dfsFindAll(GNode* node, vector<string> visited, stack<int> entrance, string type1, string type2, bool sameArg) {
	if(isNodeType(node, PROC_)) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFindAll(child, visited, entrance, type1, type2, sameArg);
		}
	}
	if(isNodeType(node, CALL_)) {
		ProcGNode* calledNode = (ProcGNode*) node->getChildren().at(0);
		int indexOfEntry = getNodePosition(calledNode->getParents(), node);
		entrance.push(indexOfEntry);

		Statement* callStmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFindAll(callStmt, visited, entrance, type1, type2, sameArg);
	}
	if(isNodeType(node, DUMMY_)) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFindAll(child, visited, entrance, type1, type2, sameArg);
		}
	}
	if(isNodeType(node, END_) && entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		BOOST_FOREACH(auto child, children) {
			dfsFindAll(child, visited, entrance, type1, type2, sameArg);
		}
	}
	if(isNodeType(node, END_) && !entrance.empty()) {
		vector<GNode*> children = node->getChildren();
		GNode* exit = children.at(entrance.top());
		entrance.pop();

		dfsFindAll(exit, visited, entrance, type1, type2, sameArg);
	}
	if(!isNodeType(node, END_) && !isNodeType(node, DUMMY_) && !isNodeType(node, CALL_) && !isNodeType(node, PROC_)) {
		Statement* stmt = stmtTable->getStmtObj(node->getStartStmt());
		dfsFindAll(stmt, visited, entrance, type1, type2, sameArg);
	}
}

bool NextStarBipClause::contains(vector<string> arr, string item) {
	if(find(arr.begin(), arr.end(), item) != arr.end()) {
		return true;
	}

	return false;
}

int NextStarBipClause::getNodePosition(vector<GNode*> arr, GNode* item) {
	int pos = find(arr.begin(), arr.end(), item) - arr.begin();
	return pos;
}

int NextStarBipClause::getVisitedPosition(vector<string> arr, string item) {
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

int NextStarBipClause::getChildNodeType(GNode* ref) {
	switch(ref->getNodeType()) {
		case WHILE_: {
			switch(ref->getChildren().at(1)->getNodeType()) {
				case END_:
					return 1;
				case DUMMY_:
					return 2;
				default:
					return 3;
			}
			break;
		}
		default: {
			switch(ref->getChildren().at(0)->getNodeType()) {
				case END_:
					return 1;
				case DUMMY_:
					return 2;
				default:
					return 3;
			}
			break;
		}
	}
}

bool NextStarBipClause::isNodeType(GNode* ref, GType type) {
	if(ref->getNodeType() == type) {
		return true;
	}
	return false;
}