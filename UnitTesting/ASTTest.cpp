#include <cppunit/config/SourcePrefix.h>
#include "ASTTest.h"

#include <iostream>
#include <string>

using namespace std;

AST* ast = AST::getInstance();
ProcNode* p = new ProcNode("Alice");

void ASTTest::setUp() {
	ast->addProcNode(p);
}

void ASTTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ASTTest );

void ASTTest::testInstances() {
	CPPUNIT_ASSERT(dynamic_cast<AST*>(ast));

	AST* asd = AST::getInstance();
	AST* qwe = AST::getInstance();

	CPPUNIT_ASSERT(asd->contains("Alice"));
	CPPUNIT_ASSERT(qwe->contains("Alice"));

	ProcNode* pn = new ProcNode("Bob");
	StmtLstNode* sln = new StmtLstNode("then");
	pn->linkStmtLstNode(sln);
	asd->addProcNode(pn);

	ProcNode* pget = asd->getProcNode("Bob");
	CPPUNIT_ASSERT(NULL != pget);

	string expname = "Bob";
	CPPUNIT_ASSERT_EQUAL(expname, pget->getName());
	CPPUNIT_ASSERT_EQUAL(pn, pget);

	StmtLstNode* sget = pget->getStmtLstNode();
	CPPUNIT_ASSERT_EQUAL(sln, sget);
	string sname = "then";
	CPPUNIT_ASSERT_EQUAL(sname, sget->getName());

	return;
}

void ASTTest::testAcrossMethods() {

	AST* asd = (AST::getInstance());
	CPPUNIT_ASSERT(asd->contains("Alice"));
	CPPUNIT_ASSERT(asd->contains("Bob"));

	ProcNode* pget = asd->getProcNode("Bob");
	CPPUNIT_ASSERT(NULL != pget);

	string expname = "Bob";
	CPPUNIT_ASSERT_EQUAL(expname, pget->getName());
	//CPPUNIT_ASSERT_EQUAL(pn, pget);

	StmtLstNode* sget = pget->getStmtLstNode();
	//CPPUNIT_ASSERT_EQUAL(sln, sget);
	string sname = "then";
	CPPUNIT_ASSERT_EQUAL(sname, sget->getName());

	return;
}