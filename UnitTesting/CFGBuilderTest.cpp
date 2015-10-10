#include "CFGBuilderTest.h"

#include <iostream>
#include <string>

using namespace std;

CFG* cfg;
CFGBuilder* builder;
StmtTable* stmtTable;
PDR* pdr1;
ParsedData endProg = ParsedData(ParsedData::END, 0);

void CFGBuilderTest::setUp() {
	cfg = CFG::getInstance();
	builder = CFGBuilder::getInstance();
	pdr1 = PDR::getInstance();
	stmtTable = StmtTable::getInstance();
}

CPPUNIT_TEST_SUITE_REGISTRATION( CFGBuilderTest );

void CFGBuilderTest::tearDown() {
	CFG::getInstance()->reset();
	PDR::resetInstanceFlag();
	CFGBuilder::resetInstanceFlag();
	AST::getInstance()->reset();
	VarTable::getInstance()->reset();
	ProcTable::getInstance()->clearTable();
	StmtTable::getInstance()->clearTable();
	ConstTable::getInstance()->clearTable();
}

void CFGBuilderTest::testProc() {
	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");
	
	parse(proc);

	CPPUNIT_ASSERT(builder->getHead()->getNodeType() == PROCEDURE_);
	CPPUNIT_ASSERT(builder->getNestingLevel() == 1);
	CPPUNIT_ASSERT(builder->getCurrProc() == "proc");
	CPPUNIT_ASSERT(builder->getPrevState() == CFGBuilder::PROCEDURE);
	CPPUNIT_ASSERT(cfg->getAllProcedures().size() == 0);
}

void CFGBuilderTest::testWithIf() {
	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");
	ParsedData ifStmt = ParsedData(ParsedData::IF, 1);
	ifStmt.setIfVar("x");

	parse(proc);
	GNode* procNode = builder->getHead();

	parse(ifStmt);
	GNode* ifNode = builder->getHead();

	vector<GNode*> procNodeChildrens = procNode->getChildren();
	vector<GNode*> ifNodeParents = ifNode->getParents();

	CPPUNIT_ASSERT(procNodeChildrens[0] == ifNode);
	CPPUNIT_ASSERT(ifNodeParents[0] == procNode);

	CPPUNIT_ASSERT(builder->getPrevState() == CFGBuilder::IF);
	CPPUNIT_ASSERT(builder->getHead()->getNodeType() == IF_);
	CPPUNIT_ASSERT(builder->getNestingLevel() == 2);
	CPPUNIT_ASSERT(builder->getStmtCounter() == 1);
}

void CFGBuilderTest::testIfAssg() {

	//procedure proc {
	//	if x then {
	//		x = 2;
	//	} else {
	//		y = 3;
	//	}
	//}

	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData ifStmt = ParsedData(ParsedData::IF, 1);
	ifStmt.setIfVar("x");

	ParsedData assgStmt1 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assgStmt1.setAssignVar("x");
	queue<string> assgExp1;
	assgExp1.push("2");
	assgStmt1.setAssignExpression(assgExp1);

	ParsedData elseStmt = ParsedData(ParsedData::ELSE, 1);

	ParsedData assgStmt2 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assgStmt2.setAssignVar("y");
	queue<string> assgExp2;
	assgExp2.push("3");
	assgStmt2.setAssignExpression(assgExp2);

	parse(proc);
	ProcGNode* procNode = (ProcGNode*) builder->getHead();

	parse(ifStmt);
	GNode* ifNode = builder->getHead();

	parse(assgStmt1);
	GNode* assgNode1 = builder->getHead();

	parse(elseStmt);
	CPPUNIT_ASSERT(builder->getHead() == ifNode);

	parse(assgStmt2);
	GNode* assgNode2 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(cfg->getAllProcedures().size() == 1);
	CPPUNIT_ASSERT(cfg->getAllProcedures().at(0) == procNode);

	CPPUNIT_ASSERT(ifNode->getChildren().at(0) == assgNode1);
	CPPUNIT_ASSERT(ifNode->getChildren().at(1) == assgNode2);
	CPPUNIT_ASSERT(assgNode1->getParents().at(0) == ifNode);
	CPPUNIT_ASSERT(assgNode2->getParents().at(0) == ifNode);
	CPPUNIT_ASSERT(assgNode1->getChildren().at(0)->getNodeType() == END_);
	CPPUNIT_ASSERT(assgNode2->getChildren().at(0)->getNodeType() == END_);

	CPPUNIT_ASSERT(ifNode->getStartStmt() == ifNode->getEndStmt() && ifNode->getStartStmt() == 1);
	CPPUNIT_ASSERT(assgNode1->getStartStmt() == assgNode1->getEndStmt() && assgNode1->getStartStmt() == 2);
	CPPUNIT_ASSERT(assgNode2->getStartStmt() == assgNode2->getEndStmt() && assgNode2->getStartStmt() == 3);
}

void CFGBuilderTest::testMultAssg() {
	/**
	procedure proc {
		x = 3;
		y = 4;
		z = x + y;
	}
	*/
	
	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("3");
	assg1.setAssignExpression(queue1);

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg2.setAssignVar("y");
	queue<string> queue2;
	queue2.push("4");
	assg2.setAssignExpression(queue2);

	ParsedData assg3 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg3.setAssignVar("z");
	queue<string> queue3;
	queue3.push("x");
	queue3.push("y");
	queue3.push("+");
	assg3.setAssignExpression(queue3);

	parse(proc);
	GNode* procNode = builder->getHead();

	parse(assg1);
	parse(assg2);
	parse(assg3);
	GNode* assgNode = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(procNode->getChildren().at(0) == assgNode);
	CPPUNIT_ASSERT(assgNode->getParents().at(0) == procNode);
	CPPUNIT_ASSERT(assgNode->getStartStmt() == 1);
	CPPUNIT_ASSERT(assgNode->getEndStmt() == 3);
	CPPUNIT_ASSERT(assgNode->getChildren().at(0)->getNodeType() == END_);
}

void CFGBuilderTest::testMultProc() {
	/*
	procedure proc1 {
		x = 2;
	}

	procedure proc2 {
		y = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");

	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("3");
	assg1.setAssignExpression(queue1);

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg2.setAssignVar("y");
	queue<string> queue2;
	queue2.push("4");
	assg2.setAssignExpression(queue2);

	parse(proc1);
	GNode* procNode1 = builder->getHead();

	parse(assg1);
	GNode* assgNode1 = builder->getHead();

	parse(proc2);
	GNode* procNode2 = builder->getHead();

	parse(assg2);
	GNode* assgNode2 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(cfg->getAllProcedures().size() == 2);
	
	CPPUNIT_ASSERT(procNode1->getChildren().at(0) == assgNode1);
	CPPUNIT_ASSERT(assgNode1->getParents().at(0) == procNode1);
	CPPUNIT_ASSERT(assgNode1->getStartStmt() == assgNode1->getEndStmt() && assgNode1->getEndStmt() == 1);
	CPPUNIT_ASSERT(assgNode1->getChildren().at(0)->getNodeType() == END_);
	
	CPPUNIT_ASSERT(procNode2->getChildren().at(0) == assgNode2);
	CPPUNIT_ASSERT(assgNode2->getParents().at(0) == procNode2);
	CPPUNIT_ASSERT(assgNode2->getStartStmt() == assgNode2->getEndStmt() && assgNode2->getEndStmt() == 2);
	CPPUNIT_ASSERT(assgNode2->getChildren().at(0)->getNodeType() == END_);
}

void CFGBuilderTest::testMultAssgCall() {
	/*
	procedure proc1 {
		x = 3;
		y = 4;
		call proc2;
	}

	procedure proc2 {
		x = 3;
		y = 4;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");

	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("3");
	assg1.setAssignExpression(queue1);

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg2.setAssignVar("y");
	queue<string> queue2;
	queue2.push("4");
	assg2.setAssignExpression(queue2);

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");

	parse(proc1);
	GNode* procNode1 = builder->getHead();

	parse(assg1);
	parse(assg2);
	GNode* block1 = builder->getHead();

	parse(call);
	GNode* callNode = builder->getHead();

	parse(proc2);
	GNode* procNode2 = builder->getHead();

	parse(assg1);
	parse(assg2);
	GNode* block2 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(procNode1->getChildren().at(0) == block1);
	CPPUNIT_ASSERT(block1->getParents().at(0) == procNode1);
	CPPUNIT_ASSERT(block1->getChildren().at(0)->getNodeType() == CALL_);
	CPPUNIT_ASSERT(block1->getStartStmt() == 1 && block1->getEndStmt() == 2);

	CPPUNIT_ASSERT(callNode->getChildren().at(0)->getNodeType() == END_);
	CPPUNIT_ASSERT(callNode->getParents().at(0) == block1);
	CPPUNIT_ASSERT(callNode->getEndStmt() == 3 && callNode->getStartStmt() == 3);

	CPPUNIT_ASSERT(procNode2->getChildren().at(0) == block2);
	CPPUNIT_ASSERT(block2->getParents().at(0) == procNode2);
	CPPUNIT_ASSERT(block2->getChildren().at(0)->getNodeType() == END_);
	CPPUNIT_ASSERT(block2->getStartStmt() == 4 && block2->getEndStmt() == 5);
}

void CFGBuilderTest::testWithWhile() {
	/*
	procedure proc {
		while x {
			x = 3;
		}
	}
	*/

	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData whileStmt = ParsedData(ParsedData::WHILE, 1);
	whileStmt.setWhileVar("x");

	ParsedData assg = ParsedData(ParsedData::ASSIGNMENT, 2);
	assg.setAssignVar("x");
	queue<string> queue;
	queue.push("3");
	assg.setAssignExpression(queue);

	parse(proc);
	GNode* procNode = builder->getHead();
	
	parse(whileStmt);
	GNode* whileNode = builder->getHead();

	parse(assg);
	GNode* assgNode = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(whileNode->getChildren().at(0) == assgNode);
	CPPUNIT_ASSERT(whileNode->getChildren().at(1)->getNodeType() == END_);
	CPPUNIT_ASSERT(whileNode->getParents().at(0) == procNode);
	CPPUNIT_ASSERT(whileNode->getParents().at(1) == assgNode);
	CPPUNIT_ASSERT(assgNode->getChildren().at(0) == whileNode);
	CPPUNIT_ASSERT(assgNode->getParents().at(0) == whileNode);
}

void CFGBuilderTest::testIfNested() {
	/*
	procedure proc {
		x = 1;
		if x then {
			if y then {
				x = 1;
			} else {
				y = 2;
			}
		} else {
			y = 2;
		}
	}
	*/

	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("1");
	assg1.setAssignExpression(queue1);

	ParsedData ifStmt1 = ParsedData(ParsedData::IF, 1);
	ifStmt1.setIfVar("x");

	ParsedData ifStmt2 = ParsedData(ParsedData::IF, 2);
	ifStmt2.setIfVar("y");

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 3);
	assg2.setAssignVar("x");
	queue<string> queue2;
	queue2.push("1");
	assg2.setAssignExpression(queue2);

	ParsedData elseStmt1 = ParsedData(ParsedData::ELSE, 2);

	ParsedData assg3 = ParsedData(ParsedData::ASSIGNMENT, 3);
	assg3.setAssignVar("y");
	queue<string> queue3;
	queue3.push("2");
	assg3.setAssignExpression(queue3);

	ParsedData elseStmt2 = ParsedData(ParsedData::ELSE, 1);

	ParsedData assg4 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assg4.setAssignVar("y");
	queue<string> queue4;
	queue4.push("2");
	assg4.setAssignExpression(queue4);

	parse(proc);

	parse(assg1);
	GNode* assgNode1 = builder->getHead();

	parse(ifStmt1);
	GNode* ifNode1 = builder->getHead();

	parse(ifStmt2);
	GNode* ifNode2 = builder->getHead();

	parse(assg2);
	GNode* assgNode2 = builder->getHead();

	parse(elseStmt1);
	parse(assg3);
	GNode* assgNode3 = builder->getHead();

	parse(elseStmt2);
	parse(assg4);
	GNode* assgNode4 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(assgNode1->getChildren().at(0) == ifNode1);
	CPPUNIT_ASSERT(assgNode1->getEndStmt() == assgNode1->getStartStmt() && assgNode1->getEndStmt() == 1);

	CPPUNIT_ASSERT(ifNode1->getParents().at(0) == assgNode1);
	CPPUNIT_ASSERT(ifNode1->getStartStmt() == ifNode1->getEndStmt() && ifNode1->getEndStmt() == 2);
	CPPUNIT_ASSERT(ifNode1->getChildren().at(0) == ifNode2);
	CPPUNIT_ASSERT(ifNode1->getChildren().at(1) == assgNode4);
	
	CPPUNIT_ASSERT(ifNode2->getParents().at(0) == ifNode1);
	CPPUNIT_ASSERT(ifNode2->getChildren().at(0) == assgNode2);
	CPPUNIT_ASSERT(ifNode2->getChildren().at(1) == assgNode3);
	CPPUNIT_ASSERT(ifNode2->getEndStmt() == ifNode2->getStartStmt() && ifNode2->getStartStmt() == 3);

	CPPUNIT_ASSERT(assgNode2->getParents().at(0) == ifNode2);
	CPPUNIT_ASSERT(assgNode2->getChildren().at(0)->getNodeType() == DUMMY_);
	CPPUNIT_ASSERT(assgNode2->getStartStmt() == assgNode2->getEndStmt() && assgNode2->getEndStmt() == 4);

	CPPUNIT_ASSERT(assgNode3->getParents().at(0) == ifNode2);
	CPPUNIT_ASSERT(assgNode3->getChildren().at(0)->getNodeType() == DUMMY_);
	CPPUNIT_ASSERT(assgNode3->getChildren().at(0) == assgNode2->getChildren().at(0));
	CPPUNIT_ASSERT(assgNode3->getStartStmt() == assgNode3->getEndStmt() && assgNode3->getEndStmt() == 5);

	CPPUNIT_ASSERT(assgNode4->getParents().at(0) == ifNode1);
	CPPUNIT_ASSERT(assgNode4->getChildren().at(0)->getNodeType() == END_);
	CPPUNIT_ASSERT(assgNode4->getStartStmt() == assgNode4->getEndStmt() && assgNode4->getEndStmt() == 6);
	CPPUNIT_ASSERT(assgNode4->getChildren().at(0) == assgNode3->getChildren().at(0)->getChildren().at(0));
}

void CFGBuilderTest::testWhileNested() {
	/*
	procedure proc {
		x = 1;
		while x {
			while y {
				y = 3;
			}
		}
	}
	*/

	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("1");
	assg1.setAssignExpression(queue1);

	ParsedData while1 = ParsedData(ParsedData::WHILE, 1);
	while1.setWhileVar("x");

	ParsedData while2 = ParsedData(ParsedData::WHILE, 2);
	while2.setWhileVar("y");

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 3);
	assg2.setAssignVar("y");
	queue<string> queue2;
	queue2.push("3");
	assg2.setAssignExpression(queue2);

	parse(proc);
	parse(assg1);
	GNode* assgNode1 = builder->getHead();

	parse(while1);
	GNode* whileNode1 = builder->getHead();

	parse(while2);
	GNode* whileNode2 = builder->getHead();

	parse(assg2);
	GNode* assgNode2 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(whileNode1->getParents().at(0) == assgNode1);
	CPPUNIT_ASSERT(whileNode1->getParents().at(1) == whileNode2);
	CPPUNIT_ASSERT(whileNode1->getChildren().at(0) == whileNode2);
	CPPUNIT_ASSERT(whileNode1->getChildren().at(1)->getNodeType() == END_);
	CPPUNIT_ASSERT(whileNode1->getEndStmt() == whileNode1->getStartStmt() && whileNode1->getEndStmt() == 2);

	CPPUNIT_ASSERT(whileNode2->getParents().at(0) == whileNode1);
	CPPUNIT_ASSERT(whileNode2->getParents().at(1) == assgNode2);
	CPPUNIT_ASSERT(whileNode2->getChildren().at(0) == assgNode2);
	CPPUNIT_ASSERT(whileNode2->getChildren().at(1) == whileNode1);
	CPPUNIT_ASSERT(whileNode2->getEndStmt() == whileNode2->getStartStmt() && whileNode2->getEndStmt() == 3);

	CPPUNIT_ASSERT(assgNode2->getParents().at(0) == whileNode2);
	CPPUNIT_ASSERT(assgNode2->getChildren().at(0) == whileNode2);
	CPPUNIT_ASSERT(assgNode2->getStartStmt() == assgNode2->getEndStmt() && assgNode2->getEndStmt() == 4);
}

void CFGBuilderTest::testIfWhile() {
	/*
	procedure proc {
		x = 1;
		if x then {
			while x {
				y = 2;
			}
		} else {
			y = 3;
		}
	}
	*/

	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData assg1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assg1.setAssignVar("x");
	queue<string> queue1;
	queue1.push("1");
	assg1.setAssignExpression(queue1);

	ParsedData ifStmt = ParsedData(ParsedData::IF, 1);
	ifStmt.setIfVar("x");

	ParsedData whileStmt = ParsedData(ParsedData::WHILE, 2);
	whileStmt.setWhileVar("x");

	ParsedData assg2 = ParsedData(ParsedData::ASSIGNMENT, 3);
	assg2.setAssignVar("y");
	queue<string> queue2;
	queue2.push("2");
	assg2.setAssignExpression(queue2);

	ParsedData elseStmt = ParsedData(ParsedData::ELSE, 1);
	
	ParsedData assg3 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assg3.setAssignVar("y");
	queue<string> queue3;
	queue3.push("3");
	assg3.setAssignExpression(queue3);

	parse(proc);
	parse(assg1);
	parse(ifStmt);
	GNode* ifNode = builder->getHead();

	parse(whileStmt);
	GNode* whileNode = builder->getHead();

	parse(assg2);
	GNode* assgNode2 = builder->getHead();

	parse(elseStmt);
	parse(assg3);
	GNode* assgNode3 = builder->getHead();

	parse(endProg);

	CPPUNIT_ASSERT(ifNode->getChildren().at(0) == whileNode);
	CPPUNIT_ASSERT(ifNode->getChildren().at(1) == assgNode3);
	CPPUNIT_ASSERT(ifNode->getChildren().size() == 2);

	CPPUNIT_ASSERT(whileNode->getChildren().at(0) == assgNode2);
	CPPUNIT_ASSERT(whileNode->getChildren().at(1)->getNodeType() == END_);
	CPPUNIT_ASSERT(whileNode->getParents().at(0) == ifNode);
	CPPUNIT_ASSERT(whileNode->getParents().at(1) == assgNode2);

	CPPUNIT_ASSERT(assgNode2->getChildren().at(0) == whileNode);
	CPPUNIT_ASSERT(assgNode2->getParents().at(0) == whileNode);

	CPPUNIT_ASSERT(assgNode3->getParents().at(0) == ifNode);
	CPPUNIT_ASSERT(assgNode3->getChildren().at(0) == whileNode->getChildren().at(1));
}

void CFGBuilderTest::parse(ParsedData data) {
	pdr1->processParsedData(data);
	builder->processParsedData(data);
}