#include "AffectsCalculator.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsCalculator::AffectsCalculator() {
	cfg = CFG::getInstance();
	results = unordered_set<vector<string>>();
	globalState = State();
}

unordered_set<vector<string>> AffectsCalculator::computeAllS1AndS2() {
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(currentNode->getNodeType() != END_) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return results;
}


//returns next node
GNode* AffectsCalculator::evaluateNode(GNode* node, State& state) {
	if (node->getNodeType() == ASSIGN_) {
		AssgGNode* assgNode = static_cast<AssgGNode*>(node);
		updateStateForAssign(assgNode, state);
		return assgNode->getChild();
	} else if (node->getNodeType() == IF_) {
		IfGNode* ifNode = static_cast<IfGNode*>(node);
		updateStateForIf(ifNode, state);
		return ifNode->getExit()->getChildren().at(0);
	} else if (node->getNodeType() == WHILE_) {
		WhileGNode* whileNode = static_cast<WhileGNode*>(node);
		updateStateForWhile(whileNode, state);
		return whileNode->getAfterLoopChild();
	} else if (node->getNodeType() == CALL_) {
		CallGNode* callNode = static_cast<CallGNode*>(node);
		updateStateForCall(callNode, state);
		return callNode->getChild();
	} else {
		//only dummy node
		cout << "Why are we accessing this unknown node?" << endl;
		return node->getChildren().at(0);
	}
}

void AffectsCalculator::updateStateForCall(CallGNode* callNode, State& state) {
	Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
	Statement::ModifiesSet modifiedVariables = callStmt->getModifies();
	//for all variables that is modified by call, reset it
	BOOST_FOREACH(string modifiedVar, modifiedVariables) {
		state[modifiedVar] =  unordered_set<int>();
	}
}

void AffectsCalculator::updateStateForWhile(WhileGNode* whileNode, State& state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	//iterate once through first
	State state1 = recurseWhile(currentNode, state);
	//iterate one more time to get those from backloop
	State state2 = recurseWhile(currentNode, state1);
	state = mergeStates(state, state2);
}

AffectsCalculator::State AffectsCalculator::recurseWhile(GNode* node, State state) {
	while(node->getNodeType() != DUMMY_) {
		evaluateNode(node, state);
	}
	return state;
}

void AffectsCalculator::updateStateForIf(IfGNode* ifNode, State& state) {
	State thenState = recurseIf(ifNode->getThenChild(), state);
	State elseState = recurseIf(ifNode->getElseChild(), state);
	//merge both states tgt
	state = mergeStates(thenState, elseState);
}

AffectsCalculator::State AffectsCalculator::recurseIf(GNode* node, State state) {
	while(node->getNodeType() != DUMMY_) {
		evaluateNode(node, state);
	}
	return state;
}


void AffectsCalculator::updateStateForAssign(AssgGNode* node, State& state) {
	int startNum = node->getStartStmt();
	int endNum = node->getEndStmt();
	for (int stmtNum = startNum; stmtNum <= endNum; stmtNum++) {
		Statement* assgStmt = stmtTable->getStmtObj(stmtNum);
		//see whether var used has been modified previously or not
		Statement::UsesSet usedVariables = assgStmt->getUses();
		BOOST_FOREACH(string usedVar, usedVariables) {
			auto entry = state.find(usedVar);
			//if it has been modified previously, add affects pairs to list
			if (entry != state.end() && entry->second.size() != 0) {
				unordered_set<int> modifyingStmts = entry->second;
				BOOST_FOREACH(int modifyingStmt, modifyingStmts) {
					vector<string> affectResult = vector<string>();
					affectResult.push_back(boost::lexical_cast<string>(modifyingStmt));
					affectResult.push_back(boost::lexical_cast<string>(stmtNum));
					results.insert(affectResult);
				}
			}
		}
		//see whether var modified overrides any others
		Statement::ModifiesSet modifiedVariables = assgStmt->getModifies();
		BOOST_FOREACH(string modifiedVar, modifiedVariables) {
			unordered_set<int> modifyingStmts = unordered_set<int>();
			modifyingStmts.insert(stmtNum);
			state[modifiedVar] = modifyingStmts;
		}
	}
}

//get the union of both states
AffectsCalculator::State AffectsCalculator::mergeStates(State state1, State state2) {
	//for each var in state 2
	BOOST_FOREACH(auto keyValue, state2) {
		string var = keyValue.first;
		unordered_set<int> statements = keyValue.second;
		//if it is present in state 1, we combine them together
		auto it = state1.find(var);
		if (it != state1.end()) {
			BOOST_FOREACH(auto value, statements) {
				it->second.insert(value);
			}
		} else {
			//otherwise, we create a new row in state 1 for the variable
			state1[var] = statements;
		}
	}
	return state1;
}