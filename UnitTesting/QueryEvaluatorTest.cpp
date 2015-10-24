 #include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/ParentClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/Result.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/ProcTable.h"
#include "../SPA/Utils.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/WhileNode.h"
#include "../SPA/IfNode.h"
#include "../SPA/ConstTable.h"
#include "../SPA/CallNode.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/Clause.h"
#include <boost\foreach.hpp>

using namespace stringconst;
using namespace std;

void QueryEvaluatorTest::setUp() {
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
	stmt3->setFollowsAfter(4);
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
	int children7[] = {8};
	stmt7->setChildren(unordered_set<int>(children7, children7+1));
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
	stmt8->setParent(7);
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(IF_STMT_);
	stmt9->setFollowsBefore(7);
	int children9[] = {10, 11};
	stmt9->setChildren(unordered_set<int>(children9, children9+2));
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
	stmt10->setParent(9);
	stable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	stmt11->setStmtNum(11);
	stmt11->setType(ASSIGN_STMT_);
	unordered_set<string> mods11;
	mods11.insert("y");
	stmt11->setModifies(mods11);
	stmt11->setTNodeRef(assg11);
	stmt11->setParent(9);
	stable->addStmt(stmt11);

	// to set up the constTable manually
	ConstTable* constTable = ConstTable::getInstance();

	Constant* c1 = new Constant("1");
	int lineNumberArr1[] = {1};
	unordered_set<int> constSet1(lineNumberArr1, lineNumberArr1 + 1);
	c1->setAppearsIn(constSet1);
	constTable->addConst(c1);

	Constant* c2 = new Constant("2");
	int lineNumberArr2[] = {1, 2, 10};
	unordered_set<int> constSet2(lineNumberArr2, lineNumberArr2 + 3);
	c2->setAppearsIn(constSet2);
	constTable->addConst(c2);

	Constant* c3 = new Constant("3");
	int lineNumberArr3[] = {2, 3};
	unordered_set<int> constSet3(lineNumberArr3, lineNumberArr3 + 2);
	c3->setAppearsIn(constSet3);
	constTable->addConst(c3);

	Constant* c4 = new Constant("4");
	int lineNumberArr4[] = {2, 8};
	unordered_set<int> constSet4(lineNumberArr4, lineNumberArr4 + 2);
	c4->setAppearsIn(constSet4);
	constTable->addConst(c4);

	Constant* c6 = new Constant("6");
	int lineNumberArr5[] = {11};
	unordered_set<int> constSet5(lineNumberArr5, lineNumberArr5 + 1);
	c6->setAppearsIn(constSet5);
	constTable->addConst(c6);

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

void QueryEvaluatorTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

void QueryEvaluatorTest::testEvaluateEmptySelectBoolean() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "false");

}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectAssignSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("a");
	p->setSecond(ARG_ASSIGN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("a") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 9);

	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 9);

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectStmtSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("b");
	p->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("b") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectWhileSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("c");
	p->setSecond(ARG_WHILE);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("c") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectIfSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("d");
	p->setSecond(ARG_IF);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("d") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectCallSyn() {
	
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("e");
	p->setSecond(ARG_CALL);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("e") == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	
	delete qe;
	delete p;
	delete q;
	delete result;
	
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectVarSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("g");
	p->setSecond(ARG_VARIABLE);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("g") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectProcSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("g");
	p->setSecond(ARG_PROCEDURE);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("g") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectConstSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("g");
	p->setSecond(ARG_CONSTANT);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("g") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}


void QueryEvaluatorTest::testModifiesEvaluateFixedSynProcPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("v");
	p->setSecond(ARG_VARIABLE);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "zumba");
	modifiesBuilder->setArgFixed(1, true);
	modifiesBuilder->setArgType(1, ARG_PROCEDURE);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m1 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	/*
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("zumba");
	mod->setFirstArgFixed(true);
	mod->setFirstArgType(ARG_PROCEDURE);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	*/
	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("v") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testModifiesEvaluateSynFixedWhilePass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("w");
	p->setSecond(ARG_WHILE);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "w");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_WHILE);
	modifiesBuilder->setArg(2, "x");
	modifiesBuilder->setArgFixed(2, true);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m1 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	/*
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("w");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_WHILE);
	mod->setSecondArg("x");
	mod->setSecondArgFixed(true);
	mod->setSecondArgType(ARG_VARIABLE);
	*/
	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("w") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testModifiesEvaluateSynSynAssgPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p1 = new StringPair();
	p1->setFirst("a");
	p1->setSecond(ARG_ASSIGN);
	StringPair *p2 = new StringPair();
	p2->setFirst("v");
	p2->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*p1);
	q->addSelectSynonym(*p2);
	
	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "a");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_ASSIGN);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m1 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	/*
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("a");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_ASSIGN);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	*/
	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("a") == true);
	CPPUNIT_ASSERT(result->isSynPresent("v") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 9);

	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 9);

	delete qe;
	delete p1;
	delete p2;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFollowsEvaluateSynSynStmtPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p1 = new StringPair();
	p1->setFirst("s1");
	p1->setSecond(ARG_STATEMENT);
	StringPair *p2 = new StringPair();
	p2->setFirst("s2");
	p2->setSecond(ARG_STATEMENT);

	Query *q = new Query();
	q->addSelectSynonym(*p1);
	q->addSelectSynonym(*p2);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "s1");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "s2");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("s1") == true);
	CPPUNIT_ASSERT(result->isSynPresent("s2") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);

	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	/*
	for (unordered_set<string>::iterator iter = toPrint.begin(); iter != toPrint.end(); ++iter) {
		cout << *iter << "|";
	}
	*/
	CPPUNIT_ASSERT(toPrint.size() == 7);

	delete qe;
	delete p1;
	delete p2;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testHalfInClauseWithModifiesSynSynStmtPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	// Synonym in clause
	StringPair *p1 = new StringPair();
	p1->setFirst("s");
	p1->setSecond(ARG_STATEMENT);
	StringPair *p2 = new StringPair();
	p2->setFirst("v");
	p2->setSecond(ARG_VARIABLE);

	//Synonym not in clause
	StringPair *n1 = new StringPair();
	n1->setFirst("s1");
	n1->setSecond(ARG_STATEMENT);
	StringPair *n2 = new StringPair();
	n2->setFirst("v1");
	n2->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*p1);
	q->addSelectSynonym(*p2);
	q->addSelectSynonym(*n1);
	q->addSelectSynonym(*n2);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "s");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_STATEMENT);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m1 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	/*
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("s");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("v");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_VARIABLE);
	*/
	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("s") == true);
	CPPUNIT_ASSERT(result->isSynPresent("v") == true);
	CPPUNIT_ASSERT(result->isSynPresent("s1") == true);
	CPPUNIT_ASSERT(result->isSynPresent("v1") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 924);
	
	delete qe;
	delete p1;
	delete p2;
	delete n1;
	delete n2;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testModifiesEvaluateSynGenericStmtPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("p");
	p->setSecond(ARG_PROCEDURE);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	
	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "s");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_STATEMENT);
	modifiesBuilder->setArg(2, "_");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_GENERIC);
	ModifiesClause* m1 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	
	/*
	ModifiesClause* mod = new ModifiesClause();
	mod->setFirstArg("s");
	mod->setFirstArgFixed(false);
	mod->setFirstArgType(ARG_STATEMENT);
	mod->setSecondArg("_");
	mod->setSecondArgFixed(false);
	mod->setSecondArgType(ARG_GENERIC);
	*/
	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->isSynPresent("p") == true);
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFollowsEvaluateFixedFixedPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_WHILE);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "true");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFollowsEvaluateFixedFixedFail() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "9");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	q->addClause((Clause*) m1);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "false");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testUnderscoreFixedPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "3");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "_");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_GENERIC);
	parentBuilder->setArg(2, "8");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m2 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "true");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testUnderscoreFixedFail() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "3");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "_");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_GENERIC);
	parentBuilder->setArg(2, "12");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m2 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "false");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFixedUnderscorePass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "_");
	parentBuilder->setArgFixed(2, false);
	parentBuilder->setArgType(2, ARG_GENERIC);
	ParentClause* m2 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "7");
	modifiesBuilder->setArgFixed(1, true);
	modifiesBuilder->setArgType(1, ARG_STATEMENT);
	modifiesBuilder->setArg(2, "_");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_GENERIC);
	ModifiesClause* m3 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);
	q->addClause((Clause*) m3);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "true");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFixedUnderscoreFail() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_ASSIGN);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "10");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "_");
	parentBuilder->setArgFixed(2, false);
	parentBuilder->setArgType(2, ARG_GENERIC);
	ParentClause* m2 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "false");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testUnderscoreUnderscorePass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m1 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "_");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_GENERIC);
	parentBuilder->setArg(2, "_");
	parentBuilder->setArgFixed(2, false);
	parentBuilder->setArgType(2, ARG_GENERIC);
	ParentClause* m2 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "true");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFixedFixedPass() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m1 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "7");
	modifiesBuilder->setArgFixed(1, true);
	modifiesBuilder->setArgType(1, ARG_WHILE);
	modifiesBuilder->setArg(2, "x");
	modifiesBuilder->setArgFixed(2, true);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m2 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "6");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_ASSIGN);
	followsBuilder->setArg(2, "7");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_WHILE);
	FollowsClause* m3 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);
	q->addClause((Clause*) m3);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "true");

	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testFixedFixedFail() {
	QueryEvaluator *qe = new QueryEvaluator();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m1 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "7");
	modifiesBuilder->setArgFixed(1, true);
	modifiesBuilder->setArgType(1, ARG_WHILE);
	modifiesBuilder->setArg(2, "j");
	modifiesBuilder->setArgFixed(2, true);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m2 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "6");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_ASSIGN);
	followsBuilder->setArg(2, "7");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_WHILE);
	FollowsClause* m3 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());

	q->addClause((Clause*) m1);
	q->addClause((Clause*) m2);
	q->addClause((Clause*) m3);

	Result* result = qe->evaluateQuery(q);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
	vector<StringPair> selectList = q->getSelectList();
	unordered_set<string> toPrint = qe->getValuesToPrint(result, selectList);
	CPPUNIT_ASSERT(toPrint.size() == 1);
	unordered_set<string>::iterator iter = toPrint.begin();
	CPPUNIT_ASSERT(*iter == "false");

	delete qe;
	delete p;
	delete q;
	delete result;
}
