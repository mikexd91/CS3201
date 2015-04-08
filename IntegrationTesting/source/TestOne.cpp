#include <cppunit/config/SourcePrefix.h>
#include "TestOne.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"

// TEST VARIABLES

//ProcTable* procTable = ProcTable::getInstance();// test proctable instance
	Parser parser;
	PDR* pdr;
	AST* ast;
	VarTable* varTable;
	ProcTable* procTable;
	StmtTable* stmtTable;

void TestOne::setUp() {
	pdr = PDR::getInstance();
	ast = AST::getInstance();
	parser = Parser();
	varTable = VarTable::getInstance();
	procTable = ProcTable::getInstance();
	stmtTable = StmtTable::getInstance();

}

void TestOne::tearDown() {
	PDR::resetInstanceFlag();
	AST::reset();
	VarTable::reset();
	procTable->clearTable();
	stmtTable->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestOne );

// method to test adding of proc to table
void TestOne::testAddProc() {
	parser.parse("procedure test {x = 2; y=x+z; z=x+y+z; while i {y=x+1; a=a+b;} }");
	//ProcTable* procTable = ProcTable::getInstance();
	//StmtTable* stmtTable = StmtTable::getInstance();
	//VarTable* varTable = VarTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	
	
	Statement* stmt1 = stmtTable->getStmtObj(1);
	string initUses1[] = { "x" };
	set<string> str1(initUses1, initUses1 + 1);
	CPPUNIT_ASSERT(stmt1->getModifies() == str1);
	
	Statement* stmt2 = stmtTable->getStmtObj(2);
	string initModifies2[] = { "y" };
	set<string> strModifies2(initModifies2, initModifies2 + 1);
	CPPUNIT_ASSERT(stmt2->getModifies() == strModifies2);
	string initUses2[] = { "x", "z" };
	set<string> strUses2(initUses2, initUses2 + 2);
	CPPUNIT_ASSERT(stmt2->getUses() == strUses2);

	Statement* stmt3 = stmtTable->getStmtObj(3);
	string initModifies3[] = { "z" };
	set<string> strModifies3(initModifies3, initModifies3 + 1);
	CPPUNIT_ASSERT(stmt3->getModifies() == strModifies3);
	string initUses3[] = { "x", "y", "z" };
	set<string> strUses3(initUses3, initUses3 + 3);
	CPPUNIT_ASSERT(stmt3->getUses() == strUses3);

	Statement* stmt4 = stmtTable->getStmtObj(4);
	string initModifies4[] = { "y" };
	set<string> strModifies4(initModifies4, initModifies4 + 1);
	//CPPUNIT_ASSERT(stmt4->getModifies() == strModifies4);
	string initUses4[] = { "x", "i" };
	set<string> strUses4(initUses4, initUses4 + 1);
	//CPPUNIT_ASSERT(stmt4->getUses() == strUses4);
	int initChildren4[] = {5, 6};
	set<int> setChildren4(initChildren4, initChildren4 + 2);
	CPPUNIT_ASSERT(stmt4->getChildren() == setChildren4);

	Statement* stmt5 = stmtTable->getStmtObj(5);
	string initModifies5[] = { "y" };
	set<string> strModifies5(initModifies5, initModifies5 + 1);
	CPPUNIT_ASSERT(stmt5->getModifies() == strModifies5);
	string initUses5[] = { "x" };
	set<string> strUses5(initUses5, initUses5 + 1);
	CPPUNIT_ASSERT(stmt5->getUses() == strUses5);
	CPPUNIT_ASSERT(stmt5->getParent() == 4);
	
	
	Statement* stmt6 = stmtTable->getStmtObj(6);
	string initModifies6[] = { "a" };
	set<string> strModifies6(initModifies6, initModifies6 + 1);
	CPPUNIT_ASSERT(stmt6->getModifies() == strModifies6);
	
	string initUses6[] = { "a", "b" };
	set<string> strUses6(initUses6, initUses6 + 2);
	CPPUNIT_ASSERT(stmt6->getUses() == strUses6);
	CPPUNIT_ASSERT(stmt6->getParent() == 4);

	Variable* varX = varTable->getVariable("x");
	int initXUsedBy[] = {2, 3, 4, 5};
	int initXModifiedBy[] = {1};
	set<int> setXUsedBy(initXUsedBy, initXUsedBy + 4);
	set<int> setXModifiedBy(initXModifiedBy, initXModifiedBy + 1);
	CPPUNIT_ASSERT(varX->getUsedByStmts() == setXUsedBy);
	CPPUNIT_ASSERT(varX->getModifiedByStmts() == setXModifiedBy);

	Variable* varY = varTable->getVariable("y");
	int initYUsedBy[] = {3};
	int initYModifiedBy[] = {2, 4, 5};
	set<int> setYUsedBy(initYUsedBy, initYUsedBy + 1);
	set<int> setYModifiedBy(initYModifiedBy, initYModifiedBy + 3);
	CPPUNIT_ASSERT(varY->getUsedByStmts() == setYUsedBy);
	CPPUNIT_ASSERT(varY->getModifiedByStmts() == setYModifiedBy);

	Variable* varZ = varTable->getVariable("z");
	int initZUsedBy[] = {2, 3};
	int initZModifiedBy[] = {3};
	set<int> setZUsedBy(initZUsedBy, initZUsedBy + 2);
	set<int> setZModifiedBy(initZModifiedBy, initZModifiedBy + 1);
	CPPUNIT_ASSERT(varZ->getUsedByStmts() == setZUsedBy);
	CPPUNIT_ASSERT(varZ->getModifiedByStmts() == setZModifiedBy);
}



void TestOne::testAssign() {
	
	parser.parse("procedure test {x = 2; y=x;}");
	
	//StmtTable* stmtTable = StmtTable::getInstance();
	
	Statement* stmt = stmtTable->getStmtObj(2);
	
	string init[] = { "y" };
	set<string> str(init, init + 1);
	CPPUNIT_ASSERT(stmt->getModifies() == str);

}

// method to test false adding of proc to table
void TestOne::testFalseAddProc() {
	// Create a procedure
	
	CPPUNIT_ASSERT(true);
}

void TestOne::testWhileAST() {
	parser.parse("procedure whileTest {while x{}}");
	CPPUNIT_ASSERT(ast->contains("whileTest"));
	CPPUNIT_ASSERT(ast->getProcNode("whileTest")->hasChildren() == true);
	CPPUNIT_ASSERT(ast->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0)->getNodeType() == NodeType::WHILE_STMT_);
	
	WhileNode* whileNode = (WhileNode*) ast->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0);
	CPPUNIT_ASSERT(whileNode->getStmtNum() == 1);
	CPPUNIT_ASSERT(whileNode->getVarNode()->getName() == "x");
	CPPUNIT_ASSERT(whileNode->getChildren().size() == 2);
}

void TestOne::testNestedWhileAST() {
	parser.parse("procedure nestedWhile {while x{ while y{z = 2;}}}");
	CPPUNIT_ASSERT(ast->contains("nestedWhile"));
	
	ProcNode* procNode = ast->getProcNode("nestedWhile");
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

void TestOne::testSiblingsAST() {
	parser.parse("procedure testSiblings {x = 2; y = 3; while x{z = x + y;}}");
	CPPUNIT_ASSERT(ast->contains("testSiblings"));

	ProcNode* proc = ast->getProcNode("testSiblings");
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
	CPPUNIT_ASSERT(firstAssg->getStmtNum() == 1);
	CPPUNIT_ASSERT(secAssg->getStmtNum() == 2);
	CPPUNIT_ASSERT(whileNode->getStmtNum() == 3);

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
	CPPUNIT_ASSERT(thirdAssg->getStmtNum() == 4);
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

void TestOne::testMultipleProcAST() {
	parser.parse("procedure proc1{} procedure proc2{} procedure proc3{}");
	CPPUNIT_ASSERT(ast->contains("proc1"));
	CPPUNIT_ASSERT(ast->contains("proc2"));
	CPPUNIT_ASSERT(ast->contains("proc3"));

	ProcNode* proc1 = ast->getProcNode("proc1");
	ProcNode* proc2 = ast->getProcNode("proc2");
	ProcNode* proc3 = ast->getProcNode("proc3");
}

void TestOne::testFollows() {
	//StmtTable* stmtTable = StmtTable::getInstance();
	
	parser.parse("procedure proc{x = 2; y = x + 3; while y{z = y + x;} w = z + 2;}");
	CPPUNIT_ASSERT(ast->contains("proc"));
	
	Statement* firstAssg = stmtTable->getStmtObj(1);
	Statement* secAssg = stmtTable->getStmtObj(2);
	Statement* whileStmt = stmtTable->getStmtObj(3);
	Statement* thirdAssg = stmtTable->getStmtObj(4);
	Statement* fourthAssg = stmtTable->getStmtObj(5);
	CPPUNIT_ASSERT(firstAssg->getFollowsAfter() == -1);
	CPPUNIT_ASSERT(firstAssg->getFollowsBefore() == 2);
	CPPUNIT_ASSERT(secAssg->getFollowsAfter() == 1);
	CPPUNIT_ASSERT(secAssg->getFollowsBefore() == 3);
	CPPUNIT_ASSERT(whileStmt->getFollowsAfter() == 2);
	CPPUNIT_ASSERT(whileStmt->getFollowsBefore() == 5);
	CPPUNIT_ASSERT(thirdAssg->getFollowsAfter() == -1);
	CPPUNIT_ASSERT(thirdAssg->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(fourthAssg->getFollowsAfter() == 3);
	CPPUNIT_ASSERT(fourthAssg->getFollowsBefore() == -1);
}

void TestOne::testWhileUses() {
	parser.parse("procedure proc { while w { while y { z = a + b; }} }");
	
	Statement* firstWhile = stmtTable->getStmtObj(1);
	Statement* secWhile = stmtTable->getStmtObj(2);
	
	string firstInit[] = {"a", "b", "w", "y"};
	set<string> firstUses(firstInit, firstInit + 4);
	CPPUNIT_ASSERT(firstWhile->getUses() == firstUses);

	string secInit[] = {"a", "b", "y"};
	set<string> secUses(secInit, secInit + 3);
	CPPUNIT_ASSERT(secWhile->getUses() == secUses);

}