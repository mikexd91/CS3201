#include <cppunit/config/SourcePrefix.h>
#include "ModifiesClauseTest.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/IfNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/ProcTable.h"
#include "../SPA/StmtLstNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/WhileNode.h"
#include "../SPA/Utils.h"

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;

// note: while stmt not tested yet

void ModifiesClauseTest::setUp() {
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
	unordered_set<string> mods10;
	mods10.insert("z");
	stmt10->setModifies(mods10);
	stmt10->setTNodeRef(assg10);
	stable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	stmt11->setStmtNum(11);
	stmt11->setType(ASSIGN_STMT_);
	unordered_set<string> mods11;
	mods11.insert("y");
	stmt11->setModifies(mods11);
	stmt11->setTNodeRef(assg11);
	stable->addStmt(stmt11);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vy = new Variable("y");
	vy->addModifyingProc("zumba");
	vy->addModifyingStmt(9);
	vy->addModifyingStmt(11);
	vy->addTNode(y11);
	vtable->addVariable(vy);

	Variable* vz = new Variable("z");
	vz->addModifyingProc("zumba");
	vz->addModifyingStmt(9);
	vz->addModifyingStmt(10);
	vz->addTNode(z10);
	vtable->addVariable(vz);

	Variable* vi = new Variable("i");
	vi->addModifyingProc("zumba");
	vi->addUsingProc("zumba");
	vi->addModifyingStmt(1);
	vi->addModifyingStmt(6);
	vi->addUsingStmt(4);
	vi->addUsingStmt(6);
	vi->addTNode(i1);
	vi->addTNode(i4);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addUsingProc("zumba");
	vj->addModifyingProc("zumba");
	vj->addModifyingStmt(2);
	vj->addUsingStmt(7);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addUsingProc("zumba");
	vk->addModifyingProc("zumba");
	vk->addModifyingStmt(3);
	vk->addUsingStmt(5);
	vk->addTNode(k3);
	vtable->addVariable(vk);

	Variable* vw = new Variable("w");
	vw->addUsingProc("zumba");
	vw->addModifyingProc("zumba");
	vw->addModifyingStmt(4);
	vw->addUsingStmt(5);
	vw->addUsingStmt(9);
	vw->addTNode(w4);
	vtable->addVariable(vw);

	Variable* vx = new Variable("x");
	vx->addModifyingStmt(5);
	vx->addModifyingStmt(7);
	vx->addModifyingStmt(8);
	vx->addModifyingProc("zumba");
	vx->addTNode(x5);
	vtable->addVariable(vx);


	// set procedure for modifies
	ProcTable* procTable = ProcTable::getInstance();
	Procedure* procedure = new Procedure("zumba");
	string procUsesArr[] = {"i", "w", "k", "j"};
	unordered_set<string> procUses(procUsesArr, procUsesArr + 4);
	string procModsArr[] = {"i", "j", "k", "w", "x", "z", "y"};
	unordered_set<string> procModifies(procModsArr, procModsArr + 7);
	procedure->setUses(procUses);
	procedure->setModifies(procModifies);
	procTable->addProc(procedure);
}

void ModifiesClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ModifiesClauseTest );

void ModifiesClauseTest::testFixedFixedStmtPass() {
	Results* result = new Results();
	ModifiesClause* m1 = new ModifiesClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("i");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(m1->isValid());

	bool evalResult = m1->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedFixedStmtFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("3");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("i");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedFixedProcPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("zumba");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("w");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedFixedProcFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("zumba");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("a");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testGenericGenericPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("_");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_GENERIC);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testGenericFixedPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("_");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_GENERIC);
	mod->setSecondArg("i");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testGenericFixedFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("_");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_GENERIC);
	mod->setSecondArg("f");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedGenericStmtPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("7");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedGenericStmtFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("15");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedGenericProcPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("zumba");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedGenericProcFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("hello");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedSynStmtPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("2");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
}

void ModifiesClauseTest::testFixedSynStmtFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("12");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testFixedSynProcPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("zumba");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 7);
}

void ModifiesClauseTest::testFixedSynProcFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("hello");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testGenericSynPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("_");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_GENERIC);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 7);
}

void ModifiesClauseTest::testSynFixedStmtPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("s");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("x");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 3);
}

void ModifiesClauseTest::testSynFixedStmtFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("s");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("a");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testSynFixedProcPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("p");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("z");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
}

void ModifiesClauseTest::testSynFixedProcFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("p");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("l");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testSynFixedWhilePass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("w");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_WHILE);
	mod->setSecondArg("x");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
}

void ModifiesClauseTest::testSynFixedWhileFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("w");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_WHILE);
	mod->setSecondArg("j");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testSynFixedIfPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("if");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_IF);
	mod->setSecondArg("z");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
}

void ModifiesClauseTest::testSynFixedIfFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("if");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_IF);
	mod->setSecondArg("a");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void ModifiesClauseTest::testSynFixedAssgPass() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("a");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_ASSIGN);
	mod->setSecondArg("x");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 2);
}

void ModifiesClauseTest::testSynFixedAssgFail() {
	Results* result = new Results();
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("s");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_ASSIGN);
	mod->setSecondArg("a");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(mod->isValid());

	bool evalResult = mod->evaluate(result);
	CPPUNIT_ASSERT(evalResult == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}
