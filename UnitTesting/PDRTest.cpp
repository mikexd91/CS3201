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
	CPPUNIT_ASSERT(nodeStack.top()->getNodeType() == NodeType::STMTLST_);
	
	TNode* procStmtLst = nodeStack.top();
	nodeStack.pop();
	CPPUNIT_ASSERT(nodeStack.top()->getNodeType() == NodeType::PROCEDURE_);
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
}