#include <cppunit/config/SourcePrefix.h>
#include "FrontEndTest.h"
#include "../SPA/Parser.h"
#include "../SPA/PDR.h"

using namespace std;
// TEST VARIABLES

//ProcTable* procTable = ProcTable::getInstance();// test proctable instance
Parser parser;
PDR* pdr;
AST* ast;
CFG* cfg;
CFGBuilder* builder;
VarTable* varTable1;
ProcTable* procTable;
StmtTable* stmtTable1;
ConstTable* constTable;

void FrontEndTest::setUp() {
	pdr = PDR::getInstance();
	ast = AST::getInstance();
	constTable = ConstTable::getInstance();
	parser = Parser();
	varTable1 = VarTable::getInstance();
	procTable = ProcTable::getInstance();
	stmtTable1 = StmtTable::getInstance();

}

void FrontEndTest::tearDown() {
	PDR::resetInstanceFlag();
	AST::reset();
	CFG::reset();
	CFGBuilder::resetInstanceFlag();
	constTable->clearTable();
	VarTable::reset();
	procTable->clearTable();
	stmtTable1->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( FrontEndTest );

// method to test adding of proc to table
void FrontEndTest::testAddProc() {
	parser.parse("procedure test {x = 2; y=x+z; z=x+y+z; while i {y=x+1; a=a+b;} }");
	ProcTable* procTable = ProcTable::getInstance();
	StmtTable* stmtTable1 = StmtTable::getInstance();
	VarTable* varTable1 = VarTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	
	Statement* stmt1 = stmtTable1->getStmtObj(1);
	string initUses1[] = { "x" };
	unordered_set<string> str1(initUses1, initUses1 + 1);
	CPPUNIT_ASSERT(stmt1->getModifies() == str1);
	
	Statement* stmt2 = stmtTable1->getStmtObj(2);
	string initModifies2[] = { "y" };
	unordered_set<string> strModifies2(initModifies2, initModifies2 + 1);
	CPPUNIT_ASSERT(stmt2->getModifies() == strModifies2);
	string initUses2[] = { "x", "z" };
	unordered_set<string> strUses2(initUses2, initUses2 + 2);
	CPPUNIT_ASSERT(stmt2->getUses() == strUses2);

	Statement* stmt3 = stmtTable1->getStmtObj(3);
	string initModifies3[] = { "z" };
	unordered_set<string> strModifies3(initModifies3, initModifies3 + 1);
	CPPUNIT_ASSERT(stmt3->getModifies() == strModifies3);
	string initUses3[] = { "x", "y", "z" };
	unordered_set<string> strUses3(initUses3, initUses3 + 3);
	CPPUNIT_ASSERT(stmt3->getUses() == strUses3);

	Statement* stmt4 = stmtTable1->getStmtObj(4);
	string initModifies4[] = { "y" };
	unordered_set<string> strModifies4(initModifies4, initModifies4 + 1);
	//CPPUNIT_ASSERT(stmt4->getModifies() == strModifies4);
	string initUses4[] = { "x", "i" };
	unordered_set<string> strUses4(initUses4, initUses4 + 1);
	//CPPUNIT_ASSERT(stmt4->getUses() == strUses4);
	int initChildren4[] = {5, 6};
	unordered_set<int> setChildren4(initChildren4, initChildren4 + 2);
	CPPUNIT_ASSERT(stmt4->getChildren() == setChildren4);
	CPPUNIT_ASSERT(stmt4->getChildrenStar() == setChildren4);

	Statement* stmt5 = stmtTable1->getStmtObj(5);
	string initModifies5[] = { "y" };
	unordered_set<string> strModifies5(initModifies5, initModifies5 + 1);
	CPPUNIT_ASSERT(stmt5->getModifies() == strModifies5);
	string initUses5[] = { "x" };
	unordered_set<string> strUses5(initUses5, initUses5 + 1);
	CPPUNIT_ASSERT(stmt5->getUses() == strUses5);
	CPPUNIT_ASSERT(stmt5->getParent() == 4);
	unordered_set<int> parentStar5 = unordered_set<int>();
	parentStar5.insert(4);
	CPPUNIT_ASSERT(stmt5->getParentStar() == parentStar5);
	
	Statement* stmt6 = stmtTable1->getStmtObj(6);
	string initModifies6[] = { "a" };
	unordered_set<string> strModifies6(initModifies6, initModifies6 + 1);
	CPPUNIT_ASSERT(stmt6->getModifies() == strModifies6);
	string initUses6[] = { "a", "b" };
	unordered_set<string> strUses6(initUses6, initUses6 + 2);
	CPPUNIT_ASSERT(stmt6->getUses() == strUses6);
	CPPUNIT_ASSERT(stmt6->getParent() == 4);
	unordered_set<int> parentStar6 = unordered_set<int>();
	parentStar6.insert(4);
	CPPUNIT_ASSERT(stmt6->getParentStar() == parentStar6);

	Variable* varX = varTable1->getVariable("x");
	int initXUsedBy[] = {2, 3, 4, 5};
	int initXModifiedBy[] = {1};
	unordered_set<int> setXUsedBy(initXUsedBy, initXUsedBy + 4);
	unordered_set<int> setXModifiedBy(initXModifiedBy, initXModifiedBy + 1);
	CPPUNIT_ASSERT(varX->getUsedByStmts() == setXUsedBy);
	CPPUNIT_ASSERT(varX->getModifiedByStmts() == setXModifiedBy);

	Variable* varY = varTable1->getVariable("y");
	int initYUsedBy[] = {3};
	int initYModifiedBy[] = {2, 4, 5};
	unordered_set<int> setYUsedBy(initYUsedBy, initYUsedBy + 1);
	unordered_set<int> setYModifiedBy(initYModifiedBy, initYModifiedBy + 3);
	CPPUNIT_ASSERT(varY->getUsedByStmts() == setYUsedBy);
	CPPUNIT_ASSERT(varY->getModifiedByStmts() == setYModifiedBy);

	Variable* varZ = varTable1->getVariable("z");
	int initZUsedBy[] = {2, 3};
	int initZModifiedBy[] = {3};
	unordered_set<int> setZUsedBy(initZUsedBy, initZUsedBy + 2);
	unordered_set<int> setZModifiedBy(initZModifiedBy, initZModifiedBy + 1);
	CPPUNIT_ASSERT(varZ->getUsedByStmts() == setZUsedBy);
	CPPUNIT_ASSERT(varZ->getModifiedByStmts() == setZModifiedBy);
}



void FrontEndTest::testAssign() {
	
	parser.parse("procedure test {x = 2; y=x;}");
	
	//StmtTable* stmtTable1 = StmtTable::getInstance();
	
	Statement* stmt = stmtTable1->getStmtObj(2);
	
	string init[] = { "y" };
	unordered_set<string> str(init, init + 1);
	CPPUNIT_ASSERT(stmt->getModifies() == str);

}

// method to test false adding of proc to table
void FrontEndTest::testFalseAddProc() {
	// Create a procedure
	
	CPPUNIT_ASSERT(true);
}

//TO DO INVALIDATION!
void FrontEndTest::testWhileAST() {
	parser.parse("procedure whileTest {while x{y = x;}}");
	CPPUNIT_ASSERT(ast->contains("whileTest"));
	CPPUNIT_ASSERT(ast->getProcNode("whileTest")->hasChildren() == true);
	CPPUNIT_ASSERT(ast->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0)->getNodeType() == WHILE_STMT_);
	
	WhileNode* whileNode = (WhileNode*) ast->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0);
	CPPUNIT_ASSERT(whileNode->getStmtNum() == 1);
	CPPUNIT_ASSERT(whileNode->getVarNode()->getName() == "x");
	CPPUNIT_ASSERT(whileNode->getChildren().size() == 2);
}

void FrontEndTest::testNestedWhileAST() {
	parser.parse("procedure nestedWhile {while x{ while y{z = 2;}}}");
	CPPUNIT_ASSERT(ast->contains("nestedWhile"));
	
	ProcNode* procNode = ast->getProcNode("nestedWhile");
	CPPUNIT_ASSERT(procNode->hasChildren());
	
	StmtLstNode* stmtLst = procNode->getStmtLstNode();
	CPPUNIT_ASSERT(stmtLst->hasChildren());
	CPPUNIT_ASSERT(stmtLst->getChildren().at(0)->getNodeType() == WHILE_STMT_);

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
	CPPUNIT_ASSERT(secondWhileStmtLst->getChildren().at(0)->getNodeType() == ASSIGN_STMT_);

	AssgNode* assign = (AssgNode*)secondWhileStmtLst->getChildren().at(0);
	CPPUNIT_ASSERT(assign->hasChildren());
	CPPUNIT_ASSERT(assign->getChildren().size() == 2);
	
	VarNode* modifiedVar = (VarNode*)assign->getChildren().at(0);
	CPPUNIT_ASSERT(modifiedVar->getName() == "z");

	ConstNode* constant = (ConstNode*)assign->getChildren().at(1);
	CPPUNIT_ASSERT(constant->getName() == "2");
}

void FrontEndTest::testSiblingsAST() {
	parser.parse("procedure testSiblings {x = 2; y = 3; while x{z = x + y;}}");
	CPPUNIT_ASSERT(ast->contains("testSiblings"));

	ProcNode* proc = ast->getProcNode("testSiblings");
	CPPUNIT_ASSERT(proc->hasChildren());

	StmtLstNode* procStmtLst = proc->getStmtLstNode();
	CPPUNIT_ASSERT(procStmtLst->hasChildren());
	CPPUNIT_ASSERT(procStmtLst->getChildren().size() == 3);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(0)->getNodeType() == ASSIGN_STMT_);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(1)->getNodeType() == ASSIGN_STMT_);
	CPPUNIT_ASSERT(procStmtLst->getChildren().at(2)->getNodeType() == WHILE_STMT_);

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
	CPPUNIT_ASSERT(thirdAssg->getExprNode()->getNodeType() == OPERATOR_);

	OpNode* operat = (OpNode*)thirdAssg->getExprNode();
	CPPUNIT_ASSERT(operat->getName() == "+");
	CPPUNIT_ASSERT(operat->getChildren().size() == 2);
	CPPUNIT_ASSERT(operat->getChildren().at(0)->getName() == "x");
	CPPUNIT_ASSERT(operat->getChildren().at(1)->getName() == "y");
}

void FrontEndTest::testMultipleProcAST() {
	parser.parse("procedure proc1{x = 1;} procedure proc2{x = 1;} procedure proc3{x = 1;}");
	CPPUNIT_ASSERT(ast->contains("proc1"));
	CPPUNIT_ASSERT(ast->contains("proc2"));
	CPPUNIT_ASSERT(ast->contains("proc3"));

	ProcNode* proc1 = ast->getProcNode("proc1");
	ProcNode* proc2 = ast->getProcNode("proc2");
	ProcNode* proc3 = ast->getProcNode("proc3");

	CPPUNIT_ASSERT(proc1->getRightSibling() == proc2);
	CPPUNIT_ASSERT(proc2->getRightSibling() == proc3);
}

void FrontEndTest::testFollows() {
	//StmtTable* stmtTable1 = StmtTable::getInstance();
	
	parser.parse("procedure proc{x = 2; y = x + 3; while y{z = y + x;} w = z + 2;}");
	CPPUNIT_ASSERT(ast->contains("proc"));
	
	Statement* firstAssg = stmtTable1->getStmtObj(1);
	Statement* secAssg = stmtTable1->getStmtObj(2);
	Statement* whileStmt = stmtTable1->getStmtObj(3);
	Statement* thirdAssg = stmtTable1->getStmtObj(4);
	Statement* fourthAssg = stmtTable1->getStmtObj(5);
	//firstAssg
	CPPUNIT_ASSERT(firstAssg->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(firstAssg->getFollowsAfter() == 2);
	CPPUNIT_ASSERT(firstAssg->getFollowsStarBefore().empty());
	int initFollowsStarAfter1[] = {2, 3, 5};
	unordered_set<int> followsStarAfter1 = unordered_set<int>(initFollowsStarAfter1, initFollowsStarAfter1+3);
	CPPUNIT_ASSERT(firstAssg->getFollowsStarAfter() == followsStarAfter1);
	//secAssg
	CPPUNIT_ASSERT(secAssg->getFollowsBefore() == 1);
	CPPUNIT_ASSERT(secAssg->getFollowsAfter() == 3);
	int initFollowsStarBefore2[] = {1};
	unordered_set<int> followsStarBefore2 = unordered_set<int>(initFollowsStarBefore2, initFollowsStarBefore2+1);
	CPPUNIT_ASSERT(secAssg->getFollowsStarBefore() == followsStarBefore2);
	int initFollowsStarAfter2[] = {3, 5};
	unordered_set<int> followsStarAfter2 = unordered_set<int>(initFollowsStarAfter2, initFollowsStarAfter2+2);
	CPPUNIT_ASSERT(secAssg->getFollowsStarAfter() == followsStarAfter2);
	//whileStmt
	CPPUNIT_ASSERT(whileStmt->getFollowsBefore() == 2);
	CPPUNIT_ASSERT(whileStmt->getFollowsAfter() == 5);
	int initFollowsStarBeforeWhile[] = {1, 2};
	unordered_set<int> followsStarBeforeWhile = unordered_set<int>(initFollowsStarBeforeWhile, initFollowsStarBeforeWhile+2);
	CPPUNIT_ASSERT(whileStmt->getFollowsStarBefore() == followsStarBeforeWhile);
	int initFollowsStarAfterWhile[] = {5};
	unordered_set<int> followsStarAfterWhile = unordered_set<int>(initFollowsStarAfterWhile, initFollowsStarAfterWhile+1);
	CPPUNIT_ASSERT(whileStmt->getFollowsStarAfter() == followsStarAfterWhile);
	//thirdAssg
	CPPUNIT_ASSERT(thirdAssg->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(thirdAssg->getFollowsAfter() == -1);
	CPPUNIT_ASSERT(thirdAssg->getFollowsStarBefore().empty());
	CPPUNIT_ASSERT(thirdAssg->getFollowsStarAfter().empty());
	//fourthAssg
	CPPUNIT_ASSERT(fourthAssg->getFollowsBefore() == 3);
	CPPUNIT_ASSERT(fourthAssg->getFollowsAfter() == -1);
	int initFollowsStarBefore4[] = {1, 2, 3};
	unordered_set<int> followsStarBefore4 = unordered_set<int>(initFollowsStarBefore4, initFollowsStarBefore4+3);
	CPPUNIT_ASSERT(fourthAssg->getFollowsStarBefore() == followsStarBefore4);
	CPPUNIT_ASSERT(fourthAssg->getFollowsStarAfter().empty());
}

void FrontEndTest::testWhileUses() {
	parser.parse("procedure proc { while w { while y { z = a + b; }} }");
	
	Statement* firstWhile = stmtTable1->getStmtObj(1);
	Statement* secWhile = stmtTable1->getStmtObj(2);
	
	string firstInit[] = {"a", "b", "w", "y"};
	unordered_set<string> firstUses(firstInit, firstInit + 4);
	CPPUNIT_ASSERT(firstWhile->getUses() == firstUses);

	string secInit[] = {"a", "b", "y"};
	unordered_set<string> secUses(secInit, secInit + 3);
	CPPUNIT_ASSERT(secWhile->getUses() == secUses);

}

void FrontEndTest::testWhileModifies() {
	parser.parse("procedure proc { while x { while y {x = 2; y = 2; z = x + y;}}} ");
	
	Statement* firstWhile = stmtTable1->getStmtObj(1);
	Statement* secWhile = stmtTable1->getStmtObj(2);
	Statement* firstAssg = stmtTable1->getStmtObj(3);
	Statement* secAssg = stmtTable1->getStmtObj(4);
	Statement* thirdAssg = stmtTable1->getStmtObj(5);
	
	string modi[] = {"x", "y", "z"};
	unordered_set<string> modifiesSet(modi, modi + 3);
	CPPUNIT_ASSERT(firstWhile->getModifies() == modifiesSet);
	CPPUNIT_ASSERT(secWhile->getModifies() == modifiesSet);
	

	unordered_set<string> firstAssgModSet;
	firstAssgModSet.insert("x");
	CPPUNIT_ASSERT(firstAssg->getModifies() == firstAssgModSet);
	
	unordered_set<string> secondAssgModSet;
	secondAssgModSet.insert("y");
	CPPUNIT_ASSERT(secAssg->getModifies() == secondAssgModSet);

	unordered_set<string> thirdAssgModSet;
	thirdAssgModSet.insert("z");
	CPPUNIT_ASSERT(thirdAssg->getModifies() == thirdAssgModSet);
}

void FrontEndTest::testParent() {
	parser.parse("procedure proc { while x { while y {x = 2; y = 2; z = x + y;} if b then {c = 3;} else {d = 4;} }} ");
	Statement* firstWhile = stmtTable1->getStmtObj(1);
	Statement* secWhile = stmtTable1->getStmtObj(2);
	Statement* firstAssg = stmtTable1->getStmtObj(3);
	Statement* secAssg = stmtTable1->getStmtObj(4);
	Statement* thirdAssg = stmtTable1->getStmtObj(5);
	Statement* firstIf = stmtTable1->getStmtObj(6);
	Statement* fourthAssg = stmtTable1->getStmtObj(7);
	Statement* fifthAssg = stmtTable1->getStmtObj(8);

	//for firstWhile
	int c1[] = {2, 6};
	unordered_set<int> children1(c1, c1+2);
	CPPUNIT_ASSERT(firstWhile->getChildren() == children1);
	CPPUNIT_ASSERT(firstWhile->getParent() == -1);

	int cStar1[] = {2, 3, 4, 5, 6, 7, 8};
	unordered_set<int> childrenStar1(cStar1, cStar1+ 7);
	CPPUNIT_ASSERT(firstWhile->getChildrenStar() == childrenStar1);
	CPPUNIT_ASSERT(firstWhile->getParentStar().empty());

	//for secondWhile
	int c2[] = {3, 4, 5};
	unordered_set<int> children2(c2, c2+3);
	CPPUNIT_ASSERT(secWhile->getChildren() == children2);
	CPPUNIT_ASSERT(secWhile->getParent() == 1);

	int cStar2[] = {3, 4, 5};
	unordered_set<int> childrenStar2(cStar2, cStar2+3);
	CPPUNIT_ASSERT(secWhile->getChildrenStar() == childrenStar2);
	int pStar2[] = {1};
	unordered_set<int> parentStar2(pStar2, pStar2+1);
	CPPUNIT_ASSERT(secWhile->getParentStar() == parentStar2);

	//for firstAssg
	CPPUNIT_ASSERT(firstAssg->getParent() == 2);
	int pStar3[] = {1, 2};
	unordered_set<int> parentStar3(pStar3, pStar3+2);
	CPPUNIT_ASSERT(firstAssg->getParentStar() == parentStar3);

	//for secAssg
	CPPUNIT_ASSERT(secAssg->getParent() == 2);
	CPPUNIT_ASSERT(secAssg->getParentStar() == parentStar3);

	//for thirdAssg
	CPPUNIT_ASSERT(thirdAssg->getParent() == 2);
	CPPUNIT_ASSERT(thirdAssg->getParentStar() == parentStar3);

	//for firstIf
	int c6[] = {7, 8};
	unordered_set<int> children6(c6, c6+2);
	CPPUNIT_ASSERT(firstIf->getChildren() == children6);
	CPPUNIT_ASSERT(firstIf->getParent() == 1);

	CPPUNIT_ASSERT(firstIf->getChildrenStar() == children6);
	int pStar6[] = {1};
	unordered_set<int> parentStar6(pStar6, pStar6+1);
	CPPUNIT_ASSERT(firstIf->getParentStar() == parentStar6);

	//for fourthAssg
	CPPUNIT_ASSERT(fourthAssg->getParent() == 6);
	int pStar7[] = {1, 6};
	unordered_set<int> parentStar7(pStar7, pStar7+2);
	CPPUNIT_ASSERT(fourthAssg->getParentStar() == parentStar7);

	//for fifthAssg
	CPPUNIT_ASSERT(fifthAssg->getParent() == 6);
	CPPUNIT_ASSERT(fifthAssg->getParentStar() == parentStar7);
}

void FrontEndTest::testStmtTableAllWhile() {
	parser.parse("procedure proc3 { while x {x = 2;} while y{x = 3;} while z{x = 2; while w {x = 2;}} }");

	unordered_set<Statement*> whileStmts = stmtTable1->getWhileStmts();
	CPPUNIT_ASSERT(whileStmts.size() == 4);

	unordered_set<Statement*>::iterator iter;

	for(iter = whileStmts.begin(); iter != whileStmts.end(); iter++) {
		Statement* stmt = *iter;
		int stmtNum = stmt->getStmtNum();
		switch(stmtNum) {
			case 1: {
				string arrInit[] = {"x"};
				unordered_set<string> usesSet(arrInit, arrInit + 1);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 3: {
				string arrInit[] = {"y"};
				unordered_set<string> usesSet(arrInit, arrInit + 1);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 5: {
				string arrInit[] = {"w", "z"};
				unordered_set<string> usesSet(arrInit, arrInit + 2);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 7: {
				string arrInit[] = {"w"};
				unordered_set<string> usesSet(arrInit, arrInit + 1);
				stmt->getUses();
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			default: 
				break;
		}
	}
}

void FrontEndTest::testConstTable() {
	parser.parse("procedure proc {x = 2; y = 3;}");

	vector<Constant*> allConst = constTable->getAllConst();
	CPPUNIT_ASSERT(allConst.size() == 2);

	Constant* firstConstant = allConst[0];
	Constant* secConstant = allConst[1];

	string combi[2] = {firstConstant->getConstName(), secConstant->getConstName()};
	string firstCombi[2] = {"2", "3"};
	string secCombi[2] = {"3", "2"};

	CPPUNIT_ASSERT(combi == firstCombi || secCombi);
}

void FrontEndTest::testCallsAST() {
	parser.parse("procedure proc1{call proc2;} procedure proc2{x = 2;}");

	CPPUNIT_ASSERT(procTable->contains("proc1"));
	CPPUNIT_ASSERT(procTable->contains("proc2"));

	ProcNode* proc1Node = ast->getProcNode("proc1");
	vector<TNode*> proc1Children = proc1Node->getChildren();
	TNode* proc1StmtLstNode = proc1Children[0];

	vector<TNode*> stmtLstNodeChildren = proc1StmtLstNode->getChildren();
	CPPUNIT_ASSERT(stmtLstNodeChildren.size() == 1);

	TNode* call1Node = stmtLstNodeChildren[0];
	CPPUNIT_ASSERT(call1Node->getNodeType() == CALL_STMT_);
	CPPUNIT_ASSERT(call1Node->getName() == "proc2");

	ProcNode* proc2Node = ast->getProcNode("proc2");
	vector<TNode*> proc2Children = proc2Node->getChildren();
	TNode* proc2StmtLstNode = proc2Children[0];

	vector<TNode*> stmtLstNode2Children = proc2StmtLstNode->getChildren();
	CPPUNIT_ASSERT(stmtLstNode2Children.size() == 1);
	
	TNode* assignNode = stmtLstNode2Children[0];
	CPPUNIT_ASSERT(assignNode->getNodeType() == ASSIGN_STMT_);
	CPPUNIT_ASSERT(assignNode->getChildren().size() == 2);
}

void FrontEndTest::testCallsPKB() {
	parser.parse("procedure proc1{call proc2;} procedure proc2{x = 2;}");

	Statement* stmt1 = stmtTable1->getStmtObj(1);
	CPPUNIT_ASSERT(stmt1->getType() == CALL_STMT_);
	CPPUNIT_ASSERT(stmt1->getCalls() == "proc2");

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	CPPUNIT_ASSERT(stmt2->getType() == ASSIGN_STMT_);

	Procedure* procedure1 = procTable->getProcObj("proc1");
	Procedure* procedure2 = procTable->getProcObj("proc2");
	
	unordered_set<Procedure*> calledBy;
	calledBy.insert(procedure1);
	CPPUNIT_ASSERT(procedure2->getCalledBy() == calledBy);

	unordered_set<Procedure*> calls;
	calls.insert(procedure2);
	CPPUNIT_ASSERT(procedure1->getCalls() == calls);
}

void FrontEndTest::testProcedureUses() {
	parser.parse("procedure proc1 { x = 2; y = x; } procedure proc2{ z = 3; y = 1; x = z + y;}");

	Procedure* procedure = procTable->getProcObj("proc1");
	string uses[] = {"x"};
	unordered_set<string> usesSet(uses, uses + 1);
	CPPUNIT_ASSERT(procedure->getUses() == usesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string uses2[] = {"z", "y"};
	unordered_set<string> usesSet2(uses2, uses2 + 2);
	CPPUNIT_ASSERT(procedure2->getUses() == usesSet2);
}

void FrontEndTest::testProcedureModifies() {
	parser.parse("procedure proc1 {a = 3; b = a + 4;} procedure proc2 {x = 4; y = 2; z = x + y;}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string modifies1[] = {"a", "b"};
	unordered_set<string> modifiesSet1(modifies1, modifies1 + 2);
	CPPUNIT_ASSERT(procedure1->getModifies() == modifiesSet1);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string modifies2[] = {"x", "y", "z"};
	unordered_set<string> modifiesSet2(modifies2, modifies2 + 3);
	CPPUNIT_ASSERT(procedure2->getModifies() == modifiesSet2);
}

void FrontEndTest::testNestedProceduresUses() {
	parser.parse("procedure proc1 {call proc2;} procedure proc2 {x = 2; while x {y = 2;}}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string uses[] = {"x"};
	unordered_set<string> usesSet(uses, uses + 1);
	CPPUNIT_ASSERT(procedure1->getUses() == usesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	CPPUNIT_ASSERT(procedure2->getUses() == usesSet);
}

void FrontEndTest::testNestedProceduresModifies() {
	parser.parse("procedure proc1 { a = 2; call proc2;} procedure proc2 {x = 2; z = x; y = x + z;}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string modifies[] = {"a", "x", "z", "y"};
	unordered_set<string> modifiesSet(modifies, modifies + 4);
	CPPUNIT_ASSERT(procedure1->getModifies() == modifiesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string modifies2[] = {"x", "z", "y"};
	unordered_set<string> modifiesSet2(modifies2, modifies2 + 3);
	CPPUNIT_ASSERT(procedure2->getModifies() == modifiesSet2);
}

void FrontEndTest::testIfStatement() {
	parser.parse("procedure proc { a = 2; if a then {y = 2;} else { z = 3; } }");

	Statement* ifStmt = stmtTable1->getStmtObj(2);
	CPPUNIT_ASSERT(ifStmt->getType() == IF_STMT_);
	
	string ifUses[] = {"a"};
	unordered_set<string> ifUsesSet(ifUses, ifUses + 1);
	CPPUNIT_ASSERT(ifStmt->getUses() == ifUsesSet);

	string ifModifies[] = {"y", "z"};
	unordered_set<string> ifModifiesSet(ifModifies, ifModifies + 2);
	CPPUNIT_ASSERT(ifStmt->getModifies() == ifModifiesSet);

	Procedure* proc = procTable->getProcObj("proc");
	CPPUNIT_ASSERT(proc->getUses() == ifUsesSet);

	string procModifies[] = {"a", "y", "z"};
	unordered_set<string> procModifiesSet(procModifies, procModifies + 3);
	CPPUNIT_ASSERT(proc->getModifies() == procModifiesSet);

	CPPUNIT_ASSERT(ifStmt->getFollowsBefore() == 1);

	Statement* secondAssign = stmtTable1->getStmtObj(3);
	CPPUNIT_ASSERT(secondAssign->getParent() == 2);
	CPPUNIT_ASSERT(secondAssign->getFollowsAfter() == -1);
	CPPUNIT_ASSERT(secondAssign->getFollowsBefore() == -1);

	Statement* thirdAssign = stmtTable1->getStmtObj(4);
	CPPUNIT_ASSERT(thirdAssign->getParent() == 2);
	CPPUNIT_ASSERT(thirdAssign->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(thirdAssign->getFollowsAfter() == -1);
}

void FrontEndTest::testNestedIfStatement() {
	parser.parse("procedure proc { a = 1; if a then { b = 2; if b then {c = 3;} else {d = 4;}} else {e = 5;}}");
	
	Statement* firstIf = stmtTable1->getStmtObj(2);
	Statement* secondIf = stmtTable1->getStmtObj(4);
	CPPUNIT_ASSERT(secondIf->getParent() == 2);

	TNode* firstIfNode = firstIf->getTNodeRef();
	TNode* secondIfNode = secondIf->getTNodeRef();
	CPPUNIT_ASSERT(secondIfNode->getParent()->getParent() == firstIfNode);
	
	string firstModifies[] = {"b", "c", "d", "e"};
	string secondModifies[] = {"c", "d"};
	unordered_set<string> firstModSet(firstModifies, firstModifies + 4);
	unordered_set<string> secondModSet(secondModifies, secondModifies + 2);
	CPPUNIT_ASSERT(firstIf->getModifies() == firstModSet);
	CPPUNIT_ASSERT(secondIf->getModifies() == secondModSet);
}

void FrontEndTest::testUsingProc() {
	parser.parse("procedure proc { x = 2; while x{ y = x;} if y then {z = y;} else {z = x;}}");

	Variable* varX = varTable1->getVariable("x");
	Variable* varY = varTable1->getVariable("y");
	Variable* varZ = varTable1->getVariable("z");

	string proc[] = {"proc"};
	unordered_set<string> procSet(proc, proc + 1);
	CPPUNIT_ASSERT(varX->getUsedByProc() == procSet);
	CPPUNIT_ASSERT(varY->getUsedByProc() == procSet);
	CPPUNIT_ASSERT(varZ->getUsedByProc().empty());
}

void FrontEndTest::testModifyingProc() {
	parser.parse("procedure proc { while x {y = 3; while y{z = 2; while z{x = z;}}} }");

	Variable* varX = varTable1->getVariable("x");
	Variable* varY = varTable1->getVariable("y");
	Variable* varZ = varTable1->getVariable("z");	

	string proc[] = {"proc"};
	unordered_set<string> procSet(proc, proc + 1);
	CPPUNIT_ASSERT(varX->getModifiedByProc() == procSet);
	CPPUNIT_ASSERT(varY->getModifiedByProc() == procSet);
	CPPUNIT_ASSERT(varZ->getModifiedByProc() == procSet);
}

void FrontEndTest::testUsingMultipleProc() {
	parser.parse("procedure proc1 {while  y{x = 2; y = x;} } procedure proc2 {x = 3; y = x;}");

	Variable* varX = varTable1->getVariable("x");
	Variable* varY = varTable1->getVariable("y");

	string proc1[] = {"proc1"};
	string proc2[] = {"proc1", "proc2"};
	unordered_set<string> procSet1(proc1, proc1 + 1);
	unordered_set<string> procSet2(proc2, proc2 + 2);
	CPPUNIT_ASSERT(varX->getUsedByProc() == procSet2);
	CPPUNIT_ASSERT(varY->getUsedByProc() == procSet1);
}

void FrontEndTest::testModifyingMultipleProc() {
	parser.parse("procedure proc1 { x = 3; } procedure proc2 { y = 4; } procedure proc3 {x = 4; if a then {w = 3;} else {z = 4;}}");

	Variable* varW = varTable1->getVariable("w");
	Variable* varX = varTable1->getVariable("x");
	Variable* varY = varTable1->getVariable("y");
	Variable* varZ = varTable1->getVariable("z");	

	Procedure* procedure1 = procTable->getProcObj("proc1");
	CPPUNIT_ASSERT(procedure1->getModifies().find("x") != procedure1->getModifies().end());

	string first[] = {"proc2"};
	unordered_set<string> firstSet(first, first + 1);
	CPPUNIT_ASSERT(varY->getModifiedByProc() == firstSet);

	string second[] = {"proc1", "proc3" };
	unordered_set<string> secondSet(second, second + 2);
	CPPUNIT_ASSERT(varX->getModifiedByProc() == secondSet);

	string third[] = {"proc3"};
	unordered_set<string> thirdSet(third, third + 1);
	CPPUNIT_ASSERT(varW->getModifiedByProc() == thirdSet);
	CPPUNIT_ASSERT(varZ->getModifiedByProc() == thirdSet);
}

void FrontEndTest::testModifyingMultipleProcCall() {
	parser.parse("procedure proc1 { x = 3; } procedure proc2 { call proc1; }");

	Variable* varX = varTable1->getVariable("x");

	Procedure* procedure2 = procTable->getProcObj("proc2");
	CPPUNIT_ASSERT(procedure2->getModifies().find("x") != procedure2->getModifies().end());

	string first[] = {"proc1", "proc2"};
	unordered_set<string> firstSet(first, first + 2);
	CPPUNIT_ASSERT(varX->getModifiedByProc() == firstSet);
}

void FrontEndTest::testUsingMultipleProcCall() {
	parser.parse("procedure proc1 { if x then { i=1; } else { z=2;} } procedure proc2 { call proc1; }");

	Variable* varX = varTable1->getVariable("x");

	Procedure* procedure2 = procTable->getProcObj("proc2");
	CPPUNIT_ASSERT(procedure2->getUses().find("x") != procedure2->getUses().end());

	string first[] = {"proc1", "proc2"};
	unordered_set<string> firstSet(first, first + 2);
	CPPUNIT_ASSERT(varX->getUsedByProc() == firstSet);
}

void FrontEndTest::testCFGSingleAssg() {
	/*
	procedure proc {
		x = 1;
	}
	*/
	parser.parse("procedure proc { x = 1; }");

	Statement* assgStmt = stmtTable1->getStmtObj(1);
	CPPUNIT_ASSERT(assgStmt->getNext().empty());
	CPPUNIT_ASSERT(assgStmt->getPrev().empty());
}

void FrontEndTest::testCFGMultAssg() {
	/*
	procedure proc {
		x = 1;
		y = 2;
		z = 3;
	}
	*/

	parser.parse("procedure proc { x = 1; y = 2; z = 3; }");

	Statement* assg1 = stmtTable1->getStmtObj(1);
	Statement* assg2 = stmtTable1->getStmtObj(2);
	Statement* assg3 = stmtTable1->getStmtObj(3);

	int assg1Next[] = {2};
	int assg2Prev[] = {1};
	int assg2Next[] = {3};
	int assg3Prev[] = {2};
	unordered_set<int> assg1NextSet(assg1Next, assg1Next + 1);
	unordered_set<int> assg2PrevSet(assg2Prev, assg2Prev + 1);
	unordered_set<int> assg2NextSet(assg2Next, assg2Next + 1);
	unordered_set<int> assg3PrevSet(assg3Prev, assg3Prev + 1);

	CPPUNIT_ASSERT(assg1->getPrev().empty());
	CPPUNIT_ASSERT(assg1->getNext() == assg1NextSet);
	CPPUNIT_ASSERT(assg2->getPrev() == assg2PrevSet);
	CPPUNIT_ASSERT(assg2->getNext() == assg2NextSet);
	CPPUNIT_ASSERT(assg3->getPrev() == assg3PrevSet);
	CPPUNIT_ASSERT(assg3->getNext().empty());
}

void FrontEndTest::testCFGMultProc() {
	/*
	procedure proc1 {
		x = 1;
		call proc2;
		y = 2;
	}

	procedure proc2 {
		y = 3;
		x = 2;
	}
	*/

	parser.parse("procedure proc1 { x = 1; call proc2; y = 2;} procedure proc2 { y = 3; x = 2;} ");
	
	Statement* stmt1 = stmtTable1->getStmtObj(1);
	Statement* stmt2 = stmtTable1->getStmtObj(2);
	Statement* stmt3 = stmtTable1->getStmtObj(3);
	Statement* stmt4 = stmtTable1->getStmtObj(4);
	Statement* stmt5 = stmtTable1->getStmtObj(5);

	int stmt1Next[] = {2};
	int stmt2Prev[] = {1};
	int stmt2Next[] = {3};
	int stmt3Prev[] = {2};
	int stmt4Next[] = {5};
	int stmt5Prev[] = {4};
	unordered_set<int> stmt1NextSet(stmt1Next, stmt1Next + 1);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 1);
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 1);
	unordered_set<int> stmt3PrevSet(stmt3Prev, stmt3Prev + 1);
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 1);
	unordered_set<int> stmt5PrevSet(stmt5Prev, stmt5Prev + 1);

	CPPUNIT_ASSERT(stmt1->getPrev().empty());
	CPPUNIT_ASSERT(stmt1->getNext() == stmt1NextSet);
	CPPUNIT_ASSERT(stmt2->getPrev() == stmt2PrevSet);
	CPPUNIT_ASSERT(stmt2->getNext() == stmt2NextSet);
	CPPUNIT_ASSERT(stmt3->getPrev() == stmt3PrevSet);
	CPPUNIT_ASSERT(stmt3->getNext().empty());
	CPPUNIT_ASSERT(stmt4->getPrev().empty());
	CPPUNIT_ASSERT(stmt4->getNext() == stmt4NextSet);
	CPPUNIT_ASSERT(stmt5->getPrev() == stmt5PrevSet);
	CPPUNIT_ASSERT(stmt5->getNext().empty());
}

void FrontEndTest::testCFGIfStmt() {
	/*
	procedure proc {
		x = 2;
		if x then {
			y = 3;
			z = 4;
		} else {
			y = 10;
			z = 11;
		}
		z = y + x;
	}
	*/

	parser.parse("procedure proc {x = 2; if x then {y = 3; z = 4;} else {y = 10; z = 11;} z = y + x;}");

	CPPUNIT_ASSERT(stmtTable1->getAllStmts().size() == 7);
	CPPUNIT_ASSERT(stmtTable1->getStmtObj(2)->getType() == NodeType::IF_STMT_);
	
	Statement* ifStmt = stmtTable1->getStmtObj(2);
	int ifStmtPrev[] = {1};
	int ifStmtNext[] = {3, 5};
	unordered_set<int> ifStmtPrevSet(ifStmtPrev, ifStmtPrev + 1);
	unordered_set<int> ifStmtNextSet(ifStmtNext, ifStmtNext + 2);
	CPPUNIT_ASSERT(ifStmt->getPrev() == ifStmtPrevSet);
	CPPUNIT_ASSERT(ifStmt->getNext() == ifStmtNextSet);

	Statement* lastStmt = stmtTable1->getStmtObj(7);
	int lastStmtPrev[] = {4, 6};
	unordered_set<int> lastStmtPrevSet(lastStmtPrev, lastStmtPrev + 2);
	CPPUNIT_ASSERT(lastStmt->getPrev() == lastStmtPrevSet);
	CPPUNIT_ASSERT(lastStmt->getNext().empty());	
}

void FrontEndTest::testCFGIfNested() {
	/*
	procedure proc {						 1
		x = 2;							     | 
		if x then {							 2
			y = 1;						  /     \
			if y then {					 3		 7		
				z = 2;					 |		 |	
			} else {					 4		 8
				a = 3;					/ \     / \
			}						   5   6   9   10
		} else {					    \ /     \ /
			w = 4;						 -       -
			if w then {					   \   /	
				x = 3;						 11
			} else {
				b = z;
			}
		}
		y = 3;
	}
	*/

	parser.parse("procedure proc { x = 2; if x then {y = 1; if y then { z = 2;} else { a = 3; }} else { w = 4; if w then { x = 3; } else { b = z; }} y = 3;}");

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	int stmt2Next[] = {3,7};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 2);
	CPPUNIT_ASSERT(stmt2->getNext() == stmt2NextSet);

	Statement* stmt4 = stmtTable1->getStmtObj(4);
	int stmt4Prev[] = {3};
	int stmt4Next[] = {5, 6};
	unordered_set<int> stmt4PrevSet(stmt4Prev, stmt4Prev + 1);
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 2);
	CPPUNIT_ASSERT(stmt4->getPrev() == stmt4PrevSet);
	CPPUNIT_ASSERT(stmt4->getNext() == stmt4NextSet);

	Statement* stmt8 = stmtTable1->getStmtObj(8);
	int stmt8Prev[] = {7};
	int stmt8Next[] = {9, 10};
	unordered_set<int> stmt8PrevSet(stmt8Prev, stmt8Prev + 1);
	unordered_set<int> stmt8NextSet(stmt8Next, stmt8Next + 2);
	CPPUNIT_ASSERT(stmt8->getPrev() == stmt8PrevSet);
	CPPUNIT_ASSERT(stmt8->getNext() == stmt8NextSet);

	Statement* stmt11 = stmtTable1->getStmtObj(11);
	int stmt11Prev[] = {5, 6, 9, 10};
	unordered_set<int> stmt11PrevSet(stmt11Prev, stmt11Prev + 4);
	CPPUNIT_ASSERT(stmt11->getPrev() == stmt11PrevSet);
	CPPUNIT_ASSERT(stmt11->getNext().empty());

	int next11[] = {11};
	unordered_set<int> next11Set(next11, next11 + 1);
	CPPUNIT_ASSERT(stmtTable1->getStmtObj(5)->getNext() == next11Set);
	CPPUNIT_ASSERT(stmtTable1->getStmtObj(6)->getNext() == next11Set);
	CPPUNIT_ASSERT(stmtTable1->getStmtObj(9)->getNext() == next11Set);
	CPPUNIT_ASSERT(stmtTable1->getStmtObj(10)->getNext() == next11Set);
}

void FrontEndTest::testCFGWhileStmt() {
	/*
	procedure proc {					1
		x = 3;							|
		while x {					<---2<--- 
			y = 4;				  /		|   /
			z = 5;				  \   3, 4
		}						   \
		z = 4;						\__>5
	}
	*/

	parser.parse("procedure proc {x = 3; while x {y = 4; z = 5;} z= 4;}");

	Statement* stmt1 = stmtTable1->getStmtObj(1);
	int stmt1Next[] = {2};
	unordered_set<int> stmt1NextSet(stmt1Next, stmt1Next + 1);
	CPPUNIT_ASSERT(stmt1->getNext() == stmt1NextSet);

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	int stmt2Next[] = {3, 5};
	int stmt2Prev[] = {1, 4};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 2);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 2);
	CPPUNIT_ASSERT(stmt2->getNext() == stmt2NextSet);
	CPPUNIT_ASSERT(stmt2->getPrev() == stmt2PrevSet);

	Statement* stmt3 = stmtTable1->getStmtObj(3);
	int stmt3Next[] = {4};
	int stmt3Prev[] = {2};
	unordered_set<int> stmt3NextSet(stmt3Next, stmt3Next + 1);
	unordered_set<int> stmt3PrevSet(stmt3Prev, stmt3Prev + 1);
	CPPUNIT_ASSERT(stmt3->getNext() == stmt3NextSet);
	CPPUNIT_ASSERT(stmt3->getPrev() == stmt3PrevSet);

	Statement* stmt4 = stmtTable1->getStmtObj(4);
	int stmt4Next[] = {2};
	int stmt4Prev[] = {3};
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 1);
	unordered_set<int> stmt4PrevSet(stmt4Prev, stmt4Prev + 1);
	CPPUNIT_ASSERT(stmt4->getNext() == stmt4NextSet);
	CPPUNIT_ASSERT(stmt4->getPrev() == stmt4PrevSet);
	
	Statement* stmt5 = stmtTable1->getStmtObj(5);
	int stmt5Prev[] = {2};
	unordered_set<int> stmt5PrevSet(stmt5Prev, stmt5Prev + 1);
	CPPUNIT_ASSERT(stmt5->getNext().empty());
	CPPUNIT_ASSERT(stmt5->getPrev() == stmt5PrevSet);
}

void FrontEndTest::testCFGWhileNested() {
	/*
	procedure proc {								1	
		x = 2;										|
		while x {						   ------->	2 -----> END
			y = 3;						  |			|
			z = 3;						  |		   3,4
			while y {					  |			|
				while z {				  |		--- 5 <-
					y = 3;				  |		|	|  /
				}						  |		|	6 <--
			}							  |		|	|	|
										  |		|	7----	
			while z {					  | 	|	
				z = 4;					  | 	--> 8 <--	
			}							  | 	  /	|	|
		}								  <------/	9 ---
	}
	*/

	parser.parse("procedure proc {x = 2; while x {y = 3; z = 3; while y {while z {y = 3;}} while z{z = 4;}}}");

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	int stmt2Next[] = {3};
	int stmt2Prev[] = {1, 8};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 1);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 2);
	CPPUNIT_ASSERT(stmt2->getNext() == stmt2NextSet);
	CPPUNIT_ASSERT(stmt2->getPrev() == stmt2PrevSet);

	Statement* stmt5 = stmtTable1->getStmtObj(5);
	int stmt5Next[] = {6, 8};
	int stmt5Prev[] = {4, 6};
	unordered_set<int> stmt5NextSet(stmt5Next, stmt5Next + 2);
	unordered_set<int> stmt5PrevSet(stmt5Prev, stmt5Prev + 2);
	CPPUNIT_ASSERT(stmt5->getNext() == stmt5NextSet);
	CPPUNIT_ASSERT(stmt5->getPrev() == stmt5PrevSet);

	Statement* stmt6 = stmtTable1->getStmtObj(6);
	int stmt6Next[] = {7, 5};
	int stmt6Prev[] = {5, 7};
	unordered_set<int> stmt6NextSet(stmt6Next, stmt6Next + 2);
	unordered_set<int> stmt6PrevSet(stmt6Prev, stmt6Prev + 2);
	CPPUNIT_ASSERT(stmt6->getNext() == stmt6NextSet);
	CPPUNIT_ASSERT(stmt6->getPrev() == stmt6PrevSet);

	Statement* stmt7 = stmtTable1->getStmtObj(7);
	int stmt7Next[] = {6};
	int stmt7Prev[] = {6};
	unordered_set<int> stmt7NextSet(stmt7Next, stmt7Next + 1);
	unordered_set<int> stmt7PrevSet(stmt7Prev, stmt7Prev + 1);
	CPPUNIT_ASSERT(stmt7->getNext() == stmt7NextSet);
	CPPUNIT_ASSERT(stmt7->getPrev() == stmt7PrevSet);

	Statement* stmt8 = stmtTable1->getStmtObj(8);
	int stmt8Next[] = {9, 2};
	int stmt8Prev[] = {5, 9};
	unordered_set<int> stmt8NextSet(stmt8Next, stmt8Next + 2);
	unordered_set<int> stmt8PrevSet(stmt8Prev, stmt8Prev + 2);
	CPPUNIT_ASSERT(stmt8->getNext() == stmt8NextSet);
	CPPUNIT_ASSERT(stmt8->getPrev() == stmt8PrevSet);
}

void FrontEndTest::testCFGMixedNested() {
	/*
	procedure proc {
		x = 2;
		while x {
			if x then  {
				y = 3;
			} else {
				y = 4;
			}
		}

		if y then {
			while x {
		 		z = 3;
			}
		} else {
			while y {
				z = 4;
				while z {
					a = 5;
				}
			}
		}
	}
	*/

	parser.parse("procedure proc {x = 2; while x {if x then {y = 3;} else {y = 4;}} if y then {while x {z = 3;}} else {while y {z = 4; while z {a = 5;}}} b = 4;}");

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	int stmt2Next[] = {3, 6};
	int stmt2Prev[] = {1, 4, 5};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 2);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 3);
	CPPUNIT_ASSERT(stmt2->getNext() == stmt2NextSet);
	CPPUNIT_ASSERT(stmt2->getPrev() == stmt2PrevSet);

	Statement* stmt3 = stmtTable1->getStmtObj(3);
	int stmt3Next[] = {4, 5};
	int stmt3Prev[] = {2};
	unordered_set<int> stmt3NextSet(stmt3Next, stmt3Next + 2);
	unordered_set<int> stmt3PrevSet(stmt3Prev, stmt3Prev + 1);
	CPPUNIT_ASSERT(stmt3->getNext() == stmt3NextSet);
	CPPUNIT_ASSERT(stmt3->getPrev() == stmt3PrevSet);

	Statement* stmt4 = stmtTable1->getStmtObj(4);
	Statement* stmt5 = stmtTable1->getStmtObj(5);
	int stmt4Next[] = {2};
	int stmt4Prev[] = {3};
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 1);
	unordered_set<int> stmt4PrevSet(stmt4Prev, stmt4Prev + 1);
	CPPUNIT_ASSERT(stmt4->getNext() == stmt4NextSet);
	CPPUNIT_ASSERT(stmt4->getPrev() == stmt4PrevSet);
	CPPUNIT_ASSERT(stmt5->getNext() == stmt4NextSet);
	CPPUNIT_ASSERT(stmt5->getPrev() == stmt4PrevSet);

	Statement* stmt6 = stmtTable1->getStmtObj(6);
	int stmt6Next[] = {7, 9};
	int stmt6Prev[] = {2};
	unordered_set<int> stmt6NextSet(stmt6Next, stmt6Next + 2);
	unordered_set<int> stmt6PrevSet(stmt6Prev, stmt6Prev + 1);
	CPPUNIT_ASSERT(stmt6->getNext() == stmt6NextSet);
	CPPUNIT_ASSERT(stmt6->getPrev() == stmt6PrevSet);

	Statement* stmt7 = stmtTable1->getStmtObj(7);
	int stmt7Next[] = {8, 13};
	int stmt7Prev[] = {6, 8};
	unordered_set<int> stmt7NextSet(stmt7Next, stmt7Next + 2);
	unordered_set<int> stmt7PrevSet(stmt7Prev, stmt7Prev + 2);
	CPPUNIT_ASSERT(stmt7->getNext() == stmt7NextSet);
	CPPUNIT_ASSERT(stmt7->getPrev() == stmt7PrevSet);

	Statement* stmt8 = stmtTable1->getStmtObj(8);
	int stmt8Next[] = {7};
	unordered_set<int> stmt8Set(stmt8Next, stmt8Next + 1);
	CPPUNIT_ASSERT(stmt8->getNext() == stmt8->getPrev() && stmt8->getNext() == stmt8Set);

	Statement* stmt9 = stmtTable1->getStmtObj(9);
	int stmt9Next[] = {10, 13};
	int stmt9Prev[] = {6, 11};
	unordered_set<int> stmt9NextSet(stmt9Next, stmt9Next + 2);
	unordered_set<int> stmt9PrevSet(stmt9Prev, stmt9Prev + 2);
	CPPUNIT_ASSERT(stmt9->getNext() == stmt9NextSet);
	CPPUNIT_ASSERT(stmt9->getPrev() == stmt9PrevSet);

	Statement* stmt11 = stmtTable1->getStmtObj(11);
	int stmt11Next[] = {12, 9};
	int stmt11Prev[] = {10, 12};
	unordered_set<int> stmt11NextSet(stmt11Next, stmt11Next + 2);
	unordered_set<int> stmt11PrevSet(stmt11Prev, stmt11Prev + 2);
	CPPUNIT_ASSERT(stmt11->getNext() == stmt11NextSet);
	CPPUNIT_ASSERT(stmt11->getPrev() == stmt11PrevSet);

	Statement* stmt12 = stmtTable1->getStmtObj(12);
	int stmt12Next[] = {11};
	unordered_set<int> stmt12Set(stmt12Next, stmt12Next + 1);
	CPPUNIT_ASSERT(stmt12->getNext() == stmt12Set);
	CPPUNIT_ASSERT(stmt12->getPrev() == stmt12Set);

	Statement* stmt13 = stmtTable1->getStmtObj(13);
	int stmt13Prev[] = {7, 9};
	unordered_set<int> stmt13PrevSet(stmt13Prev, stmt13Prev + 2);
	CPPUNIT_ASSERT(stmt13->getPrev() == stmt13PrevSet);
	CPPUNIT_ASSERT(stmt13->getNext().empty());
}
