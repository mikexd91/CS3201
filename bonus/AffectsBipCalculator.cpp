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

//e.g. Parent(1,2)
bool AffectsBipCalculator::computeFixedFixed(string firstArg, string secondArg) {
	type = FIXED_FIXED;
	isStart = true;
	//check modifies/uses aspects of firstArg and secondArg
	s1Num = boost::lexical_cast<int>(firstArg);
	s2Num = boost::lexical_cast<int>(secondArg);
	Statement* stmt1 = stmtTable->getStmtObj(s1Num);
	Statement* stmt2 = stmtTable->getStmtObj(s2Num);
	if (stmt1->getType() != ASSIGN_STMT_ || stmt2->getType() != ASSIGN_STMT_) {
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
	//add first statement into the state, so we know that which var is modified
	globalState[modifyingVar] = unordered_set<int>();
	globalState[modifyingVar].insert(s1Num);
	try {
		GNode* currentNode = stmt1->getGBipNodeRef();
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (BasicAffectsBipTermination e) {
		return result;
	}
	return false;
}

unordered_set<vector<string>> AffectsBipCalculator::computeAllS1AndS2(bool isS1S2Same) {
	//used for insertion
	type = S1_AND_S2;
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	if (isS1S2Same) {
		unordered_set<vector<string>> filteredResult = unordered_set<vector<string>>();
		BOOST_FOREACH(vector<string> result, multiSynResults) {
			if (result.at(0) == result.at(1)) {
				filteredResult.insert(result);
			}
		}
		return filteredResult;
	} else {
		return multiSynResults;
	}
}

unordered_set<string> AffectsBipCalculator::computeAllS1() {
	//used for insertion
	type = S1_ONLY;
	//iterate for each proc
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return singleSynResults;
}

unordered_set<string> AffectsBipCalculator::computeFixedSyn(string s1) {
	type = FIXED_SYN;
	isStart = true;
	s1Num = boost::lexical_cast<int>(s1);
	Statement* stmt1 = stmtTable->getStmtObj(s1Num);
	if (stmt1->getType() != ASSIGN_STMT_) {
		return unordered_set<string>();
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	string modifyingVar;
	if (modifies1.size() != 1) {
		//error
		cout << "Assignment statements should only have 1 modify variable";
		return unordered_set<string>();
	} else {
		modifyingVar = *modifies1.begin();
	}
	//add first statement into the state, so we know that which var is modified
	globalState[modifyingVar] = unordered_set<int>();
	globalState[modifyingVar].insert(s1Num);
	try {
		GNode* currentNode = stmt1->getGBipNodeRef();
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (BasicAffectsBipTermination e) {
		return singleSynResults;
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
			while(currentNode != NULL) {
				currentNode = evaluateNode(currentNode, globalState);
			}
			//reset state
			globalState = State();
		}
	} catch (BasicAffectsBipTermination e) {
		return result;
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
		GNode* currentNode = stmt1->getGBipNodeRef();
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (BasicAffectsBipTermination e) {
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
		while(currentNode != NULL) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
	}
	return singleSynResults;
}

//returns next node
GNode* AffectsBipCalculator::evaluateNode(GNode* node, State& state) {
	GNode* nextNode;
	if (node->getNodeType() == ASSIGN_) {
		AssgGNode* assgNode = static_cast<AssgGNode*>(node);
		updateStateForAssign(assgNode, state);
		nextNode = assgNode->getChild();
	} else if (node->getNodeType() == IF_) {
		IfGNode* ifNode = static_cast<IfGNode*>(node);
		updateStateForIf(ifNode, state);
		nextNode = ifNode->getExit()->getChildren().at(0);
	} else if (node->getNodeType() == WHILE_) {
		WhileGNode* whileNode = static_cast<WhileGNode*>(node);
		updateStateForWhile(whileNode, state);
		nextNode = whileNode->getAfterLoopChild();
	} else if (node->getNodeType() == CALL_) {
		CallGNode* callNode = static_cast<CallGNode*>(node);
		updateStateForCall(callNode, state);
		nextNode = callNode->getChild();
	} else if (node->getNodeType() == END_) {
		//proc is not called by another procedure
		if (parentCallStmts.empty()) {
			if (type == FIXED_GENERIC || type == FIXED_FIXED || type == FIXED_SYN) {
				//for fixed generic, we do not start from the beginning
				//we therefore need to consider the case where this procedure might be called from another procedure
				//we iterate through children, if there are
				//state between children of end node is not modified
				//we retain the state before entering the different children
				if (!node->getChildren().empty()) {
					BOOST_FOREACH(GNode* child, node->getChildren()) {
						updateStateBeyondEnd(child, state);
					}
				}
			}
			//otherwise, we do not consider the possibility that it was called, since we will iterate through it later
			nextNode = NULL;
		} else {
			//current proc is called by another procedure
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
				nextNode =  NULL;
			} else if (pos >= endNode->getChildren().size()){
				cout << "Call index is larger than children, something is wrong" << endl;
				nextNode = NULL;
			} else {
				nextNode =  endNode->getChildren().at(pos);
			}
		}
	} else {
		//dummy node
		nextNode =  node->getChildren().at(0);
	}
	if (!toProceed(state)) {
		throw AffectsBipTermination();
	}
	return nextNode;
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
	try {
		while(currentNode != whileNode) {
			currentNode = evaluateNode(currentNode, state);
		}
	} catch (AffectsBipTermination) {};
	return state;
}

void AffectsBipCalculator::updateStateForIf(IfGNode* ifNode, State& state) {
	State thenState = recurseIf(ifNode->getThenChild(), state);
	State elseState = recurseIf(ifNode->getElseChild(), state);
	//merge both states tgt
	state = mergeStates(thenState, elseState);
}

AffectsBipCalculator::State AffectsBipCalculator::recurseIf(GNode* node, State state) {
	try {
		while(node->getNodeType() != DUMMY_) {
			node = evaluateNode(node, state);
		}
	} catch (AffectsBipTermination) {};
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
				case FIXED_FIXED:
					if (stmtNum == s2Num) {
						result = true;
						throw BasicAffectsBipTermination();
					}
					break;
				case GENERIC_GENERIC: 
				case FIXED_GENERIC:
					result = true;
					throw BasicAffectsBipTermination();
					break;
				case S1_ONLY: 
					BOOST_FOREACH(int modifyingStmt, modifyingStmts) {
							singleSynResults.insert(boost::lexical_cast<string>(modifyingStmt));
						}
					break;
				case FIXED_SYN:
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
		if (type == FIXED_GENERIC || type == FIXED_FIXED || type == FIXED_SYN) {
			BOOST_FOREACH(string modifiedVar, modifiedVariables) {
				//if current statements modifies the var that we are looking out for
				if (modifiedVar == *(stmtTable->getStmtObj(s1Num)->getModifies().begin())) {
					if (stmtNum != s1Num) {
						//another statement overrode our assg stmt
						state.erase(modifiedVar);
					}
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

bool AffectsBipCalculator::toProceed(State state) {
	if (type == FIXED_GENERIC) {
		State::iterator it = state.find(*(stmtTable->getStmtObj(s1Num)->getModifies().begin()));
		bool isModifiedVarPresent = it != state.end();
		if (isModifiedVarPresent){
			//return true if it is still not replaced
			return it->second.find(s1Num) != it->second.end();
		} else {
			//erased
			return false;
		}
	}
	return true;
}

void AffectsBipCalculator::updateStateBeyondEnd(GNode* node, State state) {
	while (node != NULL) {
		node = evaluateNode(node, state);
	}
}