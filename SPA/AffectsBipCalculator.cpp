#include "AffectsBipCalculator.h"
#include "EndGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsBipCalculator::AffectsBipCalculator() {
	stmtTable = StmtTable::getInstance();
	cfg = CFGbip::getInstance();
	singleSynResults = unordered_set<string>();
	multiSynResults = unordered_set<vector<string>>();
	globalState = State();
	isStart = false;
	result = false;
	parentCallStmts = stack<int>();
}

AffectsBipCalculator::~AffectsBipCalculator() {
}

unordered_set<vector<string>> AffectsBipCalculator::computeAllS1AndS2() {
	//used for insertion
	type = S1_AND_S2;
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(!(currentNode->getNodeType() == END_ && parentCallStmts.empty())) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return multiSynResults;
}

unordered_set<string> AffectsBipCalculator::computeAllS1() {
	//used for insertion
	type = S1_ONLY;
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(!(currentNode->getNodeType() == END_ && parentCallStmts.empty())) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return singleSynResults;
}


bool AffectsBipCalculator::computeS1GenericS2Generic() {
	//used for insertion
	type = GENERIC_GENERIC;
	try {
		//iterate for each proc
		vector<ProcGNode*> procNodes = cfg->getAllProcedures();
		//between each proc, reinitialise state
		BOOST_FOREACH(ProcGNode* procNode, procNodes) {
			GNode* currentNode = procNode->getChild();
			while(!(currentNode->getNodeType() == END_ && parentCallStmts.empty())) {
				currentNode = evaluateNode(currentNode, globalState);
			}
			//reset state
			globalState = State();
		}
	} catch (AffectsBipTermination e) {
		return true;
	}
	return false;
}

bool AffectsBipCalculator::computeS1FixedS2Generic(string s1) {
	type = FIXED_GENERIC;
	isStart = true;
	s1Num = boost::lexical_cast<int>(s1);
	Statement* stmt1 = stmtTable->getStmtObj(s1Num);
	if (stmt1->getType() != ASSIGN_STMT_) {
		return false;
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	string modifyingVar;
	if (modifies1.size() != 1) {
		//error
		cout << "Assignment statements should only have 1 modify variable";
		return false;
	} else {
		modifyingVar = *modifies1.begin();
	}
	//add first statement into the state, so we know that which var is modified
	globalState[modifyingVar] = unordered_set<int>();
	globalState[modifyingVar].insert(s1Num);
	try {
		GNode* currentNode = stmt1->getGNodeRef();
		while(!(currentNode->getNodeType() == END_ && parentCallStmts.empty())) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (AffectsBipTermination e) {
		return result;
	}
	return false;
}

unordered_set<string> AffectsBipCalculator::computeAllS2() {
	//used for insertion
	type = S2_ONLY;
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(!(currentNode->getNodeType() == END_ && parentCallStmts.empty())) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return singleSynResults;
}

//returns next node
GNode* AffectsBipCalculator::evaluateNode(GNode* node, State& state) {
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
		inWhileLoop = true;
		updateStateForWhile(whileNode, state);
		inWhileLoop = false;
		return whileNode->getAfterLoopChild();
	} else if (node->getNodeType() == CALL_) {
		CallGNode* callNode = static_cast<CallGNode*>(node);
		updateStateForCall(callNode, state);
		return callNode->getChild();
	} else if (node->getNodeType() == END_) {
		if (parentCallStmts.empty()) {
			cout << "This should not happen." << endl;
			return node;
		} else {
			EndGNode* endNode = static_cast<EndGNode*>(node);
			//find stmt to return to, and set nextNode to that
			int parentCallStmt = parentCallStmts.top();
			parentCallStmts.pop();
			ProcGNode* originalProcNode;
			originalProcNode = static_cast<ProcGNode*>(endNode->getProcNode());
			//check position of parentCallStmt in the parents of procNode
			//get int of parent stmts
			vector<int> callStmtNums = vector<int>();
			BOOST_FOREACH(GNode* callNode, originalProcNode->getParents()) {
				callStmtNums.push_back(callNode->getStartStmt());
			}
			size_t pos = find(callStmtNums.begin(), callStmtNums.end(), parentCallStmt) - callStmtNums.begin();
			if (pos >= originalProcNode->getParents().size()) {
				cout << "Call stmt is not present in parents, something is wrong" << endl;
				return node;
			} else if (pos >= endNode->getChildren().size()){
				cout << "Call index is larger than children, something is wrong" << endl;
				return node;
			} else {
				return endNode->getChildren().at(pos);
			}
		}
	} else {
		//dummy node
		return node->getChildren().at(0);
	}
}

void AffectsBipCalculator::updateStateForCall(CallGNode* callNode, State& state) {
	parentCallStmts.push(callNode->getStartStmt());
}

void AffectsBipCalculator::updateStateForWhile(WhileGNode* whileNode, State& state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	//iterate once through first
	State state1 = recurseWhile(whileNode, state);
	//iterate one more time to get those from backloop
	State state2 = recurseWhile(whileNode, state1);
	state = mergeStates(state, state2);
}

AffectsBipCalculator::State AffectsBipCalculator::recurseWhile(WhileGNode* whileNode, State state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	while(currentNode != whileNode) {
		currentNode = evaluateNode(currentNode, state);
	}
	return state;
}

void AffectsBipCalculator::updateStateForIf(IfGNode* ifNode, State& state) {
	State thenState = recurseIf(ifNode->getThenChild(), state);
	State elseState = recurseIf(ifNode->getElseChild(), state);
	//merge both states tgt
	state = mergeStates(thenState, elseState);
}

AffectsBipCalculator::State AffectsBipCalculator::recurseIf(GNode* node, State state) {
	while(node->getNodeType() != DUMMY_) {
		node = evaluateNode(node, state);
	}
	return state;
}


void AffectsBipCalculator::updateStateForAssign(AssgGNode* node, State& state) {
	int startNum;
	if(isStart) {
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
		BOOST_FOREACH(string usedVar, usedVariables) {
			auto entry = state.find(usedVar);
			//if it has been modified previously, add affects pairs to list
			if (entry != state.end() && entry->second.size() != 0) {
				unordered_set<int> modifyingStmts = entry->second;
				switch (type) {
				case GENERIC_GENERIC: 
				case FIXED_GENERIC:
					result = true;
					throw AffectsBipTermination();
					break;
				case S1_ONLY: 
					BOOST_FOREACH(int modifyingStmt, modifyingStmts) {
							singleSynResults.insert(boost::lexical_cast<string>(modifyingStmt));
						}
					break;
				case S2_ONLY: 
					singleSynResults.insert(boost::lexical_cast<string>(stmtNum));
					break;
				case S1_AND_S2:
					BOOST_FOREACH(int modifyingStmt, modifyingStmts) {
						vector<string> affectResult = vector<string>();
						affectResult.push_back(boost::lexical_cast<string>(modifyingStmt));
						affectResult.push_back(boost::lexical_cast<string>(stmtNum));
						multiSynResults.insert(affectResult);
					}
					break;
				}
			}
		}
		Statement::ModifiesSet modifiedVariables = assgStmt->getModifies();
		if (type == FIXED_GENERIC) {
			BOOST_FOREACH(string modifiedVar, modifiedVariables) {
				if (modifiedVar == *(stmtTable->getStmtObj(s1Num)->getModifies().begin()) && !inWhileLoop) {
					result = false;
					throw AffectsBipTermination();
				}
			}
		} else {
			//see whether var modified overrides any others
			BOOST_FOREACH(string modifiedVar, modifiedVariables) {
				unordered_set<int> modifyingStmts = unordered_set<int>();
				modifyingStmts.insert(stmtNum);
				state[modifiedVar] = modifyingStmts;
			}
		}
	}
}

//get the union of both states
AffectsBipCalculator::State AffectsBipCalculator::mergeStates(State state1, State state2) {
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