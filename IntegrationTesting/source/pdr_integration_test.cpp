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

void PDR_Integration_Test::testSiblings() {
	parser1.parse("procedure testSiblings {x = 2; y = 3; while x{z = x + y;}}");
	CPPUNIT_ASSERT(ast1->contains("testSiblings"));

	ProcNode* proc = ast1->getProcNode("testSiblings");
	CPPUNIT_ASSERT(proc->hasChildren());

	StmtLstNode* procStmtLst = proc->getStmtLstNode();
	CPPUNIT_ASSERT(procStmtLst->hasChildren());
	CPPUNIT_ASSERT(procStmtLst->getChildren().size() == 3);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(0)->getNodeType() == NodeType::ASSIGN_STMT_);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(1)->getNodeType() == NodeType::ASSIGN_STMT_);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(2)->getNodeType() == NodeType::WHILE_STMT_);

	AssgNode* firstAssg = (AssgNode*)procStmtLst->getChildren().at(0);
	AssgNode* secAssg = (AssgNode*)procStmtLst->getChildren().at(1);
	WhileNode* whileNode = (WhileNode*)procStmtLst->getChildren().at(2);

	// testing the nodes
	CPPUNIT_ASSERT(firstAssg->getVarNode()->getName() == "x");
	CPPUNIT_ASSERT(firstAssg->getExprNode()->getName() == "2");
	CPPUNIT_ASSERT(secAssg->getVarNode()->getName() == "y");
	CPPUNIT_ASSERT(secAssg->getExprNode()->getName() == "3");
	CPPUNIT_ASSERT(whileNode->getVarNode()->getName() == "x");

	// testing sibling linkages
	CPPUNIT_ASSERT(firstAssg->getLeftSibling() == NULL);
	CPPUNIT_ASSERT(firstAssg->getRightSibling() == secAssg);
	CPPUNIT_ASSERT(secAssg->getLeftSibling() == firstAssg);
	CPPUNIT_ASSERT(secAssg->getRightSibling() == whileNode);
	CPPUNIT_ASSERT(whileNode->getLeftSibling() == secAssg);
	CPPUNIT_ASSERT(whileNode->getRightSibling() == NULL);

	// test while child
	StmtLstNode* whileStmtLst = whileNode->getStmtLstNode();
	CPPUNIT_ASSERT(whileStmtLst->hasChildren());

	AssgNode* thirdAssg = (AssgNode*)whileStmtLst->getChildren().at(0);
	CPPUNIT_ASSERT(thirdAssg->getLeftSibling() == NULL);
	CPPUNIT_ASSERT(thirdAssg->getRightSibling() == NULL);
	CPPUNIT_ASSERT(thirdAssg->getVarNode()->getName() == "z");
	CPPUNIT_ASSERT(thirdAssg->getExprNode()->getNodeType() == NodeType::OPERATOR_);

	OpNode* operat = (OpNode*)thirdAssg->getExprNode();
	CPPUNIT_ASSERT(operat->getName() == "+");
	CPPUNIT_ASSERT(operat->getChildren().size() == 2);
	CPPUNIT_ASSERT(operat->getChildren().at(0)->getName() == "x");
	CPPUNIT_ASSERT(operat->getChildren().at(1)->getName() == "y");
}