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
	CPPUNIT_ASSERT(whileNode->getChildren().size() != 3);
	CPPUNIT_ASSERT(whileNode->getChildren().size() == 2);
}