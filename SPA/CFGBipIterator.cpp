#pragma once

#include "CFGBipIterator.h"
#include "AssgGNode.h"
#include "CallGNode.h"
#include "IfGNode.h"
#include "EndGNode.h"
#include <boost/foreach.hpp>

using namespace std;


CFGBipIterator::CFGBipIterator(GNode* start) {
	startNode = start;
	nextNode = start;
	nodeStack = stack<GNodeContainer>();
	numIter=-1;
	parentCallStmts = stack<int>();
	end = false;
}

CFGBipIterator::~CFGBipIterator() {
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGBipIterator::toConsiderElseStmt() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == IF_ && nodeStack.top().count == 1;
}

IfGNode* CFGBipIterator::getCurrentIfNode() {
	if (nodeStack.top().node->getNodeType() == IF_) {
		return static_cast<IfGNode*>(nodeStack.top().node);
	} else {
		return NULL;
	}
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGBipIterator::isInWhileLoop() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == WHILE_;
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGBipIterator::isInIfContainer() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == IF_;
}

WhileGNode* CFGBipIterator::getCurrentWhileNode() {
	if (nodeStack.top().node->getNodeType() == WHILE_) {
		return static_cast<WhileGNode*>(nodeStack.top().node);
	} else {
		return NULL;
	}
}

void CFGBipIterator::skipWhileLoop(WhileGNode* node) {
	if (nodeStack.top().node == node) {
		nextNode = node->getAfterLoopChild();
		nodeStack.pop();
	}
}

void CFGBipIterator::skipThenStmt(IfGNode* node) {
	if (toConsiderElseStmt() && nodeStack.top().node == node) {
		nextNode = node->getElseChild();
		nodeStack.top().toContinue = false;
		nodeStack.top().count--;
	}
}

void CFGBipIterator::skipElseStmt(IfGNode* node) {
	if (nodeStack.top().node == node) {
		if (nodeStack.top().toContinue) {
			IfGNode* ifNode = static_cast<IfGNode*>(node);
			nextNode = ifNode->getExit();
		} else {
			nextNode = new EndGNode();
		}
	}
}

//skip else statement 

bool CFGBipIterator::isStart() {
	return numIter == 0;
}

bool CFGBipIterator::isEnd() {
	return end;
}

GNode* CFGBipIterator::getNextNode() {
	numIter++;
	switch(nextNode->getNodeType()) {
		case ASSIGN_:
			AssgGNode* assgNode;
			assgNode = static_cast<AssgGNode*>(nextNode);
			nextNode = assgNode->getChild();
			return assgNode;
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
			return whileNode;
		case IF_:
			IfGNode* ifNode;
			ifNode = static_cast<IfGNode*>(nextNode);
			//seeing ifNode for the first time
			nodeStack.push(GNodeContainer(ifNode, 1));
			nextNode = ifNode->getThenChild();
			return ifNode;
		case CALL_:
			CallGNode* callNode;
			callNode = static_cast<CallGNode*>(nextNode);
			nextNode = callNode->getChild();
			parentCallStmts.push(callNode->getStartStmt());
			return callNode;
		case DUMMY_:
			//only for if nodes
			DummyGNode* dummyNode;
			dummyNode = static_cast<DummyGNode*>(nextNode);
			if (nodeStack.empty() || !nodeStack.top().node->isNodeType(IF_)) {
				//node is in middle of path
				//just continue on, as the other then/else branch is not on the next* path
				nextNode = dummyNode->getChildren().at(0);
			} else if (nodeStack.top().count == 0) {
				//iterated through both then and else, pop off and not consider it again
				nodeStack.pop();
				nextNode = dummyNode->getChildren().at(0);
			} else {
				nodeStack.top().count--;
				//count is now 0, to iterate through else
				IfGNode* ifNode = static_cast<IfGNode*>(nodeStack.top().node);
				nextNode = ifNode->getElseChild();
			}
			return dummyNode;
			break;
		case PROC_:
			ProcGNode* procNode;
			procNode = static_cast<ProcGNode*>(nextNode);
			nextNode = procNode->getChild();
			return procNode;
			break;
		case END_:
			EndGNode* endNode;
			endNode = static_cast<EndGNode*>(nextNode);
			if (!parentCallStmts.empty()) {
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
					end = true;
				} else if (pos >= endNode->getChildren().size()){
					cout << "Call index is larger than children, something is wrong" << endl;
					end = true;
				} else {
					nextNode = endNode->getChildren().at(pos);
				}
			} else {
				end = true;
			}
			return endNode;
			break;
		default:
			return NULL;
	}
}

