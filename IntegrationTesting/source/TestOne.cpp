#include <cppunit/config/SourcePrefix.h>
#include "TestOne.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"

using namespace std;
// TEST VARIABLES

//ProcTable* procTable = ProcTable::getInstance();// test proctable instance
Parser parser;
PDR* pdr;
AST* ast;
VarTable* varTable1;
ProcTable* procTable;
StmtTable* stmtTable1;
ConstTable* constTable;

void TestOne::setUp() {
	pdr = PDR::getInstance();
	ast = AST::getInstance();
	constTable = ConstTable::getInstance();
	parser = Parser();
	varTable1 = VarTable::getInstance();
	procTable = ProcTable::getInstance();
	stmtTable1 = StmtTable::getInstance();

}

void TestOne::tearDown() {
	PDR::resetInstanceFlag();
	AST::reset();
	constTable->clearTable();
	VarTable::reset();
	procTable->clearTable();
	stmtTable1->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestOne );

// method to test adding of proc to table
void TestOne::testAddProc() {
	parser.parse("procedure test {x = 2; y=x+z; z=x+y+z; while i {y=x+1; a=a+b;} }");
	//ProcTable* procTable = ProcTable::getInstance();
	//StmtTable* stmtTable1 = StmtTable::getInstance();
	//VarTable* varTable1 = VarTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	
	Statement* stmt1 = stmtTable1->getStmtObj(1);
	string initUses1[] = { "x" };
	set<string> str1(initUses1, initUses1 + 1);
	CPPUNIT_ASSERT(stmt1->getModifies() == str1);
	
	Statement* stmt2 = stmtTable1->getStmtObj(2);
	string initModifies2[] = { "y" };
	set<string> strModifies2(initModifies2, initModifies2 + 1);
	CPPUNIT_ASSERT(stmt2->getModifies() == strModifies2);
	string initUses2[] = { "x", "z" };
	set<string> strUses2(initUses2, initUses2 + 2);
	CPPUNIT_ASSERT(stmt2->getUses() == strUses2);

	Statement* stmt3 = stmtTable1->getStmtObj(3);
	string initModifies3[] = { "z" };
	set<string> strModifies3(initModifies3, initModifies3 + 1);
	CPPUNIT_ASSERT(stmt3->getModifies() == strModifies3);
	string initUses3[] = { "x", "y", "z" };
	set<string> strUses3(initUses3, initUses3 + 3);
	CPPUNIT_ASSERT(stmt3->getUses() == strUses3);

	Statement* stmt4 = stmtTable1->getStmtObj(4);
	string initModifies4[] = { "y" };
	set<string> strModifies4(initModifies4, initModifies4 + 1);
	//CPPUNIT_ASSERT(stmt4->getModifies() == strModifies4);
	string initUses4[] = { "x", "i" };
	set<string> strUses4(initUses4, initUses4 + 1);
	//CPPUNIT_ASSERT(stmt4->getUses() == strUses4);
	int initChildren4[] = {5, 6};
	set<int> setChildren4(initChildren4, initChildren4 + 2);
	CPPUNIT_ASSERT(stmt4->getChildren() == setChildren4);

	Statement* stmt5 = stmtTable1->getStmtObj(5);
	string initModifies5[] = { "y" };
	set<string> strModifies5(initModifies5, initModifies5 + 1);
	CPPUNIT_ASSERT(stmt5->getModifies() == strModifies5);
	string initUses5[] = { "x" };
	set<string> strUses5(initUses5, initUses5 + 1);
	CPPUNIT_ASSERT(stmt5->getUses() == strUses5);
	CPPUNIT_ASSERT(stmt5->getParent() == 4);
	
	
	Statement* stmt6 = stmtTable1->getStmtObj(6);
	string initModifies6[] = { "a" };
	set<string> strModifies6(initModifies6, initModifies6 + 1);
	CPPUNIT_ASSERT(stmt6->getModifies() == strModifies6);
	
	string initUses6[] = { "a", "b" };
	set<string> strUses6(initUses6, initUses6 + 2);
	CPPUNIT_ASSERT(stmt6->getUses() == strUses6);
	CPPUNIT_ASSERT(stmt6->getParent() == 4);

	Variable* varX = varTable1->getVariable("x");
	int initXUsedBy[] = {2, 3, 4, 5};
	int initXModifiedBy[] = {1};
	set<int> setXUsedBy(initXUsedBy, initXUsedBy + 4);
	set<int> setXModifiedBy(initXModifiedBy, initXModifiedBy + 1);
	CPPUNIT_ASSERT(varX->getUsedByStmts() == setXUsedBy);
	CPPUNIT_ASSERT(varX->getModifiedByStmts() == setXModifiedBy);

	Variable* varY = varTable1->getVariable("y");
	int initYUsedBy[] = {3};
	int initYModifiedBy[] = {2, 4, 5};
	set<int> setYUsedBy(initYUsedBy, initYUsedBy + 1);
	set<int> setYModifiedBy(initYModifiedBy, initYModifiedBy + 3);
	CPPUNIT_ASSERT(varY->getUsedByStmts() == setYUsedBy);
	CPPUNIT_ASSERT(varY->getModifiedByStmts() == setYModifiedBy);

	Variable* varZ = varTable1->getVariable("z");
	int initZUsedBy[] = {2, 3};
	int initZModifiedBy[] = {3};
	set<int> setZUsedBy(initZUsedBy, initZUsedBy + 2);
	set<int> setZModifiedBy(initZModifiedBy, initZModifiedBy + 1);
	CPPUNIT_ASSERT(varZ->getUsedByStmts() == setZUsedBy);
	CPPUNIT_ASSERT(varZ->getModifiedByStmts() == setZModifiedBy);
}



void TestOne::testAssign() {
	
	parser.parse("procedure test {x = 2; y=x;}");
	
	//StmtTable* stmtTable1 = StmtTable::getInstance();
	
	Statement* stmt = stmtTable1->getStmtObj(2);
	
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
	CPPUNIT_ASSERT(ast->getProcNode("whileTest")->getStmtLstNode()->getChildren().at(0)->getNodeType() == WHILE_STMT_);
	
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

void TestOne::testSiblingsAST() {
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

void TestOne::testMultipleProcAST() {
	parser.parse("procedure proc1{} procedure proc2{} procedure proc3{}");
	CPPUNIT_ASSERT(ast->contains("proc1"));
	CPPUNIT_ASSERT(ast->contains("proc2"));
	CPPUNIT_ASSERT(ast->contains("proc3"));

	ProcNode* proc1 = ast->getProcNode("proc1");
	ProcNode* proc2 = ast->getProcNode("proc2");
	ProcNode* proc3 = ast->getProcNode("proc3");

	CPPUNIT_ASSERT(proc1->getRightSibling() == proc2);
	CPPUNIT_ASSERT(proc2->getRightSibling() == proc3);
}

void TestOne::testFollows() {
	//StmtTable* stmtTable1 = StmtTable::getInstance();
	
	parser.parse("procedure proc{x = 2; y = x + 3; while y{z = y + x;} w = z + 2;}");
	CPPUNIT_ASSERT(ast->contains("proc"));
	
	Statement* firstAssg = stmtTable1->getStmtObj(1);
	Statement* secAssg = stmtTable1->getStmtObj(2);
	Statement* whileStmt = stmtTable1->getStmtObj(3);
	Statement* thirdAssg = stmtTable1->getStmtObj(4);
	Statement* fourthAssg = stmtTable1->getStmtObj(5);
	CPPUNIT_ASSERT(firstAssg->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(firstAssg->getFollowsAfter() == 2);
	CPPUNIT_ASSERT(secAssg->getFollowsBefore() == 1);
	CPPUNIT_ASSERT(secAssg->getFollowsAfter() == 3);
	CPPUNIT_ASSERT(whileStmt->getFollowsBefore() == 2);
	CPPUNIT_ASSERT(whileStmt->getFollowsAfter() == 5);
	CPPUNIT_ASSERT(thirdAssg->getFollowsBefore() == -1);
	CPPUNIT_ASSERT(thirdAssg->getFollowsAfter() == -1);
	CPPUNIT_ASSERT(fourthAssg->getFollowsBefore() == 3);
	CPPUNIT_ASSERT(fourthAssg->getFollowsAfter() == -1);
}

void TestOne::testWhileUses() {
	parser.parse("procedure proc { while w { while y { z = a + b; }} }");
	
	Statement* firstWhile = stmtTable1->getStmtObj(1);
	Statement* secWhile = stmtTable1->getStmtObj(2);
	
	string firstInit[] = {"a", "b", "w", "y"};
	set<string> firstUses(firstInit, firstInit + 4);
	CPPUNIT_ASSERT(firstWhile->getUses() == firstUses);

	string secInit[] = {"a", "b", "y"};
	set<string> secUses(secInit, secInit + 3);
	CPPUNIT_ASSERT(secWhile->getUses() == secUses);

}

void TestOne::testWhileModifies() {
	parser.parse("procedure proc { while x { while y {x = 2; y = 2; z = x + y;}}} ");
	
	Statement* firstWhile = stmtTable1->getStmtObj(1);
	Statement* secWhile = stmtTable1->getStmtObj(2);
	Statement* firstAssg = stmtTable1->getStmtObj(3);
	Statement* secAssg = stmtTable1->getStmtObj(4);
	Statement* thirdAssg = stmtTable1->getStmtObj(5);
	
	string modi[] = {"x", "y", "z"};
	set<string> modifiesSet(modi, modi + 3);
	CPPUNIT_ASSERT(firstWhile->getModifies() == modifiesSet);
	CPPUNIT_ASSERT(secWhile->getModifies() == modifiesSet);
	

	set<string> firstAssgModSet;
	firstAssgModSet.insert("x");
	CPPUNIT_ASSERT(firstAssg->getModifies() == firstAssgModSet);
	
	set<string> secondAssgModSet;
	secondAssgModSet.insert("y");
	CPPUNIT_ASSERT(secAssg->getModifies() == secondAssgModSet);

	set<string> thirdAssgModSet;
	thirdAssgModSet.insert("z");
	CPPUNIT_ASSERT(thirdAssg->getModifies() == thirdAssgModSet);
}

void TestOne::testStmtTableAllWhile() {
	parser.parse("procedure proc3 { while x {} while y{} while z{x = 2; while w {}} }");

	set<Statement*> whileStmts = stmtTable1->getWhileStmts();
	CPPUNIT_ASSERT(whileStmts.size() == 4);

	set<Statement*>::iterator iter;

	for(iter = whileStmts.begin(); iter != whileStmts.end(); iter++) {
		Statement* stmt = *iter;
		int stmtNum = stmt->getStmtNum();
		switch(stmtNum) {
			case 1: {
				string arrInit[] = {"x"};
				set<string> usesSet(arrInit, arrInit + 1);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 2: {
				string arrInit[] = {"y"};
				set<string> usesSet(arrInit, arrInit + 1);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 3: {
				string arrInit[] = {"w", "z"};
				set<string> usesSet(arrInit, arrInit + 2);
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			case 5: {
				string arrInit[] = {"w"};
				set<string> usesSet(arrInit, arrInit + 1);
				stmt->getUses();
				CPPUNIT_ASSERT(stmt->getUses() == usesSet);
				break;
			}
			default: 
				break;
		}
	}
}

void TestOne::testConstTable() {
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

void TestOne::testCallsAST() {
	parser.parse("procedure proc1{call proc2;} procedure proc2{x = 2;}");

	CPPUNIT_ASSERT(procTable->contains("proc1"));
	CPPUNIT_ASSERT(procTable->contains("proc2"));

	ProcNode* proc1Node = ast->getProcNode("proc1");
	vector<TNode*> proc1Children = proc1Node->getChildren();
	TNode* proc1StmtLstNode = proc1Children[0];

	vector<TNode*> stmtLstNodeChildren = proc1StmtLstNode->getChildren();
	CPPUNIT_ASSERT(stmtLstNodeChildren.size() == 1);

	TNode* call1Node = stmtLstNodeChildren[0];
	CPPUNIT_ASSERT(call1Node->getNodeType() == NodeType::CALL_STMT_);
	CPPUNIT_ASSERT(call1Node->getName() == "proc2");

	ProcNode* proc2Node = ast->getProcNode("proc2");
	vector<TNode*> proc2Children = proc2Node->getChildren();
	TNode* proc2StmtLstNode = proc2Children[0];

	vector<TNode*> stmtLstNode2Children = proc2StmtLstNode->getChildren();
	CPPUNIT_ASSERT(stmtLstNode2Children.size() == 1);
	
	TNode* assignNode = stmtLstNode2Children[0];
	CPPUNIT_ASSERT(assignNode->getNodeType() == NodeType::ASSIGN_STMT_);
	CPPUNIT_ASSERT(assignNode->getChildren().size() == 2);
}

void TestOne::testCallsPKB() {
	parser.parse("procedure proc1{call proc2;} procedure proc2{x = 2;}");

	Statement* stmt1 = stmtTable1->getStmtObj(1);
	CPPUNIT_ASSERT(stmt1->getType() == NodeType::CALL_STMT_);
	CPPUNIT_ASSERT(stmt1->getCalls() == "proc2");

	Statement* stmt2 = stmtTable1->getStmtObj(2);
	CPPUNIT_ASSERT(stmt2->getType() == NodeType::ASSIGN_STMT_);

	Procedure* procedure1 = procTable->getProcObj("proc1");
	Procedure* procedure2 = procTable->getProcObj("proc2");
	
	set<Procedure*> calledBy;
	calledBy.insert(procedure1);
	CPPUNIT_ASSERT(procedure2->getCalledBy() == calledBy);

	set<Procedure*> calls;
	calls.insert(procedure2);
	CPPUNIT_ASSERT(procedure1->getCalls() == calls);
}

void TestOne::testProcedureUses() {
	parser.parse("procedure proc1 { x = 2; y = x; } procedure proc2{ z = 3; y = 1; x = z + y;}");

	Procedure* procedure = procTable->getProcObj("proc1");
	string uses[] = {"x"};
	set<string> usesSet(uses, uses + 1);
	CPPUNIT_ASSERT(procedure->getUses() == usesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string uses2[] = {"z", "y"};
	set<string> usesSet2(uses2, uses2 + 2);
	CPPUNIT_ASSERT(procedure2->getUses() == usesSet2);
}

void TestOne::testProcedureModifies() {
	parser.parse("procedure proc1 {a = 3; b = a + 4;} procedure proc2 {x = 4; y = 2; z = x + y;}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string modifies1[] = {"a", "b"};
	set<string> modifiesSet1(modifies1, modifies1 + 2);
	CPPUNIT_ASSERT(procedure1->getModifies() == modifiesSet1);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string modifies2[] = {"x", "y", "z"};
	set<string> modifiesSet2(modifies2, modifies2 + 3);
	CPPUNIT_ASSERT(procedure2->getModifies() == modifiesSet2);
}

void TestOne::testNestedProceduresUses() {
	parser.parse("procedure proc1 {call proc2;} procedure proc2 {x = 2; while x {y = 2;}}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string uses[] = {"x"};
	set<string> usesSet(uses, uses + 1);
	CPPUNIT_ASSERT(procedure1->getUses() == usesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	CPPUNIT_ASSERT(procedure2->getUses() == usesSet);
}

void TestOne::testNestedProceduresModifies() {
	parser.parse("procedure proc1 { a = 2; call proc2;} procedure proc2 {x = 2; z = x; y = x + z;}");

	Procedure* procedure1 = procTable->getProcObj("proc1");
	string modifies[] = {"a", "x", "z", "y"};
	set<string> modifiesSet(modifies, modifies + 4);
	CPPUNIT_ASSERT(procedure1->getModifies() == modifiesSet);

	Procedure* procedure2 = procTable->getProcObj("proc2");
	string modifies2[] = {"x", "z", "y"};
	set<string> modifiesSet2(modifies2, modifies2 + 3);
	CPPUNIT_ASSERT(procedure2->getModifies() == modifiesSet2);
}