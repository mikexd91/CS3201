#include <cppunit/config/SourcePrefix.h>
#include "pdr_integration_test.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"

using namespace std;

Parser parser1;
PDR* pdr1;
AST* ast1;

void PDR_Integration_Test::setUp() {
	pdr1 = PDR::getInstance();
	parser1 = Parser();
	ast1 = AST::getInstance();
}

void PDR_Integration_Test::tearDown() {
	PDR::resetInstanceFlag();
}

CPPUNIT_TEST_SUITE_REGISTRATION( PDR_Integration_Test );

void PDR_Integration_Test::testWhile() {
	parser1.parse("procedure whileTest {while x{}}");
	CPPUNIT_ASSERT(ast1->contains("whileTest"));
	CPPUNIT_ASSERT(ast1->getProcNode("whileTest")->hasChildren() == true);
	CPPUNIT_ASSERT(ast1->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0)->getNodeType() == NodeType::WHILE_STMT_);
	
	WhileNode* whileNode = (WhileNode*) ast1->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0);
	CPPUNIT_ASSERT(whileNode->getStmtNum() == 1);
	CPPUNIT_ASSERT(whileNode->getVarNode()->getName() == "x");
	CPPUNIT_ASSERT(whileNode->getChildren().size() == 2);
}

void PDR_Integration_Test::testNestedWhile() {
	parser1.parse("procedure nestedWhile {while x{ while y{z = 2;}}}");
	CPPUNIT_ASSERT(ast1->contains("nestedWhile"));
	
	ProcNode* procNode = ast1->getProcNode("nestedWhile");
	CPPUNIT_ASSERT(procNode->hasChildren());
	
	StmtLstNode* stmtLst = procNode->getStmtLstNode();
	CPPUNIT_ASSERT(stmtLst->hasChildren());
	CPPUNIT_ASSERT(stmtLst->getChildren().at(0)->getNodeType() == NodeType::WHILE_STMT_);

	WhileNode* firstWhile = (WhileNode*)stmtLst->getChildren().at(0);
	CPPUNIT_ASSERT(firstWhile->getChildren().size() == 2);
	CPPUNIT_ASSERT(firstWhile->getVarNode()->getName() == "x");
	CPPUNIT_ASSERT(firstWhile->getStmtNum() == 1);

	StmtLstNode* firstWhileStmtLst = firstWhile->getStmtLstNode();
	CPPUNIT_ASSERT(firstWhileStmtLst->hasChildren());
	CPPUNIT_ASSERT(firstWhileStmtLst->getChildren().size() == 1);
	
	WhileNode* secondWhile = (WhileNode*)firstWhileStmtLst->getChildren().at(0);
	CPPUNIT_ASSERT(secondWhile->getChildren().size() == 2);
	CPPUNIT_ASSERT(secondWhile->getVarNode()->getName() == "y");
	CPPUNIT_ASSERT(secondWhile->getStmtNum() == 2);
	
	StmtLstNode* secondWhileStmtLst = secondWhile->getStmtLstNode();
	CPPUNIT_ASSERT(secondWhileStmtLst->hasChildren());
	CPPUNIT_ASSERT(secondWhileStmtLst->getChildren().size() == 1);
	CPPUNIT_ASSERT(secondWhileStmtLst->getChildren().at(0)->getNodeType() == NodeType::ASSIGN_STMT_);

	AssgNode* assign = (AssgNode*)secondWhileStmtLst->getChildren().at(0);
	CPPUNIT_ASSERT(assign->hasChildren());
	CPPUNIT_ASSERT(assign->getChildren().size() == 2);
	
	VarNode* modifiedVar = (VarNode*)assign->getChildren().at(0);
	CPPUNIT_ASSERT(modifiedVar->getName() == "z");

	ConstNode* constant = (ConstNode*)assign->getChildren().at(1);
	CPPUNIT_ASSERT(constant->getName() == "2");
}