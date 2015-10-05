/*
 * CFGBuilder.cpp
 *
 *  Created on: 14 Sep, 2015
 *      Author: Leon
 */

#include "CFGBuilder.h"

using namespace std;

bool CFGBuilder::instanceFlag = false;
CFGBuilder* CFGBuilder::cfgBuilderInstance = NULL;

CFGBuilder::CFGBuilder() {
	stmtCounter = 0;
	currNestingLevel = 0;
	currentProcedure = NULL;
	head = NULL;
	prevState = NONE;
}

CFGBuilder* CFGBuilder::getInstance() {
	if(!instanceFlag) {
		cfgBuilderInstance = new CFGBuilder();
		instanceFlag = true;
	}

	return cfgBuilderInstance;
}

void CFGBuilder::resetInstanceFlag() {
	instanceFlag = false;
}

void CFGBuilder::processParsedData(ParsedData data) {
	switch(data.getType()) {
		case ParsedData::PROCEDURE:
			processProcStmt(data);
			break;
		case ParsedData::ASSIGNMENT: case ParsedData::CALL:
			processAssgCallStmt(data);
			break;
		case ParsedData::IF:
			processIfStmt(data);
			break;
		case ParsedData::ELSE:
			processElseStmt(data);
			break;
		case ParsedData::WHILE:
			processWhileStmt(data);
			break;
		case ParsedData::END:
			processEndProgram(data);
			break;
		default:
			break;
	}
}

void CFGBuilder::processProcStmt(ParsedData data) {
	CFG* cfg = CFG::getInstance();

	ProcCNode* procNode = new ProcCNode(data.getProcName());
	EndCNode* endNode = new EndCNode();

	int diff = currNestingLevel - 1;
	for(int i = diff; i > 0; i--) {
		if(i > 1) {
			if(nestingStack.top()->getNodeType() == IF_) {
				DummyCNode* dumNode = new DummyCNode();

				dumNode->setIfParentStmt(ifStack.top()->getEndStmt());
				dumNode->setElseParentStmt(head->getEndStmt());

				dumNode->setFirstParent(ifStack.top());
				dumNode->setSecondParent(head);

				setRecoverChild(ifStack.top(), dumNode);
				setRecoverChild(head, dumNode);

				head = dumNode;
				ifStack.pop();
				nestingStack.pop();
			} else {
				if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
					head->setFirstChild(nestingStack.top());
				} else {
					head->setSecondChild(nestingStack.top());
				}
				nestingStack.top()->setSecondParent(head);
				addNextAndPrev(head, nestingStack.top());

				head = nestingStack.top();
				nestingStack.pop();
			}
		} else /* i == 1 */ {
			if(nestingStack.top()->getNodeType() == IF_) {
				endNode->setFirstParent(ifStack.top());
				endNode->setSecondParent(head);

				setRecoverChild(ifStack.top(), endNode);
				setRecoverChild(head, endNode);

				ifStack.pop();
			} else {
				if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_){
					head->setFirstChild(nestingStack.top());
				} else {
					head->setSecondChild(nestingStack.top());
				}
				nestingStack.top()->setSecondParent(head);

				nestingStack.top()->setSecondChild(endNode);
				endNode->setFirstParent(nestingStack.top());

			}

			nestingStack.pop();
		}
	}

	if(diff == 0) {
		head->setFirstChild(endNode);
		endNode->setFirstParent(head);
	}

	if(currentProcedure != NULL) {
		cfg->addProcedure(currentProcedure);
	}

	head = procNode;
	currentProcedure = procNode;
	currNestingLevel = data.getNestingLevel() + 1;
	prevState = PROCEDURE;
}

void CFGBuilder::processAssgCallStmt(ParsedData data) {
	// Assg and calls can have 3 scenarios of nesting level
	// Either the nesting increases by 1, or it recovers by any amount
	// or stays the same
	if(data.getNestingLevel() < currNestingLevel) {
		int diff = currNestingLevel - data.getNestingLevel();
		AssgCallCNode* node = new AssgCallCNode(++stmtCounter);

		for(int i = diff; i > 0; i--) {
			if(i > 1) {
				if(nestingStack.top()->getNodeType() == IF_) {
					DummyCNode* dumNode = new DummyCNode();

					dumNode->setIfParentStmt(ifStack.top()->getEndStmt());
					dumNode->setElseParentStmt(head->getEndStmt());

					dumNode->setFirstParent(ifStack.top());
					dumNode->setSecondParent(head);

					setRecoverChild(ifStack.top(), dumNode);
					setRecoverChild(head, dumNode);

					head = dumNode;
					ifStack.pop();
					nestingStack.pop();
				} else {
					if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					head = nestingStack.top();
					nestingStack.pop();
				}
			} else /* i == 1 */ {
				if(nestingStack.top()->getNodeType() == IF_) {
					CNode* ifParentNode = ifStack.top();
					CNode* elseParentNode = head;

					node->setFirstParent(ifParentNode);
					node->setSecondParent(elseParentNode);

					setRecoverChild(ifParentNode, node);
					setRecoverChild(elseParentNode, node);

					addNextAndPrev(node, ifParentNode);
					addNextAndPrev(node, elseParentNode);

					head = node;
					ifStack.pop();
					nestingStack.pop();
				} else /* top is while */ {
					if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					// add the assignment after all the recovery
					nestingStack.top()->setSecondChild(node);
					node->setFirstParent(nestingStack.top());
					addNextAndPrev(node, nestingStack.top());

					head = node;
					nestingStack.pop();
				}
			}
		}
	} else /* entering nesting */ {
		if(prevState == ASSGCALL) {
			int prevStmt = head->getEndStmt();
			++stmtCounter;

			addNextAndPrev(stmtCounter, prevStmt);
			head->setEndStmt(stmtCounter);
			return;
		}

		AssgCallCNode* node = new AssgCallCNode(++stmtCounter);
		node->setFirstParent(head);
		if(prevState == IF || prevState == WHILE) {
			head->setFirstChild(node);
			addNextAndPrev(node, head);
		} else if(prevState == ELSE) {
			head->setSecondChild(node);
			addNextAndPrev(node, head);
		} else /* dummy */ {
			head->setFirstChild(node);
		}
		head = node;
	}

	prevState = ASSGCALL;
	currNestingLevel = data.getNestingLevel();
}

void CFGBuilder::processIfStmt(ParsedData data) {
	IfCNode* node = new IfCNode(++stmtCounter);

	if(currNestingLevel > data.getNestingLevel()) {
		int diff = currNestingLevel - data.getNestingLevel();

		for(int i = diff; i > 0; i--) {
			if (i > 1) {
				if (nestingStack.top()->getNodeType() == IF_) {
					DummyCNode* dumNode = new DummyCNode();

					dumNode->setIfParentStmt(ifStack.top()->getEndStmt());
					dumNode->setElseParentStmt(head->getEndStmt());

					dumNode->setFirstParent(ifStack.top());
					dumNode->setSecondParent(head);

					setRecoverChild(ifStack.top(), dumNode);
					setRecoverChild(head, dumNode);

					head = dumNode;
					ifStack.pop();
					nestingStack.pop();
				} else {
					if (head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					head = nestingStack.top();
					nestingStack.pop();
				}
			} else /* i == 1 */{
				if(nestingStack.top()->getNodeType() == IF_) {
					node->setFirstParent(ifStack.top());
					node->setSecondParent(head);
					setRecoverChild(ifStack.top(), node);
					setRecoverChild(head, node);

					addNextAndPrev(node, ifStack.top());
					addNextAndPrev(node, head);

					head = node;
					nestingStack.pop();
					ifStack.pop();
				} else /* while stmt */{
					if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					//recovery
					nestingStack.top()->setSecondChild(node);
					node->setFirstParent(nestingStack.top());
					addNextAndPrev(node, nestingStack.top());

					head = node;
					nestingStack.pop();
				}
			}
		}

	} else {
		// if prev is assg/call it can only continue on with the same nesting level
		if(prevState == ASSGCALL) {
			prevState = IF;

			int prevInt = head->getEndStmt();
			addNextAndPrev(stmtCounter, prevInt);

			head->setFirstChild(node);
			node->setFirstParent(head);
			head = node;

			nestingStack.push(node);

			return;
		}

		// prev state was either if/else/while
		// will definitely enter nesting
		node->setFirstParent(head);
		if(prevState == IF || prevState == WHILE) {
			head->setFirstChild(node);
			addNextAndPrev(node, head);
		} else if(prevState == ELSE) {
			head->setSecondChild(node);
			addNextAndPrev(node, head);
		} else /* dummy */{
			head->setFirstChild(node);
		}
		head = node;
	}

	currNestingLevel = data.getNestingLevel() + 1;
	prevState = IF;
	nestingStack.push(node);
}

void CFGBuilder::processElseStmt(ParsedData data) {
	// dummy node parents will always set the dummy node as the first child
	// when processing an else stmt, the nesting level will always decrease
	// when nesting level decreases by one, the top of the nesting stack will
	// always be an if to pair it up
	int diff = currNestingLevel - data.getNestingLevel();

	for(int i = diff; i > 0; i--) {
		if(i == 1) {
			prevState = ELSE;
			ifStack.push(head);
			head = nestingStack.top();
		} else {
			if(nestingStack.top()->getNodeType() == IF_) {		// recovering from an if nest
				DummyCNode* dumNode = new DummyCNode;
				CNode* ifParent = ifStack.top();

				dumNode->setIfParentStmt(ifParent->getEndStmt());
				dumNode->setElseParentStmt(head->getEndStmt());

				dumNode->setFirstParent(ifParent);
				dumNode->setSecondParent(head);

				setRecoverChild(ifParent, dumNode);
				setRecoverChild(head, dumNode);

				head = dumNode;
				nestingStack.pop();
				ifStack.pop();
			} else {											// recovering from a while nest
				CNode* whileParent = nestingStack.top();
				head->setFirstChild(whileParent);
				setRecoverParent(whileParent, head);
				whileParent->setSecondParent(head);
				addNextAndPrev(whileParent, head);

				head = whileParent;
				nestingStack.pop();
			}
		}
	}

	currNestingLevel = data.getNestingLevel() + 1;
}

void CFGBuilder::processWhileStmt(ParsedData data) {
	WhileCNode* node = new WhileCNode(++stmtCounter);

	if(currNestingLevel > data.getNestingLevel()) {
		int diff = currNestingLevel - data.getNestingLevel();

		for(int i = diff; i > 0; i--) {
			if (i > 1) {
				if (nestingStack.top()->getNodeType() == IF_) {
					DummyCNode* dumNode = new DummyCNode();

					dumNode->setIfParentStmt(ifStack.top()->getEndStmt());
					dumNode->setElseParentStmt(head->getEndStmt());

					dumNode->setFirstParent(ifStack.top());
					dumNode->setSecondParent(head);

					setRecoverChild(ifStack.top(), dumNode);
					setRecoverChild(head, dumNode);

					head = dumNode;
					ifStack.pop();
					nestingStack.pop();
				} else {
					if (head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					head = nestingStack.top();
					nestingStack.pop();
				}
			} else /* i == 1 */{
				if(nestingStack.top()->getNodeType() == IF_) {
					node->setFirstParent(ifStack.top());
					node->setSecondParent(head);
					setRecoverChild(ifStack.top(), node);
					setRecoverChild(head, node);

					addNextAndPrev(node, ifStack.top());
					addNextAndPrev(node, head);

					head = node;
					nestingStack.pop();
					ifStack.pop();
				} else /* while stmt */{
					if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
						head->setFirstChild(nestingStack.top());
					} else {
						head->setSecondChild(nestingStack.top());
					}
					nestingStack.top()->setSecondParent(head);
					addNextAndPrev(nestingStack.top(), head);

					//recovery
					nestingStack.top()->setSecondChild(node);
					node->setFirstParent(nestingStack.top());
					addNextAndPrev(node, nestingStack.top());

					head = node;
					nestingStack.pop();
				}
			}
		}
	} else /* entering nesting or stay the same */ {
		if(prevState == ASSGCALL) { /* same nesting level */
			prevState = WHILE;

			addNextAndPrev(node, head);

			head->setFirstChild(node);
			node->setFirstParent(head);
			head = node;

			nestingStack.push(node);

			return;
		}

		node->setFirstParent(head);
		if(prevState == IF || prevState == WHILE) {
			head->setFirstChild(node);
			addNextAndPrev(node, head);
		} else if(prevState == ELSE){
			head->setSecondChild(node);
			addNextAndPrev(node, head);
		} else /* dummy or procedure */{
			head->setFirstChild(node);
		}

		head = node;
	}

	currNestingLevel = data.getNestingLevel() + 1;
	prevState = WHILE;
	nestingStack.push(node);
}

void CFGBuilder::processEndProgram(ParsedData data) {
	CFG* cfg = CFG::getInstance();

	EndCNode* endNode = new EndCNode();

	int diff = currNestingLevel - 1;

	for(int i = diff; i > 0; i--) {
		if(i > 1) {
			if(nestingStack.top()->getNodeType() == IF_) {
				DummyCNode* dumNode = new DummyCNode();

				dumNode->setIfParentStmt(ifStack.top()->getEndStmt());
				dumNode->setElseParentStmt(head->getEndStmt());

				dumNode->setFirstParent(ifStack.top());
				dumNode->setSecondParent(head);

				setRecoverChild(ifStack.top(), dumNode);
				setRecoverChild(head, dumNode);

				head = dumNode;
				ifStack.pop();
				nestingStack.pop();
			} else {
				if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_) {
					head->setFirstChild(nestingStack.top());
				} else {
					head->setSecondChild(nestingStack.top());
				}
				nestingStack.top()->setSecondParent(head);
				addNextAndPrev(nestingStack.top(), head);

				head = nestingStack.top();
				nestingStack.pop();
			}
		} else /* i == 1 */ {
			if(nestingStack.top()->getNodeType() == IF_) {
				endNode->setFirstParent(ifStack.top());
				endNode->setSecondParent(head);

				setRecoverChild(ifStack.top(), endNode);
				setRecoverChild(head, endNode);

				ifStack.pop();
			} else {
				if(head->getNodeType() == ASSIGN_CALL_ || head->getNodeType() == DUMMY_){
					head->setFirstChild(nestingStack.top());
				} else {
					head->setSecondChild(nestingStack.top());
				}
				nestingStack.top()->setSecondParent(head);
				addNextAndPrev(nestingStack.top(), head);

				nestingStack.top()->setSecondChild(endNode);
				endNode->setFirstParent(nestingStack.top());

			}

			nestingStack.pop();
		}
	}

	if(diff == 0) {
		head->setFirstChild(endNode);
		endNode->setFirstParent(head);
	}

	if(currentProcedure != NULL) {
		cfg->addProcedure(currentProcedure);
	}

	head = endNode;
}

void CFGBuilder::addNextAndPrev(CNode* next, CNode* prev) {
	StmtTable* stmtTable = StmtTable::getInstance();

	if(prev->getNodeType() == DUMMY_ && next->getNodeType() != DUMMY_) {
		int nextStmt = next->getStartStmt();
		int prevStmt1 = prev->getStartStmt();
		int prevStmt2 = prev->getEndStmt();

		Statement* nextStatement = stmtTable->getStmtObj(nextStmt);
		unordered_set<int> nextSetAdded = nextStatement->getPrev();
		nextSetAdded.insert(prevStmt1);
		nextSetAdded.insert(prevStmt2);
		nextStatement->setPrev(nextSetAdded);

		Statement* prevStatement1 = stmtTable->getStmtObj(prevStmt1);
		unordered_set<int> prevSetAdded1 = prevStatement1->getNext();
		prevSetAdded1.insert(nextStmt);
		prevStatement1->setNext(prevSetAdded1);

		Statement* prevStatement2 = stmtTable->getStmtObj(prevStmt2);
		unordered_set<int> prevSetAdded2 = prevStatement2->getNext();
		prevSetAdded2.insert(nextStmt);
		prevStatement2->setNext(prevSetAdded1);
	} else if(prev->getNodeType() != DUMMY_ && next->getNodeType() != DUMMY_) {
		int nextStmt = next->getStartStmt();
		int prevStmt = prev->getEndStmt();

		Statement* nextStatement = stmtTable->getStmtObj(nextStmt);
		unordered_set<int> nextSetAdded = nextStatement->getPrev();
		nextSetAdded.insert(prevStmt);
		nextStatement->setPrev(nextSetAdded);

		Statement* prevStatement = stmtTable->getStmtObj(prevStmt);
		unordered_set<int> prevSetAdded = prevStatement->getNext();
		prevSetAdded.insert(nextStmt);
		prevStatement->setNext(prevSetAdded);
	}
}

void CFGBuilder::addNextAndPrev(int next, int prev) {
	StmtTable* stmtTable = StmtTable::getInstance();

	Statement* previousStmt = stmtTable->getStmtObj(prev);
	unordered_set<int> prevNextSet = previousStmt->getNext();
	prevNextSet.insert(next);
	previousStmt->setNext(prevNextSet);

	Statement* nextStmt = stmtTable->getStmtObj(next);
	unordered_set<int> nextPrevSet = nextStmt->getPrev();
	nextPrevSet.insert(prev);
	nextStmt->setPrev(nextPrevSet);
}

void CFGBuilder::setRecoverChild(CNode* parent, CNode* child) {
	if(parent->getNodeType() == WHILE_) {
		parent->setSecondChild(child);
	} else {
		parent->setFirstChild(child);
	}
}

void CFGBuilder::setRecoverParent(CNode* parent, CNode* child) {
	if(child->getNodeType() == WHILE_) {
		child->setSecondParent(parent);
	} else {
		child->setFirstParent(parent);
	}
}

// for testing
string CFGBuilder::getCurrProc() {
	return this->currentProcedure->getName();
}

CNode* CFGBuilder::getHead() {
	return this->head;
}

int CFGBuilder::getStmtCounter() {
	return this->stmtCounter;
}

int CFGBuilder::getNestingLevel() {
	return this->currNestingLevel;
}

CFGBuilder::PrevState CFGBuilder::getPrevState() {
	return this->prevState;
}

stack<CNode*> CFGBuilder::getNestingStack() {
	return this->nestingStack;
}

stack<CNode*> CFGBuilder::getIfStack() {
	return this->ifStack;
}