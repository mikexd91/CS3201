#include <cppunit/config/SourcePrefix.h>
#include "FollowsClauseTest.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/StmtTable.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/Clause.h"
#include "../SPA/Utils.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/WhileNode.h"
#include "../SPA/IfNode.h"
#include "../SPA/CallNode.h"

#include <iostream>
#include <boost/foreach.hpp>

using namespace stringconst;
using namespace std;

void FollowsClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		i = 1+2;	//1
		j = 2+3+4;	//2
		k = 3;		//3
		w = i;		//4
		x = w+k;	//5
		i = i;		//6
		while j {	//7
			x = 4;	//8
		}
		if w then { //9
			z = 2;	//10
		} else {
			y = 6;	//11
			x = 9;  //12
		}
	}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("zumba");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	AssgNode* assg1 = new AssgNode(1);
	VarNode* i1 = new VarNode("i");
	assg1->linkVarNode(i1);
	OpNode* plus1 = new OpNode("+");
	ConstNode* one1 = new ConstNode("1");
	ConstNode* two1 = new ConstNode("2");
	plus1->linkLeftNode(one1);
	plus1->linkRightNode(two1);
	assg1->linkExprNode(plus1);
	procsl->linkStmtNode(assg1);

	AssgNode* assg2 = new AssgNode(2);
	VarNode* j2 = new VarNode("j");
	OpNode* plus2_1 = new OpNode("+");
	ConstNode* four2 = new ConstNode("4");
	OpNode* plus2_2 = new OpNode("+");
	ConstNode* three2 = new ConstNode("3");
	ConstNode* two2 = new ConstNode("2");
	plus2_2->linkRightNode(three2);
	plus2_2->linkLeftNode(two2);
	plus2_1->linkRightNode(four2);
	plus2_1->linkLeftNode(plus2_2);
	assg2->linkVarNode(j2);
	assg2->linkExprNode(plus2_1);
	procsl->linkStmtNode(assg2);

	AssgNode* assg3 = new AssgNode(3);
	VarNode* k3 = new VarNode("k");
	ConstNode* three3 = new ConstNode("3");
	assg3->linkVarNode(k3);
	assg3->linkExprNode(three3);
	procsl->linkStmtNode(assg3);

	AssgNode* assg4 = new AssgNode(4);
	VarNode* w4 = new VarNode("w");
	assg4->linkVarNode(w4);
	VarNode* i4 = new VarNode("i");
	assg4->linkExprNode(i4);
	procsl->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(5);
	VarNode* x5 = new VarNode("x");
	assg5->linkVarNode(x5);
	OpNode* plus5 = new OpNode("+");
	VarNode* w5 = new VarNode("w");
	VarNode* k5 = new VarNode("k");
	plus5->linkLeftNode(w5);
	plus5->linkRightNode(k5);
	assg5->linkExprNode(plus5);
	procsl->linkStmtNode(assg5);

	AssgNode* assg6 = new AssgNode(6);
	VarNode* i6 = new VarNode("i");
	assg6->linkVarNode(i6);
	VarNode* i6_2 = new VarNode("i");
	assg6->linkExprNode(i6_2);
	procsl->linkStmtNode(assg6);

	WhileNode* whileNode = new WhileNode(7);
	VarNode* j7 = new VarNode("j");
	whileNode->linkVarNode(j7);
	StmtLstNode* whileStmtLst = new StmtLstNode();
	whileNode->linkStmtLstNode(whileStmtLst);
	procsl->linkStmtNode(whileNode);

	AssgNode* assg8 = new AssgNode(8);
	VarNode* x8 = new VarNode("x");
	assg8->linkVarNode(x8);
	whileStmtLst->linkChild(assg8);

	IfNode* ifNode = new IfNode(9);
	VarNode* w9 = new VarNode("w");
	ifNode->linkVarNode(w9);
	StmtLstNode* thenStmtLst = new StmtLstNode();
	ifNode->linkThenStmtLstNode(thenStmtLst);
	procsl->linkStmtNode(ifNode);

	AssgNode* assg10 = new AssgNode(10);
	VarNode* z10 = new VarNode("z");
	assg10->linkVarNode(z10);
	thenStmtLst->linkChild(assg10);

	StmtLstNode* elseStmtLst = new StmtLstNode();
	ifNode->linkElseStmtLstNode(elseStmtLst);

	AssgNode* assg11 = new AssgNode(11);
	VarNode* y11 = new VarNode("y");
	assg11->linkVarNode(y11);
	elseStmtLst->linkChild(assg11);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	int followsStarArr1[] = {2, 3, 4, 5, 6, 7, 9};
	unordered_set<int> followsStarAfter1(followsStarArr1, followsStarArr1 + 7);
	stmt1->setFollowsStarAfter(followsStarAfter1);
	string ivar = "i";
	unordered_set<string> mods1;
	mods1.emplace(ivar);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(3);
	int followsStarBeforeArr2[] = {1};
	unordered_set<int> followsStarBefore2(followsStarBeforeArr2, followsStarBeforeArr2 + 1);
	stmt2->setFollowsStarBefore(followsStarBefore2);
	int followsStarAfterArr2[] = {3, 4, 5, 6, 7, 9};
	unordered_set<int> followsStarAfter2(followsStarAfterArr2, followsStarAfterArr2 + 6);
	stmt2->setFollowsStarAfter(followsStarAfter2);
	string jvar = "j";
	unordered_set<string> mods2;
	mods2.emplace(jvar);
	stmt2->setModifies(mods2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2); 
	stmt3->setFollowsAfter(4);
	int followsStarBeforeArr3[] = {1, 2};
	unordered_set<int> followsStarBefore3(followsStarBeforeArr3, followsStarBeforeArr3 + 2);
	stmt3->setFollowsStarBefore(followsStarBefore3);
	int followsStarAfterArr3[] = {4, 5, 6, 7, 9};
	unordered_set<int> followsStarAfter3(followsStarAfterArr3, followsStarAfterArr3 + 5);
	stmt3->setFollowsStarAfter(followsStarAfter3);
	string kvar = "k";
	unordered_set<string> mods3;
	mods3.emplace(kvar);
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg3);
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	stmt4->setFollowsBefore(3);
	stmt4->setFollowsAfter(5);
	int followsStarBeforeArr4[] = {1, 2, 3};
	unordered_set<int> followsStarBefore4(followsStarBeforeArr4, followsStarBeforeArr4 + 3);
	stmt4->setFollowsStarBefore(followsStarBefore4);
	int followsStarAfterArr4[] = {5, 6, 7, 9};
	unordered_set<int> followsStarAfter4(followsStarAfterArr4, followsStarAfterArr4 + 4);
	stmt4->setFollowsStarAfter(followsStarAfter4);
	string wvar = "w";
	unordered_set<string> mods4;
	mods4.emplace(wvar);
	stmt4->setModifies(mods4);
	unordered_set<string> uses4;
	uses4.emplace(ivar);
	stmt4->setUses(uses4);
	stmt4->setTNodeRef(assg4);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsBefore(4);
	stmt5->setFollowsAfter(6);
	int followsStarBeforeArr5[] = {1, 2, 3, 4};
	unordered_set<int> followsStarBefore5(followsStarBeforeArr5, followsStarBeforeArr5 + 4);
	stmt5->setFollowsStarBefore(followsStarBefore5);
	int followsStarAfterArr5[] = {6, 7, 9};
	unordered_set<int> followsStarAfter5(followsStarAfterArr5, followsStarAfterArr5 + 3);
	stmt5->setFollowsStarAfter(followsStarAfter5);
	string xvar = "x";
	unordered_set<string> mods5;
	mods5.emplace(xvar);
	stmt5->setModifies(mods5);
	unordered_set<string> uses5;
	uses5.emplace(wvar);
	uses5.emplace(kvar);
	stmt5->setUses(uses5);
	stmt5->setTNodeRef(assg5);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	stmt6->setFollowsAfter(7);
	int followsStarBeforeArr6[] = {1, 2, 3, 4, 5};
	unordered_set<int> followsStarBefore6(followsStarBeforeArr6, followsStarBeforeArr6 + 5);
	stmt6->setFollowsStarBefore(followsStarBefore6);
	int followsStarAfterArr6[] = {7, 9};
	unordered_set<int> followsStarAfter6(followsStarAfterArr6, followsStarAfterArr6 + 2);
	stmt6->setFollowsStarAfter(followsStarAfter6);
	unordered_set<string> mods6;
	mods6.emplace(ivar);
	stmt6->setModifies(mods6);
	unordered_set<string> uses6;
	uses6.emplace(ivar);
	stmt6->setUses(uses6);
	stmt6->setTNodeRef(assg6);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(WHILE_STMT_);
	stmt7->setFollowsBefore(6);
	stmt7->setFollowsAfter(9);
	int followsStarBeforeArr7[] = {1, 2, 3, 4, 5, 6};
	unordered_set<int> followsStarBefore7(followsStarBeforeArr7, followsStarBeforeArr7 + 6);
	stmt7->setFollowsStarBefore(followsStarBefore7);
	int followsStarAfterArr7[] = {9};
	unordered_set<int> followsStarAfter7(followsStarAfterArr7, followsStarAfterArr7 + 1);
	stmt7->setFollowsStarAfter(followsStarAfter7);
	unordered_set<string> mods7;
	mods7.emplace(xvar);
	stmt7->setModifies(mods7);
	unordered_set<string> uses7;
	uses7.emplace(jvar);
	stmt7->setUses(uses7);
	stmt7->setTNodeRef(whileNode);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	unordered_set<string> mods8;
	mods8.emplace(xvar);
	stmt8->setModifies(mods8);
	stmt8->setTNodeRef(assg8);
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(IF_STMT_);
	stmt9->setFollowsBefore(7);
	int followsStarBeforeArr9[] = {1, 2, 3, 4, 5, 6, 7};
	unordered_set<int> followsStarBefore9(followsStarBeforeArr9, followsStarBeforeArr9 + 7);
	stmt9->setFollowsStarBefore(followsStarBefore9);
	unordered_set<string> mods9;
	mods9.emplace("z");
	mods9.emplace("y");
	unordered_set<string> uses9;
	uses9.emplace("w");
	stmt9->setModifies(mods9);
	stmt9->setUses(uses9);
	stable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	stmt10->setStmtNum(10);
	stmt10->setType(ASSIGN_STMT_);
	stmt10->setFollowsAfter(11);
	int followsStarAfterArr10[] = {11};
	unordered_set<int> followsStarAfter10(followsStarAfterArr10, followsStarAfterArr10 + 1);
	stmt10->setFollowsStarAfter(followsStarAfter10);
	unordered_set<string> mods10;
	mods10.insert("z");
	stmt10->setModifies(mods10);
	stmt10->setTNodeRef(assg10);
	stable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	stmt11->setStmtNum(11);
	stmt11->setType(ASSIGN_STMT_);
	stmt11->setFollowsBefore(10);
	stmt11->setFollowsAfter(12);
	int followsStarBeforeArr11[] = {10};
	unordered_set<int> followsStarBefore11(followsStarBeforeArr11, followsStarBeforeArr11 + 1);
	stmt11->setFollowsStarBefore(followsStarBefore11);
	int followsStarAfterArr11[] = {12};
	unordered_set<int> followsStarAfter11(followsStarAfterArr11, followsStarAfterArr11 + 1);
	stmt11->setFollowsStarAfter(followsStarAfter11);
	unordered_set<string> mods11;
	mods11.insert("y");
	stmt11->setModifies(mods11);
	stmt11->setTNodeRef(assg11);
	stable->addStmt(stmt11);

	Statement* stmt12 = new Statement();
	stmt12->setStmtNum(12);
	stmt12->setType(ASSIGN_STMT_);
	stmt12->setFollowsBefore(11);
	int followsStarBeforeArr12[] = {10, 11};
	unordered_set<int> followsStarBefore12(followsStarBeforeArr12, followsStarBeforeArr12 + 2);
	stmt12->setFollowsStarBefore(followsStarBefore12);
	unordered_set<string> mods12;
	mods12.insert("x");
	stmt12->setModifies(mods12);
	stable->addStmt(stmt12);
}

void FollowsClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( FollowsClauseTest );
void FollowsClauseTest::testFollowsFixedFixedPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "9");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsFixedFixedFailWrongOrder() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "3");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "2");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsFixedFixedFailNestingLevel() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "8");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsFixedFixedFailSameStmt() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "1");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "1");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsSynFixedPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "a");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "3");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 1);
}

void FollowsClauseTest::testFollowsSynFixedFail() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "a");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "1");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
	CPPUNIT_ASSERT(result.isSynPresent("a") == false);
}

void FollowsClauseTest::testFollowsFixedSynPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "1");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "a");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 1);
	CPPUNIT_ASSERT(result.isSynPresent("a") == true);
}

void FollowsClauseTest::testFollowsFixedSynFail() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "12");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "a");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
	CPPUNIT_ASSERT(result.isSynPresent("a") == false);
}
void FollowsClauseTest::testFollowsSynSynPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "b");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "a");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 9);
	CPPUNIT_ASSERT(result.isSynPresent("a") == true);
	CPPUNIT_ASSERT(result.isSynPresent("b") == true);
}

void FollowsClauseTest::testFollowsSynSynSameFail() {
	/*
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "b");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "b");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
	*/
}

void FollowsClauseTest::testFollowsFirstUnderscoreFixedPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "9");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsFirstUnderscoreSynPass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "bal");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 9);
}

void FollowsClauseTest::testFollowsFixedSecondUnderscorePass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "11");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsSynSecondUnderscorePass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "a");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 9);
}

void FollowsClauseTest::testFollowsBothUnderscorePass() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}

void FollowsClauseTest::testFollowsStmtOverflow() {
	Result result = Result();
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	
	followsBuilder->setArg(1, "-1");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "1");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	
	FollowsClause* c1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());
	
	bool evalResult = c1->evaluate(&result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result.getResultTableSize() == 0);
}
