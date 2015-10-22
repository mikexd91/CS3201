#include "AffectsStarCalculator.h"
#include "DummyGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsStarCalculator::AffectsStarCalculator() {
	stmtTable = StmtTable::getInstance();
	cfg = CFG::getInstance();
	singleSynResults = unordered_set<string>();
	multiSynResults = unordered_set<vector<string>>();
	globalState = State();
	globalResult = AffectsStarResult();
	isStart = true;
	inWhile = false;
}

AffectsStarCalculator::~AffectsStarCalculator() {
}
bool AffectsStarCalculator::computeFixedFixed(string s1String, string s2String) {
	//get node for s1 and s2
	s1Num = boost::lexical_cast<int>(s1String);
	s2Num = boost::lexical_cast<int>(s2String);
	Statement* s1 = stmtTable->getStmtObj(s1Num);
	Statement* s2 = stmtTable->getStmtObj(s2Num);
	type = FIXED_FIXED;
	if (s1->getType() != ASSIGN_STMT_ || s2->getType() != ASSIGN_STMT_ ) {
		return false;
	}
	GNode* s1GNode = s1->getGNodeRef();
	GNode* s2GNode = s2->getGNodeRef();
	//adds s1's modifies to the list
	BOOST_FOREACH(string modifiedVar, s1->getModifies()) {
		unordered_set<int> modifyingStmts = unordered_set<int>();
		modifyingStmts.insert(s1->getStmtNum());
		globalState[modifiedVar] = modifyingStmts;
	}
	GNode* currentNode = s1GNode;
	try {
		while (!currentNode->isNodeType(END_)) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (AffectsTermination e) {
		return result;
	}
	return false;
}

//returns next node
GNode* AffectsStarCalculator::evaluateNode(GNode* node, State& state) {
	if (node->getNodeType() == ASSIGN_) {
		AssgGNode* assgNode = static_cast<AssgGNode*>(node);
		updateStateForAssign(assgNode, state);
		return assgNode->getChild();
	} else {
		if (node->getNodeType() == IF_) {
			IfGNode* ifNode = static_cast<IfGNode*>(node);
			updateStateForIf(ifNode, state);
			return ifNode->getExit()->getChildren().at(0);
		} else if (node->getNodeType() == WHILE_) {
			WhileGNode* whileNode = static_cast<WhileGNode*>(node);
			inWhile = true;
			updateStateForWhile(whileNode, state);
			inWhile = false;
			return whileNode->getAfterLoopChild();
		} else if (node->getNodeType() == CALL_) {
			CallGNode* callNode = static_cast<CallGNode*>(node);
			updateStateForCall(callNode, state);
			return callNode->getChild();
		} else {
			//only dummy node
			DummyGNode* dummyNode = static_cast<DummyGNode*>(node);
			return dummyNode->getChildren().at(0);
		}
	}
}

void AffectsStarCalculator::updateStateForCall(CallGNode* callNode, State& state) {
	Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
	Statement::ModifiesSet modifiedVariables = callStmt->getModifies();
	//for all variables that is modified by call, reset it
	BOOST_FOREACH(string modifiedVar, modifiedVariables) {
		state[modifiedVar] =  unordered_set<int>();
	}
}

void AffectsStarCalculator::updateStateForWhile(WhileGNode* whileNode, State& state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	//iterate once through first
	State state1 = recurseWhile(whileNode, state);
	//iterate one more time to get those from backloop
	AffectsStarResult previousResult;
	do {
		 previousResult = globalResult;
		 state1 = recurseWhile(whileNode, state1);
	} while (!areResultsEqual(previousResult, globalResult));
	//check if stmt we want is in results
	if (type == FIXED_FIXED) {
		if (globalResult.find(s2Num) != globalResult.end()) {
			//s2 was already evaluated, if it is in result (meaning that it satisfy the relationship, we return true!
			//let's enforce a stricter rule
			unordered_set<int> affectsStarS2 = globalResult[s2Num];
			result = affectsStarS2.find(s1Num) != affectsStarS2.end();
			throw AffectsTermination();
		}
	}

	state = mergeStates(state, state1);
}

AffectsStarCalculator::State AffectsStarCalculator::recurseWhile(WhileGNode* whileNode, State state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	while(currentNode != whileNode && currentNode->getNodeType() != END_) {
		currentNode = evaluateNode(currentNode, state);
	}
	return state;
}

void AffectsStarCalculator::updateStateForIf(IfGNode* ifNode, State& state) {
	State thenState = recurseIf(ifNode->getThenChild(), state);
	State elseState = recurseIf(ifNode->getElseChild(), state);
	//merge both states tgt
	state = mergeStates(thenState, elseState);
}

AffectsStarCalculator::State AffectsStarCalculator::recurseIf(GNode* node, State state) {
	while(node->getNodeType() != DUMMY_) {
		node = evaluateNode(node, state);
	}
	return state;
}


void AffectsStarCalculator::updateStateForAssign(AssgGNode* node, State& state) {
	int startNum;
	if (isStart) {
		startNum = s1Num+1;
		isStart = false;
	} else {
		startNum = node->getStartStmt();
	}
	int endNum = node->getEndStmt();
	for (int stmtNum = startNum; stmtNum <= endNum; stmtNum++) {
		Statement* assgStmt = stmtTable->getStmtObj(stmtNum);
		//see whether var used has been modified previously or not
		Statement::UsesSet usedVariables = assgStmt->getUses();
		result = false;
		//for each variable that is used
		BOOST_FOREACH(string usedVar, usedVariables) {
			auto entry = state.find(usedVar);
			//if variable has been modified before -> current assgStmt has been affected by another statement
			if (entry != state.end() && entry->second.size() != 0) {
				//add the new stmt, and all the statements that affect* it into the list
				globalResult[stmtNum] = unordered_set<int>();
				BOOST_FOREACH(int affectsStmtNum, entry->second) {
					//insert itself first
					globalResult[stmtNum].insert(affectsStmtNum);
					//find all statement that has affected* affectsStmtNum, and add it into the result
					globalResult[stmtNum].insert(globalResult[affectsStmtNum].begin(), globalResult[affectsStmtNum].end());
				}
				unordered_set<int> modifyingStmts = entry->second;
				result = true;
				break;
			} 
			//else -> used var has not been modified by stmt before
		}
		//for fixed fixed
		//for each statement, add it in directly if it has been affected
		//if it does not affect, add it in if it replaces an existing value
		Statement::ModifiesSet modifiedVariables = assgStmt->getModifies();
		//should only iterate once
		BOOST_FOREACH(string modifiedVar, modifiedVariables) {
			bool isAffectedStmt = globalResult.find(stmtNum) != globalResult.end();
			bool isReplaceExistingVal = state.find(modifiedVar) != state.end();
			if (isAffectedStmt || isReplaceExistingVal) {
				//insert or replace value
				unordered_set<int> modifyingStmts = unordered_set<int>();
				modifyingStmts.insert(stmtNum);
				state[modifiedVar] = modifyingStmts;
			}
		}

		//we found the node, just terminate only if not in while loop
		//or we already know that it affects
		//otherwise can only eval after looping thru
		if (result || (!inWhile && stmtNum == s2Num)) {
			throw AffectsTermination();
		}
	}
}

//get the union of both states
AffectsStarCalculator::State AffectsStarCalculator::mergeStates(State state1, State state2) {
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

//check if the 2 states are equal
bool AffectsStarCalculator::areResultsEqual(AffectsStarResult result1, AffectsStarResult result2) {
	//for each stmt number key is result1
	//works as result2 can only contain more results than result1
	//not possible for result1 to contain results that result2 dont have
	BOOST_FOREACH(auto keyValue, result2) {
		int result2Key = keyValue.first;
		unordered_set<int> result2Values = keyValue.second;
		//if it is present in result 1, we get the results
		auto it = result1.find(result2Key);
		if (it != result1.end()) {
			unordered_set<int> result1Values = result1[result2Key];
			if (result1Values != result2Values) {
				//different values, result1 and result2 are different
				return false;
			}
		} else {
			//key not present in result1, result1 and result2 are different
			return false;
		}
	}
	return true;
}