#pragma once

#include "CFGIterator.h"
#include "AssgGNode.h"
#include "CallGNode.h"
#include "IfGNode.h"
#include "EndGNode.h"

using namespace std;


CFGIterator::CFGIterator(GNode* start) {
	startNode = start;
	nextNode = start;
	nodeStack = stack<GNodeContainer>();
	dummyNodeStack = stack<DummyGNode*>();
	numIter=-1;
}

CFGIterator::~CFGIterator() {
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGIterator::toConsiderElseStmt() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == IF_ && nodeStack.top().count == 1;
}

IfGNode* CFGIterator::getCurrentIfNode() {
	if (nodeStack.top().node->getNodeType() == IF_) {
		return static_cast<IfGNode*>(nodeStack.top().node);
	} else {
		return NULL;
	}
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGIterator::isInWhileLoop() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == WHILE_;
}

//only if node is in if container directly, does not work if node is in a while container inside an if statement
bool CFGIterator::isInIfContainer() {
	return !nodeStack.empty() && nodeStack.top().node->getNodeType() == IF_;
}

WhileGNode* CFGIterator::getCurrentWhileNode() {
	if (nodeStack.top().node->getNodeType() == WHILE_) {
		return static_cast<WhileGNode*>(nodeStack.top().node);
	} else {
		return NULL;
	}
}

void CFGIterator::skipWhileLoop(WhileGNode* node) {
	if (nodeStack.top().node == node) {
		nextNode = node->getAfterLoopChild();
		nodeStack.pop();
	}
}

void CFGIterator::skipThenStmt(IfGNode* node) {
	if (toConsiderElseStmt() && nodeStack.top().node == node) {
		nextNode = node->getElseChild();
		nodeStack.top().toContinue = false;
		nodeStack.top().count--;
	}
}

void CFGIterator::skipElseStmt(IfGNode* node) {
	if (nodeStack.top().node == node) {
		if (nodeStack.top().toContinue) {
			nextNode = dummyNodeStack.top()->getChildren().at(0);
			dummyNodeStack.pop();
		} else {
			nextNode = new EndGNode();
		}
	}
}

//skip else statement 

bool CFGIterator::isStart() {
	return numIter == 0;
}

GNode* CFGIterator::getNextNode() {
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
		case END_:
			return nextNode;
			break;
	}
}

