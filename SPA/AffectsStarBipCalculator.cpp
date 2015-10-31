#include "AffectsStarBipCalculator.h"
#include "DummyGNode.h"
#include "EndGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsStarBipCalculator::AffectsStarBipCalculator() {
	stmtTable = StmtTable::getInstance();
	cfg = CFGbip::getInstance();
	globalState = State();
	globalResult = AffectsStarBipResult();
	isStart = true;
	inWhile = false;
	isEnd = false;
}

AffectsStarBipCalculator::~AffectsStarBipCalculator() {
}

bool AffectsStarBipCalculator::computeFixedFixed(string s1String, string s2String) {
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
		while (!isEnd) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (BasicAffectsStarBipTermination e) {
		return result;
	}
	return false;
}

unordered_set<string> AffectsStarBipCalculator::computeFixedSyn(string s1String) {
	//get node for s1 and s2
	s1Num = boost::lexical_cast<int>(s1String);
	Statement* s1 = stmtTable->getStmtObj(s1Num);
	type = FIXED_SYN;
	if (s1->getType() != ASSIGN_STMT_) {
		return unordered_set<string>();
	}
	GNode* s1GNode = s1->getGNodeRef();
	//adds s1's modifies to the list
	BOOST_FOREACH(string modifiedVar, s1->getModifies()) {
		unordered_set<int> modifyingStmts = unordered_set<int>();
		modifyingStmts.insert(s1->getStmtNum());
		globalState[modifiedVar] = modifyingStmts;
	}
	GNode* currentNode = s1GNode;
	try {
		while (!isEnd) {
			currentNode = evaluateNode(currentNode, globalState);
		}
	} catch (BasicAffectsStarBipTermination e) {}
	//get result
	unordered_set<string> s2 = unordered_set<string>();
	BOOST_FOREACH(auto result, globalResult) {
		s2.insert(boost::lexical_cast<string>(result.first));
	}
	return s2;
}

unordered_set<vector<string>> AffectsStarBipCalculator::computeSynSyn(bool isSameSyn) {
	type = SYN_SYN;
	vector<ProcGNode*> procNodes = cfg->getAllProcedures();
	//between each proc, reinitialise state
	BOOST_FOREACH(ProcGNode* procNode, procNodes) {
		GNode* currentNode = procNode->getChild();
		while(!isEnd) {
			currentNode = evaluateNode(currentNode, globalState);
		}
		//reset state
		globalState = State();
		isEnd = false;
	}

	unordered_set<vector<string>> multiSyns = unordered_set<vector<string>>();
	if (isSameSyn) {
		BOOST_FOREACH(auto row, globalResult) {
			int s2 = row.first;
			unordered_set<int> s1Set = row.second;
			BOOST_FOREACH(int s1, s1Set) {
				if (s1 == s2) {
					vector<string> toInsert = vector<string>();
					toInsert.push_back(boost::lexical_cast<string>(s1));
					toInsert.push_back(boost::lexical_cast<string>(s2));
					multiSyns.insert(toInsert);
				}
			}
		}
	} else {
		BOOST_FOREACH(auto row, globalResult) {
			int s2 = row.first;
			unordered_set<int> s1Set = row.second;
			BOOST_FOREACH(int s1, s1Set) {
				vector<string> toInsert = vector<string>();
				toInsert.push_back(boost::lexical_cast<string>(s1));
				toInsert.push_back(boost::lexical_cast<string>(s2));
				//cout << "test: " << s1 << ", " << s2 << endl;
				multiSyns.insert(toInsert);
			}
		}
	}
	return multiSyns;
}

//returns next node
GNode* AffectsStarBipCalculator::evaluateNode(GNode* node, State& state) {
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
		} else if (node->getNodeType() == END_) {
			if (parentCallStmts.empty()) {
				if (type == FIXED_FIXED || type == FIXED_SYN) {
					//iterate through children
					if (!node->getChildren().empty()) {
						BOOST_FOREACH(GNode* child, node->getChildren()) {
							stmtsAfterEnd.push(child);
						}
					}
					if (stmtsAfterEnd.empty()) {
						isEnd = true;
						return node;
					} else {
						GNode* nextNode = stmtsAfterEnd.top();
						stmtsAfterEnd.pop();
						return nextNode;
					}
				} else {
					//SYN_SYN: no need to consider stmts after end
					isEnd = true;
					return node;
				}
			} else {
				//proc was called by another proc
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
			//proc or dummy node
			return node->getChildren().at(0);
		}
	}
}

void AffectsStarBipCalculator::updateStateForCall(CallGNode* callNode, State& state) {
	parentCallStmts.push(callNode->getStartStmt());
}

void AffectsStarBipCalculator::updateStateForWhile(WhileGNode* whileNode, State& state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	//iterate once through first
	State state1 = recurseWhile(whileNode, state);
	//iterate one more time to get those from backloop
	AffectsStarBipResult previousResult;
	do {
		 previousResult = globalResult;
		 state1 = recurseWhile(whileNode, state1);
	} while (!areResultsEqual(previousResult, globalResult));
	//we will not check if the stmt was within while container
	//cos it might be within a call stmt, it'll be hard to check
	state = mergeStates(state, state1);
}

AffectsStarBipCalculator::State AffectsStarBipCalculator::recurseWhile(WhileGNode* whileNode, State state) {
	GNode* currentNode = whileNode->getBeforeLoopChild();
	try {
		while(currentNode != whileNode && currentNode->getNodeType() != END_) {
			currentNode = evaluateNode(currentNode, state);
		}
	} catch(EmptyStateBipTermination) {}
	return state;
}

void AffectsStarBipCalculator::updateStateForIf(IfGNode* ifNode, State& state) {
	State thenState = State();
	State elseState = State();
	try {
		thenState = recurseIf(ifNode->getThenChild(), state);
	} catch(EmptyStateBipTermination) {}
	try {
		elseState = recurseIf(ifNode->getElseChild(), state);
	} catch(EmptyStateBipTermination) {}
	//merge both states tgt
	state = mergeStates(thenState, elseState);
}

AffectsStarBipCalculator::State AffectsStarBipCalculator::recurseIf(GNode* node, State state) {
	while(node->getNodeType() != DUMMY_) {
		node = evaluateNode(node, state);
	}
	return state;
}


void AffectsStarBipCalculator::updateStateForAssign(AssgGNode* node, State& state) {
	int startNum;
	if (type != SYN_SYN && isStart) {
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
				if (type == FIXED_FIXED && stmtNum == s2Num) {
					result = true;
					throw AffectsStarBipTermination();
				}
				//add the new stmt, and all the statements that affect* it into the list
				BOOST_FOREACH(int affectsStmtNum, entry->second) {
					//insert itself first
					globalResult[stmtNum].insert(affectsStmtNum);
					//find all statement that has affected* affectsStmtNum, and add it into the result
					if (globalResult.find(affectsStmtNum) != globalResult.end()) {
						globalResult[stmtNum].insert(globalResult[affectsStmtNum].begin(), globalResult[affectsStmtNum].end());
					}
				}
			} 
			//else -> used var has not been modified by stmt before
		}

		//we found the node, just terminate only if not in while loop
		//otherwise can only eval after looping thru
		if (type == FIXED_FIXED && !inWhile && stmtNum == s2Num) {
			//failure as exception is not thrown earlier, throw exception
			throw AffectsStarBipTermination();
		}

		Statement::ModifiesSet modifiedVariables = assgStmt->getModifies();
		if (type == SYN_SYN) {
			BOOST_FOREACH(string modifiedVar, modifiedVariables) {
				unordered_set<int> modifyingStmts = unordered_set<int>();
				modifyingStmts.insert(stmtNum);
				state[modifiedVar] = modifyingStmts;
			}
		} else {
			//type == FIXED_FIXED OR FIXED_SYN
			//should only iterate once, since assg stmt only has 1 modified variable
			//we only add update the modified variable if it has been affected
			BOOST_FOREACH(string modifiedVar, modifiedVariables) {
				bool isAffectedStmt = globalResult.find(stmtNum) != globalResult.end();
				bool isReplaceExistingVal = state.find(modifiedVar) != state.end();
				if (isAffectedStmt) {
					//for each statement, add it in directly if it has been affected
					//insert or replace value
					unordered_set<int> modifyingStmts = unordered_set<int>();
					modifyingStmts.insert(stmtNum);
					state[modifiedVar] = modifyingStmts;
				} else if (isReplaceExistingVal) {
					//if it has not been affected but it replaces an existing value, reset the value
					//do not add the stmt to the table
					state.erase(modifiedVar);
				}
			}
			//we should terminate looping through the container if there is no more values in the state table
			//it is not possible for the source stmt to affect any more statements 
			//when there is no more value in the state table
			if (!inWhile && isEmpty(state)) {
				throw EmptyStateBipTermination();
			}
		}
	}
}

//get the union of both states
AffectsStarBipCalculator::State AffectsStarBipCalculator::mergeStates(State state1, State state2) {
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
bool AffectsStarBipCalculator::areResultsEqual(AffectsStarBipResult result1, AffectsStarBipResult result2) {
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

//checks if the state is empty
bool AffectsStarBipCalculator::isEmpty(State state) {
	BOOST_FOREACH(auto row, state) {
		if (!row.second.empty()) {
			return false;
		}
	}
	return true;
}