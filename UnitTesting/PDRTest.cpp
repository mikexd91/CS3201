#include <cppunit/config/SourcePrefix.h>
#include "PDRTest.h"

#include <iostream>
#include <string>

using namespace std;

	PDR* pdr;

void PDRTest::setUp() {
	pdr = PDR::getInstance();
}

void PDRTest::tearDown() {
	PDR::resetInstanceFlag();
	AST::getInstance()->reset();
	VarTable::getInstance()->reset();
	ProcTable::getInstance()->clearTable();
	StmtTable::getInstance()->clearTable();
	ConstTable::getInstance()->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( PDRTest );

void PDRTest::testProcessProc() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");

	pdr->processParsedData(procedure);
	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 0);

	stack<TNode*> nodeStack = pdr->getNodeStack();

	CPPUNIT_ASSERT(nodeStack.size() == 2);
	CPPUNIT_ASSERT(nodeStack.top()->getNodeType() == STMTLST_);
	
	TNode* procStmtLst = nodeStack.top();
	nodeStack.pop();
	CPPUNIT_ASSERT(nodeStack.top()->getNodeType() == PROCEDURE_);
	CPPUNIT_ASSERT(nodeStack.top()->getName() == "proc");
	CPPUNIT_ASSERT(procStmtLst->getParent() == nodeStack.top());
}

void PDRTest::testProcessAssign() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");
	pdr->processParsedData(procedure);

	ParsedData assign1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign1.setAssignVar("x");
	queue<string> assign1Exp;
	assign1Exp.push("2");
	assign1.setAssignExpression(assign1Exp);
	pdr->processParsedData(assign1);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 1);

	ParsedData assign2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign2.setAssignVar("y");
	queue<string> assign2Exp;
	assign2Exp.push("3");
	assign2.setAssignExpression(assign2Exp);
	pdr->processParsedData(assign2);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 2);
}

void PDRTest::testProcessWhile() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");
	pdr->processParsedData(procedure);

	ParsedData while1 = ParsedData(ParsedData::WHILE, 1);
	while1.setWhileVar("x");
	pdr->processParsedData(while1);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 2);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 1);
	
	ParsedData while2 = ParsedData(ParsedData::WHILE, 1);
	while2.setWhileVar("y");
	pdr->processParsedData(while2);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 2);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 2);

	ParsedData while3 = ParsedData(ParsedData::WHILE, 2);
	while3.setWhileVar("z");
	pdr->processParsedData(while3);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 3);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 3);
}

void PDRTest::testProcessCallStmts1() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");
	pdr->processParsedData(procedure);

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");
	pdr->processParsedData(call);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 1);
}

void PDRTest::testMultipleProcs1() {
	ParsedData procedure1 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure1.setProcName("proc1");
	pdr->processParsedData(procedure1);

	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getProcName() == "proc1");
	
	ParsedData procedure2 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure2.setProcName("proc2");
	pdr->processParsedData(procedure2);

	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getProcName() == "proc2");
}

void PDRTest::testMultipleProcs2() {
	ParsedData procedure1 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure1.setProcName("proc1");
	pdr->processParsedData(procedure1);

	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getProcName() == "proc1");
	
	ParsedData calledProc1 = ParsedData(ParsedData::CALL, 1);
	calledProc1.setProcName("calledProc1");
	pdr->processParsedData(calledProc1);

	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getProcName() == "proc1");

	ParsedData procedure2 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure2.setProcName("proc2");
	pdr->processParsedData(procedure2);

	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getProcName() == "proc2");
}

void PDRTest::testNestingLevel() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");
	pdr->processParsedData(procedure);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);

	ParsedData whileStmt = ParsedData(ParsedData::WHILE, 1);
	whileStmt.setWhileVar("x");
	pdr->processParsedData(whileStmt);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 2);

	ParsedData callStmt = ParsedData(ParsedData::CALL, 1);
	callStmt.setProcName("proc1");
	pdr->processParsedData(callStmt);

	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 1);
}

void PDRTest::testProcUsesAndModifies1() {
	ParsedData procedure1 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure1.setProcName("proc1");
	pdr->processParsedData(procedure1);

	ParsedData assignStmt1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assignStmt1.setAssignVar("x");
	queue<string> expressionQueue1;
	expressionQueue1.push("2");
	assignStmt1.setAssignExpression(expressionQueue1);
	pdr->processParsedData(assignStmt1);

	unordered_set<string> modifiesSet1;
	modifiesSet1.insert("x");
	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getModifies() == modifiesSet1);
	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getUses().empty());

	ParsedData assignStmt2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assignStmt2.setAssignVar("y");
	queue<string> expressionQueue2;
	expressionQueue2.push("x");
	assignStmt2.setAssignExpression(expressionQueue2);
	pdr->processParsedData(assignStmt2);

	string modifies2[] = {"x", "y"};
	unordered_set<string> modifiesSet2(modifies2, modifies2 + 2);
	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getModifies() == modifiesSet2);

	string uses2[] = {"x"};
	unordered_set<string> usesSet2(uses2, uses2 + 1);
	CPPUNIT_ASSERT(pdr->getCurrentProcedure()->getUses() == usesSet2);
}

// for testing modifies and uses from calls
void PDRTest::testProcUsesAndModifiesCalledBy() {
	ParsedData procedure1 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure1.setProcName("proc1");
	pdr->processParsedData(procedure1);

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");
	pdr->processParsedData(call);

	ParsedData procedure2 = ParsedData(ParsedData::PROCEDURE, 0);
	procedure2.setProcName("proc2");
	pdr->processParsedData(procedure2);

	ParsedData assign1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign1.setAssignVar("x");
	queue<string> expQueue1;
	expQueue1.push("2");
	assign1.setAssignExpression(expQueue1);
	pdr->processParsedData(assign1);
	
	ParsedData assign2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign2.setAssignVar("y");
	queue<string> expQueue2;
	expQueue2.push("x");
	assign2.setAssignExpression(expQueue2);
	pdr->processParsedData(assign2);

	unordered_set<Procedure*> calledBy = pdr->getCurrentProcedure()->getCalledBy();
	unordered_set<Procedure*>::iterator iter;
	string modifies[] = {"x", "y"};
	string uses[] = {"x"};
	unordered_set<string> modifiesSet(modifies, modifies + 2);
	unordered_set<string> usesSet(uses, uses + 1);
	for(iter = calledBy.begin(); iter != calledBy.end(); iter++) {
		Procedure* calledByProc = *iter;
		CPPUNIT_ASSERT(calledByProc->getModifies() == modifiesSet);
		CPPUNIT_ASSERT(calledByProc->getUses() == usesSet);
	}
}

void PDRTest::testProcessIf() {
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, 0);
	procedure.setProcName("proc");
	pdr->processParsedData(procedure);

	ParsedData assign1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign1.setAssignVar("x");
	queue<string> expressionQueue;
	expressionQueue.push("2");
	assign1.setAssignExpression(expressionQueue);
	pdr->processParsedData(assign1);

	ParsedData ifStmt = ParsedData(ParsedData::IF, 1);
	ifStmt.setIfVar("x");
	pdr->processParsedData(ifStmt);
	IfNode* parentIfNode = (IfNode*)pdr->getNodeStack().top()->getParent();
	CPPUNIT_ASSERT(parentIfNode->getNodeType() == IF_STMT_);
	CPPUNIT_ASSERT(parentIfNode->getLeftSibling()->getNodeType() == ASSIGN_STMT_);
	CPPUNIT_ASSERT(parentIfNode->getVarNode()->getName() == "x");

	ParsedData assign2 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assign2.setAssignVar("y");
	assign2.setAssignExpression(expressionQueue);
	pdr->processParsedData(assign2);

	ParsedData elseStmt = ParsedData(ParsedData::ELSE, 1);
	pdr->processParsedData(elseStmt);
	CPPUNIT_ASSERT(pdr->getCurrStmtNumber() == 3);
	CPPUNIT_ASSERT(pdr->getParentNumStack().top() == 2);

	ParsedData assign3 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assign3.setAssignVar("z");
	assign3.setAssignExpression(expressionQueue);
	pdr->processParsedData(assign3);

	CPPUNIT_ASSERT(parentIfNode->getElseStmtLstNode()->getChildren().size() == 1);
	
	vector<TNode*> children = parentIfNode->getElseStmtLstNode()->getChildren();
	TNode* assignChild = children[0];
	CPPUNIT_ASSERT(assignChild->getNodeType() == ASSIGN_STMT_);
	CPPUNIT_ASSERT(pdr->getCurrNestingLevel() == 2);
}