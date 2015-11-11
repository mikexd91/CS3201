/*
 * CFGbipBuilderTest.cpp
 *
 *  Created on: 21 Oct, 2015
 *      Author: Leon
 */

#pragma once

#include "CFGbipBuilderTest.h"

#include <iostream>
#include <string>

using namespace std;

void CFGbipBuilderTest::setUp() {
	cfg = CFGbip::getInstance();
	builder = CFGbipBuilder::getInstance();
	pdr = PDR::getInstance();
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
	this->endProg = new ParsedData(ParsedData::END, 0);
}

CPPUNIT_TEST_SUITE_REGISTRATION( CFGbipBuilderTest );

void CFGbipBuilderTest::tearDown() {
	CFGbip::getInstance()->reset();
	PDR::resetInstanceFlag();
	CFGbipBuilder::resetInstanceFlag();
	AST::getInstance()->reset();
	VarTable::getInstance()->reset();
	ProcTable::getInstance()->clearTable();
	StmtTable::getInstance()->clearTable();
	ConstTable::getInstance()->clearTable();
}

void CFGbipBuilderTest::testProc() {
	ParsedData proc = ParsedData(ParsedData::PROCEDURE, 0);
	proc.setProcName("proc");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc);
	ProcGNode* procNode = (ProcGNode*) builder->getHead();

	parse(assign);
	parse(*endProg);
	EndGNode* endNode = (EndGNode*) builder->getHead();

	Procedure* procObj = procTable->getProcObj("proc");
	CPPUNIT_ASSERT(procObj->getGBipNodeRef() == procNode);
	CPPUNIT_ASSERT(procNode->getEndNode() == endNode);
	CPPUNIT_ASSERT(endNode->getProcNode() == procNode);

	CPPUNIT_ASSERT(cfg->getAllProcedures().size() == 1);
}

void CFGbipBuilderTest::testProc2() {
	/*
	procedure proc1 {
		x = 2;
	}

	procedure proc2 {
		x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();

	parse(assign);
	GNode* node1 = builder->getHead();

	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();

	parse(assign);
	GNode* node2 = builder->getHead();

	parse(*endProg);

	CPPUNIT_ASSERT(procNode1->getChild() == node1);
	CPPUNIT_ASSERT(procNode1->getEndNode() == node1->getChildren().at(0));
	CPPUNIT_ASSERT(procNode1->getParents().size() == 0);
	
	EndGNode* endNode1 = (EndGNode*) node1->getChildren().at(0);
	CPPUNIT_ASSERT(procNode1 == endNode1->getProcNode());
	CPPUNIT_ASSERT(endNode1->getChildren().size() == 0);

	CPPUNIT_ASSERT(procNode2->getChild() == node2);
	CPPUNIT_ASSERT(procNode2->getEndNode() == node2->getChildren().at(0));
	CPPUNIT_ASSERT(procNode2->getParents().size() == 0);

	EndGNode* endNode2 = (EndGNode*) node2->getChildren().at(0);
	CPPUNIT_ASSERT(endNode2->getProcNode() == procNode2);
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 0);

	Statement* stmt1 = stmtTable->getStmtObj(1);
	CPPUNIT_ASSERT(stmt1->getGBipNodeRef() == node1);

	Statement* stmt2 = stmtTable->getStmtObj(2);
	CPPUNIT_ASSERT(stmt2->getGBipNodeRef() == node2);

	Procedure* procedure1 = procTable->getProcObj("proc1");
	Procedure* procedure2 = procTable->getProcObj("proc2");
	CPPUNIT_ASSERT(procedure1->getGBipNodeRef() == procNode1);
	CPPUNIT_ASSERT(procedure2->getGBipNodeRef() == procNode2);
}

void CFGbipBuilderTest::testCall1Proc() {
	/*
	procedure proc1 {
		x = 2;
		call proc2;
	}

	procedure proc2 {
		x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();

	parse(assign);
	parse(call);
	CallGNode* node2 = (CallGNode*) builder->getHead();

	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();

	parse(assign);
	GNode* node3 = builder->getHead();
	
	parse(*endProg);

	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();

	CPPUNIT_ASSERT(node2->getChild() == procNode2);
	CPPUNIT_ASSERT(node2->getChildren().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node2));

	CPPUNIT_ASSERT(endNode1->getParent() == endNode2); 
	CPPUNIT_ASSERT(endNode1->getParents().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), endNode1));
}

void CFGbipBuilderTest::testCallAssign() {
	/*
	procedure proc1 {
		call proc2;
		x = 2;
	}

	procedure proc2 {
		x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");
	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(call);
	CallGNode* node1 = (CallGNode*) builder->getHead();
	parse(assign);
	GNode* node2 = builder->getHead();
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);
	GNode* node3 = builder->getHead();
	parse(*endProg);

	CPPUNIT_ASSERT(node1->getChild() == procNode2);
	CPPUNIT_ASSERT(node1->getChildren().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node1));

	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	CPPUNIT_ASSERT(node2->getParents().at(0) == endNode2);
	CPPUNIT_ASSERT(node2->getParents().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), node2));
	CPPUNIT_ASSERT(endNode2->getParent() == node3);

	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	CPPUNIT_ASSERT(node2->getChildren().at(0) == endNode1);
}

void CFGbipBuilderTest::testCallIfThen() {
	/*
	procedure proc1 {
	1	x = 2;
	2	if x then {
	3		call proc2;
		} else {
	4		x = 2;
		}
	5	x = 2;
	}

	procedure proc2 {
	6	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assign1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign1.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign1.setAssignExpression(q);

	ParsedData assign2 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assign2.setAssignVar("x");
	assign2.setAssignExpression(q);

	ParsedData ifData = ParsedData(ParsedData::IF, 1);
	ifData.setIfVar("x");
	ParsedData elseData = ParsedData(ParsedData::ELSE, 1);

	ParsedData call = ParsedData(ParsedData::CALL, 2);
	call.setProcName("proc2");
	
	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(assign1);
	GNode* node1 = builder->getHead();
	parse(ifData);
	IfGNode* node2 = (IfGNode*) builder->getHead();
	parse(call);
	CallGNode* node3 = (CallGNode*) builder->getHead();
	parse(elseData);
	parse(assign2);
	GNode* node4 = builder->getHead();
	parse(assign1);
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign1);
	GNode* node6 = builder->getHead();
	parse(*endProg);

	CPPUNIT_ASSERT(node3->getChildren().size() == 1);
	CPPUNIT_ASSERT(node3->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node3));

	DummyGNode* dumNode = (DummyGNode*) node2->getExit();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	CPPUNIT_ASSERT(dumNode->getParents().size() == 2);
	CPPUNIT_ASSERT(dumNode->getParents().at(0) == endNode2);
	CPPUNIT_ASSERT(dumNode->getParents().at(1) == node4);

	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), dumNode));
}

void CFGbipBuilderTest::testCallIfElse() {
	/*
	procedure proc1 {
	1	x = 2;
	2	if x then {
	3		x = 2;
		} else {
	4		call proc2;
		}
	5	x = 2;
	}

	procedure proc2 {
	6	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData assign1 = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign1.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign1.setAssignExpression(q);

	ParsedData assign2 = ParsedData(ParsedData::ASSIGNMENT, 2);
	assign2.setAssignVar("x");
	assign2.setAssignExpression(q);

	ParsedData ifData = ParsedData(ParsedData::IF, 1);
	ifData.setIfVar("x");
	ParsedData elseData = ParsedData(ParsedData::ELSE, 1);

	ParsedData call = ParsedData(ParsedData::CALL, 2);
	call.setProcName("proc2");

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(assign1);
	parse(ifData);
	IfGNode* node2 = (IfGNode*) builder->getHead();
	parse(assign2);
	GNode* node3 = builder->getHead();
	parse(elseData);
	parse(call);
	CallGNode* node4 = (CallGNode*) builder->getHead();
	parse(assign1);
	GNode* node5 = builder->getHead();
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign1);
	parse(*endProg);

	CPPUNIT_ASSERT(node4->getChildren().size() == 1);
	CPPUNIT_ASSERT(node4->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node4));
	CPPUNIT_ASSERT(node2->getChildren().at(0) == node3);
	CPPUNIT_ASSERT(node2->getChildren().at(1) == node4);

	DummyGNode* dumNode = (DummyGNode*) node2->getExit();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	CPPUNIT_ASSERT(dumNode->getParents().size() == 2);
	CPPUNIT_ASSERT(dumNode->getParents().at(0) == node3);
	CPPUNIT_ASSERT(dumNode->getParents().at(1) == endNode2);
	CPPUNIT_ASSERT(dumNode->getChildren().size() == 1);
	CPPUNIT_ASSERT(dumNode->getChildren().at(0) == node5);

	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), dumNode));
}

void CFGbipBuilderTest::testThenElseCalls() {
	/*
	procedure proc1 {
	1	x = 2;
	2	if x then {
	3		call proc2;
		} else {
	4		call proc3;
		}
	}

	procedure proc2 {
	5	x = 2;
	}

	procedure proc3 {
	6	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");
	ParsedData proc3 = ParsedData(ParsedData::PROCEDURE, 0);
	proc3.setProcName("proc3");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	ParsedData ifData = ParsedData(ParsedData::IF, 1);
	ifData.setIfVar("x");
	ParsedData elseData = ParsedData(ParsedData::ELSE, 1);

	ParsedData call1 = ParsedData(ParsedData::CALL, 2);
	call1.setProcName("proc2");
	ParsedData call2 = ParsedData(ParsedData::CALL, 2);
	call2.setProcName("proc3");

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(ifData);
	IfGNode* node2 = (IfGNode*) builder->getHead();
	parse(call1);
	CallGNode* node3 = (CallGNode*) builder->getHead();
	parse(elseData);
	parse(call2);
	CallGNode* node4 = (CallGNode*) builder->getHead();
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(proc3);
	ProcGNode* procNode3 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(*endProg);

	CPPUNIT_ASSERT(node3->getChildren().size() == 1);
	CPPUNIT_ASSERT(node3->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node3));

	CPPUNIT_ASSERT(node4->getChildren().size() == 1);
	CPPUNIT_ASSERT(node4->getChild() == procNode3);
	CPPUNIT_ASSERT(procNode3->getParents().size() == 1);
	CPPUNIT_ASSERT(contains(procNode3->getParents(), node4));

	DummyGNode* dumNode = (DummyGNode*) node2->getExit();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	EndGNode* endNode3 = (EndGNode*) procNode3->getEndNode();
	CPPUNIT_ASSERT(dumNode->getParents().size() == 2);
	CPPUNIT_ASSERT(dumNode->getParents().at(0) == endNode2);
	CPPUNIT_ASSERT(dumNode->getParents().at(1) == endNode3);
	CPPUNIT_ASSERT(dumNode->getChildren().at(0)->getNodeType() == END_);

	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().at(0) == dumNode);
	CPPUNIT_ASSERT(endNode3->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode3->getChildren().at(0) == dumNode);
}

void CFGbipBuilderTest::testCallWhile() {
	/*
	procedure proc1 {
	1	while x {
	2		call proc2;
		}
	}

	procedure proc2 {
	3	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData whileData = ParsedData(ParsedData::WHILE, 1);
	whileData.setWhileVar("x");
	
	ParsedData call = ParsedData(ParsedData::CALL, 2);
	call.setProcName("proc2");
	
	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(whileData);
	WhileGNode* node1 = (WhileGNode*) builder->getHead();
	parse(call);
	CallGNode* node2 = (CallGNode*) builder->getHead();
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);
	GNode* node3 = builder->getHead();
	parse(*endProg);

	CPPUNIT_ASSERT(node2->getChildren().size() == 1);
	CPPUNIT_ASSERT(node2->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().at(0) == node2);

	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	CPPUNIT_ASSERT(node1->getParents().size() == 2);
	CPPUNIT_ASSERT(node1->getParents().at(1) == endNode2);
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().at(0) == node1);

	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	CPPUNIT_ASSERT(node1->getAfterLoopChild() == endNode1);
}

void CFGbipBuilderTest::testIfWhile() {
	/*
	procedure proc1 {
	1	if x then {
	2		while x {
	3			call proc2;
			}
		} else {
	4		while x {
	5			call proc3;
			}
		}	
	}

	procedure proc2 {
	6	x = 2;
	}

	procedure proc3 {
	7	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");
	ParsedData proc3 = ParsedData(ParsedData::PROCEDURE, 0);
	proc3.setProcName("proc3");

	ParsedData ifData = ParsedData(ParsedData::IF, 1);
	ifData.setIfVar("x");
	ParsedData elseData = ParsedData(ParsedData::ELSE, 1);

	ParsedData whileData = ParsedData(ParsedData::WHILE, 2);
	whileData.setWhileVar("x");
	
	ParsedData call1 = ParsedData(ParsedData::CALL, 3);
	call1.setProcName("proc2");
	ParsedData call2 = ParsedData(ParsedData::CALL, 3);
	call2.setProcName("proc3");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(ifData);
	IfGNode* node1 = (IfGNode*) builder->getHead();
	parse(whileData);
	WhileGNode* node2 = (WhileGNode*) builder->getHead();
	parse(call1);
	CallGNode* node3 = (CallGNode*) builder->getHead();
	parse(elseData);
	parse(whileData);
	WhileGNode* node4 = (WhileGNode*) builder->getHead();
	parse(call2);
	CallGNode* node5 = (CallGNode*) builder->getHead();
	
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);

	parse(proc3);
	ProcGNode* procNode3 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(*endProg);

	CPPUNIT_ASSERT(node3->getChildren().size() == 1);
	CPPUNIT_ASSERT(node3->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().at(0) == node3);
	
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().at(0) == node2);
	CPPUNIT_ASSERT(node2->getParents().size() == 2);
	CPPUNIT_ASSERT(node2->getParents().at(1) == endNode2);
	
	CPPUNIT_ASSERT(node5->getChildren().size() == 1);
	CPPUNIT_ASSERT(node5->getChild() == procNode3);
	CPPUNIT_ASSERT(procNode3->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode3->getParents().at(0) == node5);

	EndGNode* endNode3 = (EndGNode*) procNode3->getEndNode();
	CPPUNIT_ASSERT(endNode3->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode3->getChildren().at(0) == node4);
	CPPUNIT_ASSERT(node4->getParents().size() == 2);
	CPPUNIT_ASSERT(node4->getParents().at(1) == endNode3);

	DummyGNode* dumNode = (DummyGNode*) node1->getExit();
	CPPUNIT_ASSERT(node2->getAfterLoopChild() == dumNode);
	CPPUNIT_ASSERT(node4->getAfterLoopChild() == dumNode);
}

void CFGbipBuilderTest::testWhileIf() {
	/*
	procedure proc1 {
	1	while x {
	2		if x then {
	3			call proc2;
			} else {
	4			call proc3;
			}
		}
	}

	procedure proc2 {
	5	x = 2;
	}

	procedure proc3 {
	6	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");
	ParsedData proc3 = ParsedData(ParsedData::PROCEDURE, 0);
	proc3.setProcName("proc3");

	ParsedData whileData = ParsedData(ParsedData::WHILE, 1);
	whileData.setWhileVar("x");

	ParsedData ifData = ParsedData(ParsedData::IF, 2);
	ifData.setIfVar("x");
	ParsedData elseData = ParsedData(ParsedData::ELSE, 2);

	ParsedData call1 = ParsedData(ParsedData::CALL, 3);
	call1.setProcName("proc2");
	ParsedData call2 = ParsedData(ParsedData::CALL, 3);
	call2.setProcName("proc3");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(whileData);
	WhileGNode* node1 = (WhileGNode*) builder->getHead();
	parse(ifData);
	IfGNode* node2 = (IfGNode*) builder->getHead();
	parse(call1);
	CallGNode* node3 = (CallGNode*) builder->getHead();
	parse(elseData);
	parse(call2);
	CallGNode* node4 = (CallGNode*) builder->getHead();
	
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);
	
	parse(proc3);
	ProcGNode* procNode3 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(*endProg);

	CPPUNIT_ASSERT(node3->getChildren().size() == 1);
	CPPUNIT_ASSERT(node3->getChild() == procNode2);
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().at(0) == node3);

	CPPUNIT_ASSERT(node4->getChildren().size() == 1);
	CPPUNIT_ASSERT(node4->getChild() == procNode3);
	CPPUNIT_ASSERT(procNode3->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode3->getParents().at(0) == node4);
	
	DummyGNode* dumNode = (DummyGNode*) node2->getExit();
	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	EndGNode* endNode3 = (EndGNode*) procNode3->getEndNode();
	CPPUNIT_ASSERT(dumNode->getParents().size() == 2);
	CPPUNIT_ASSERT(dumNode->getParents().at(0) == endNode2);
	CPPUNIT_ASSERT(dumNode->getParents().at(1) == endNode3);

	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().at(0) == dumNode);
	CPPUNIT_ASSERT(endNode3->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode3->getChildren().at(0) == dumNode);
}

void CFGbipBuilderTest::testCallMulti() {
	/*
	procedure proc1 {
	1	call proc2;
	2	x = 2;
	3	call proc2;
	}

	procedure proc2 {
	4	x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");

	ParsedData call = ParsedData(ParsedData::CALL, 1);
	call.setProcName("proc2");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(call);
	CallGNode* node1 = (CallGNode*) builder->getHead();
	parse(assign);
	GNode* node2 = builder->getHead();
	parse(call);
	CallGNode* node3 = (CallGNode*) builder->getHead();

	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(*endProg);

	CPPUNIT_ASSERT(node1->getChildren().size() == 1);
	CPPUNIT_ASSERT(node1->getChild() == procNode2);
	CPPUNIT_ASSERT(node3->getChildren().size() == 1);
	CPPUNIT_ASSERT(node3->getChild() == procNode2);
	
	CPPUNIT_ASSERT(procNode2->getParents().size() == 2);
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node1));
	CPPUNIT_ASSERT(contains(procNode2->getParents(), node3));

	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	CPPUNIT_ASSERT(endNode2->getChildren().size() == 2);
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), node2));
	CPPUNIT_ASSERT(contains(endNode2->getChildren(), endNode1));

	CPPUNIT_ASSERT(node2->getParents().size() == 1);
	CPPUNIT_ASSERT(node2->getParents().at(0) == endNode2);
	CPPUNIT_ASSERT(endNode1->getParents().size() == 1);
	CPPUNIT_ASSERT(endNode1->getParent() == endNode2);

	CPPUNIT_ASSERT(getPosition(procNode2->getParents(), node1) == getPosition(endNode2->getChildren(), node2));
	CPPUNIT_ASSERT(getPosition(procNode2->getParents(), node3) == getPosition(endNode2->getChildren(), endNode1));

	Statement* stmt4 = stmtTable->getStmtObj(4);
}

void CFGbipBuilderTest::testNestedCall() {
	/*
	procedure proc1 {
		call proc2;
	}

	procedure proc2 {
		call proc3;
	}

	procedure proc3 {
		x = 2;
	}
	*/

	ParsedData proc1 = ParsedData(ParsedData::PROCEDURE, 0);
	proc1.setProcName("proc1");
	ParsedData proc2 = ParsedData(ParsedData::PROCEDURE, 0);
	proc2.setProcName("proc2");
	ParsedData proc3 = ParsedData(ParsedData::PROCEDURE, 0);
	proc3.setProcName("proc3");

	ParsedData call1 = ParsedData(ParsedData::CALL, 1);
	call1.setProcName("proc2");
	ParsedData call2 = ParsedData(ParsedData::CALL, 1);
	call2.setProcName("proc3");

	ParsedData assign = ParsedData(ParsedData::ASSIGNMENT, 1);
	assign.setAssignVar("x");
	queue<string> q;
	q.push("2");
	assign.setAssignExpression(q);

	parse(proc1);
	ProcGNode* procNode1 = (ProcGNode*) builder->getHead();
	parse(call1);
	CallGNode* node1 = (CallGNode*) builder->getHead();
	
	parse(proc2);
	ProcGNode* procNode2 = (ProcGNode*) builder->getHead();
	parse(call2);
	CallGNode* node2 = (CallGNode*) builder->getHead();

	parse(proc3);
	ProcGNode* procNode3 = (ProcGNode*) builder->getHead();
	parse(assign);
	parse(*endProg);

	EndGNode* endNode1 = (EndGNode*) procNode1->getEndNode();
	EndGNode* endNode2 = (EndGNode*) procNode2->getEndNode();
	EndGNode* endNode3 = (EndGNode*) procNode3->getEndNode();

	CPPUNIT_ASSERT(node1->getChildren().size() == 1);
	CPPUNIT_ASSERT(node1->getChild() == procNode2);	
	CPPUNIT_ASSERT(procNode2->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode2->getParents().at(0) == node1);

	CPPUNIT_ASSERT(node2->getChildren().size() == 1);
	CPPUNIT_ASSERT(node2->getChild() == procNode3);
	CPPUNIT_ASSERT(procNode3->getParents().size() == 1);
	CPPUNIT_ASSERT(procNode3->getParents().at(0) == node2);

	CPPUNIT_ASSERT(endNode3->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode3->getChildren().at(0) == endNode2);

	CPPUNIT_ASSERT(endNode2->getChildren().size() == 1);
	CPPUNIT_ASSERT(endNode2->getChildren().at(0) == endNode1);
}

void CFGbipBuilderTest::parse(ParsedData data) {
	pdr->processParsedData(data);
	builder->processParsedData(data);
}

bool CFGbipBuilderTest::contains(vector<GNode*> arr, GNode* node) {
	if(find(arr.begin(), arr.end(), node) != arr.end()) {
		return true;
	}

	return false;
}

int CFGbipBuilderTest::getPosition(vector<GNode*> arr, GNode* item) {
	int pos = find(arr.begin(), arr.end(), item) - arr.begin();

	return pos;
}