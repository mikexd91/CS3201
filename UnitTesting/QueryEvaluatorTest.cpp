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
#include "../SPA/WithClause.h"
#include "../SPA/WithClauseRef.h"
#include "../SPA/WithClauseBuilder.h"
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
	VarTable::getInstance()->reset();
	ProcTable::getInstance()->clearTable();
	ConstTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

void QueryEvaluatorTest::testNoResult(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_IF);
	TEST_Q->addSelectSynonym(SELECT_1);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_IF);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "2");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_R->getResultTableSize() == 0);
	CPPUNIT_ASSERT(TEST_PRINT.size() == 0);
}

void QueryEvaluatorTest::testBoolNoResult(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("BOOLEAN");
	SELECT_1.setSecond(stringconst::ARG_BOOLEAN);
	TEST_Q->addSelectSynonym(SELECT_1);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_IF);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "2");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_R->getResultTableSize() == 0);
	CPPUNIT_ASSERT(TEST_PRINT.find("false") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testBoolNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("BOOLEAN");
	SELECT_1.setSecond(stringconst::ARG_BOOLEAN);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("false") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testStmtNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("7") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("11") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testCallNoClause(){string asd = "no meaningful tests in this source";}

void QueryEvaluatorTest::testAssNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("a");
	SELECT_1.setSecond(stringconst::ARG_ASSIGN);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("7") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("11") != TEST_PRINT.end());

	delete optimisedDV;
	delete TEST_Q;
	delete qe;
	delete TEST_R;
}

void QueryEvaluatorTest::testIfNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("i");
	SELECT_1.setSecond(stringconst::ARG_IF);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("7") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("11") == TEST_PRINT.end());
}

void QueryEvaluatorTest::testWhileNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("w");
	SELECT_1.setSecond(stringconst::ARG_WHILE);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("7") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("11") == TEST_PRINT.end());
}

void QueryEvaluatorTest::testProcNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("p");
	SELECT_1.setSecond(stringconst::ARG_PROCEDURE);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("zumba") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testConstNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("c");
	SELECT_1.setSecond(stringconst::ARG_CONSTANT);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") == TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testVarNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("v");
	SELECT_1.setSecond(stringconst::ARG_VARIABLE);
	TEST_Q->addSelectSynonym(SELECT_1);
	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);

	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("i") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("j") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("k") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("w") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("x") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("z") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("y") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testBoolOneClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("BOOLEAN");
	SELECT_1.setSecond(stringconst::ARG_BOOLEAN);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, INTEGER_);
	withBuilder->setEntity(1, "1");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("true") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testBoolMoreClauses(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("BOOLEAN");
	SELECT_1.setSecond(stringconst::ARG_BOOLEAN);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, INTEGER_);
	withBuilder->setEntity(1, "1");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("true") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testSynNotInClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("a");
	SELECT_1.setSecond(stringconst::ARG_ASSIGN);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("1") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("11") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testSynInOneClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "s");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testSynInMoreClauses(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "s");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s1");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") == TEST_PRINT.end());
}

void QueryEvaluatorTest::testSynInSomeClauses(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "s");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s2");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s1");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") == TEST_PRINT.end());
}

void QueryEvaluatorTest::testTupleNoClause(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);

	StringPair SELECT_2 = StringPair();
	SELECT_2.setFirst("s1");
	SELECT_2.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_2);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s1");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	int size = TEST_Q->getClauseList().size();
	optimisedDV->push_back(size);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("10 10") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("9 9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("8 8") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("7 7") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("6 6") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("5 5") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("4 4") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("3 3") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("2 2") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("1 1") != TEST_PRINT.end());
}

void QueryEvaluatorTest::testOptEval(){
	Query* TEST_Q = new Query();
	StringPair SELECT_1 = StringPair();
	SELECT_1.setFirst("s");
	SELECT_1.setSecond(stringconst::ARG_STATEMENT);
	TEST_Q->addSelectSynonym(SELECT_1);
	
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "s");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* TEST_PC = (ParentClause*) parentBuilder->build();
	TEST_Q->addClause((Clause*)TEST_PC);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s2");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s1");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);
	WithClause* TEST_WC = withBuilder->build();
	TEST_Q->addClause(TEST_WC);

	vector<int>* optimisedDV = new vector<int>();
	optimisedDV->push_back(1);
	optimisedDV->push_back(1);
	QueryEvaluator* qe = new QueryEvaluator();
	Result* TEST_R = qe->evalOptimisedQuery(TEST_Q, optimisedDV);
	unordered_set<string> TEST_PRINT = qe->printValues(TEST_R, TEST_Q->getSelectList());
	CPPUNIT_ASSERT(TEST_PRINT.find("9") != TEST_PRINT.end());
	CPPUNIT_ASSERT(TEST_PRINT.find("10") == TEST_PRINT.end());
}

void QueryEvaluatorTest::testSingleResultCopy(){
	QueryEvaluator* qe = new QueryEvaluator();

	Result* R1 = new Result();
	SingleSynInsert INSERT_1 = SingleSynInsert();
	INSERT_1.setSyn("s");
	INSERT_1.insertValue("1");
	INSERT_1.insertValue("2");
	INSERT_1.insertValue("3");
	INSERT_1.insertValue("4");
	R1->push(INSERT_1);

	unordered_set<string> P1 = R1->getSyn("s");
	//cout << endl << "start A" << endl;
	//BOOST_FOREACH(string s, P1){
	//	cout << s << endl;
	//}
	//cout << "end A" << endl;

	Result* R2 = new Result();
	qe->copyResult(R1, R2, "s");

	unordered_set<string> P2 = R2->getSyn("s");
	//cout << endl << "start B" << endl;
	//BOOST_FOREACH(string s, P2){
	//	cout << s << endl;
	//}
	//cout << "end B" << endl;

	Result* R3 = new Result();
	SingleSynInsert INSERT_3 = SingleSynInsert();
	INSERT_3.setSyn("s");
	INSERT_3.insertValue("2");
	INSERT_3.insertValue("4");
	R3->push(INSERT_3);

	qe->copyResult(R3, R2, "s");
	//P2 = R2->getSyn("s");
	//cout << endl << "start C" << endl;
	//BOOST_FOREACH(string s, P2){
	//	cout << s << endl;
	//}
	//cout << "end C" << endl;
}

void QueryEvaluatorTest::testTupleResultCopy(){
	QueryEvaluator* qe = new QueryEvaluator();

	Result* R1 = new Result();
	SingleSynInsert INSERT_1 = SingleSynInsert();
	INSERT_1.setSyn("s");
	INSERT_1.insertValue("1");
	INSERT_1.insertValue("2");
	INSERT_1.insertValue("3");
	R1->push(INSERT_1);
	SingleSynInsert INSERT_2 = SingleSynInsert();
	INSERT_2.setSyn("a");
	INSERT_2.insertValue("A");
	INSERT_2.insertValue("B");
	INSERT_2.insertValue("C");
	R1->push(INSERT_2);

	vector<StringPair> syns = vector<StringPair>();
	StringPair syn1 = StringPair();
	StringPair syn2 = StringPair();
	syn1.setFirst("s");
	syn2.setFirst("a");
	syns.push_back(syn1);
	syns.push_back(syn2);
	unordered_set<string> P1 = qe->printValues(R1, syns);
	//cout << endl << "start A" << endl;
	//int i = 0;
	//BOOST_FOREACH(string s, P1){
	//	i++;
	//	cout << i << ": " << s << endl;
	//}
	//cout << "end A" << endl;
	//i = 0;

	vector<string> synlist = vector<string>();
	synlist.push_back("s");
	synlist.push_back("a");
	Result* R2 = new Result();
	qe->mergeMultiResult(R1, R2, synlist);
	unordered_set<string> P2 = qe->printValues(R2, syns);
	//cout << endl << "start B" << endl;
	//BOOST_FOREACH(string s, P2){
	//	i++;
	//	cout << i << ": " << s << endl;
	//}
	//cout << "end B" << endl;
}

void QueryEvaluatorTest::testSingleResultMerge(){
	
	QueryEvaluator* qe = new QueryEvaluator();

	Result* R1 = new Result();
	Result* R2 = new Result();
	SingleSynInsert I1 = SingleSynInsert();
	SingleSynInsert I2 = SingleSynInsert();
	I1.setSyn("s");
	I2.setSyn("s");

	I1.insertValue("1");
	I1.insertValue("2");
	I1.insertValue("3");
	I1.insertValue("4");
	I1.insertValue("5");
	I1.insertValue("6");

	I2.insertValue("3");
	I2.insertValue("4");
	I2.insertValue("5");
	I2.insertValue("6");
	I2.insertValue("7");
	I2.insertValue("8");

	R1->push(I1);
	R2->push(I2);

	Result* RF = new Result();
	qe->copyResult(R1, RF, "s");
	qe->copyResult(R2, RF, "s");
	
	//int i=0;
	//cout << endl << "START" << endl;
	//BOOST_FOREACH(string s, RF->getSyn("s")){
	//	i++;
	//	cout << i << ": " << s << endl;
	//}
	//cout << "END" << endl;
}

void QueryEvaluatorTest::testTupleResultMerge(){

	QueryEvaluator* qe = new QueryEvaluator();

	Result* R1 = new Result();
	Result* R2 = new Result();
	Result* RF = new Result();
	SingleSynInsert I11 = SingleSynInsert();
	SingleSynInsert I12 = SingleSynInsert();
	SingleSynInsert I21 = SingleSynInsert();
	SingleSynInsert I22 = SingleSynInsert();
	I11.setSyn("s");
	I12.setSyn("a");
	I21.setSyn("s");
	I22.setSyn("a");

	I11.insertValue("1");
	I11.insertValue("2");
	I11.insertValue("3");

	I12.insertValue("A");
	I12.insertValue("B");
	I12.insertValue("C");

	I21.insertValue("3");
	I21.insertValue("4");
	I21.insertValue("5");

	I22.insertValue("A");
	I22.insertValue("B");
	I22.insertValue("C");

	R1->push(I11);
	R1->push(I12);

	R2->push(I21);
	R2->push(I22);
	
	vector<StringPair> syns = vector<StringPair>();
	StringPair syn1 = StringPair();
	StringPair syn2 = StringPair();
	syn1.setFirst("s");
	syn2.setFirst("a");
	syns.push_back(syn1);
	syns.push_back(syn2);

	vector<string> synlist = vector<string>();
	synlist.push_back("s");
	synlist.push_back("a");

	unordered_set<string> P1 = qe->printValues(R1, syns);
	//cout << endl << "start R1" << endl;
	int i = 0;
	BOOST_FOREACH(string s, P1){
		i++;
		//cout << i << ": " << s << endl;
	}
	//cout << "end R1" << endl;
	i = 0;

	unordered_set<string> P2 = qe->printValues(R2, syns);
	//cout << endl << "start R2" << endl;
	BOOST_FOREACH(string s, P2){
		i++;
		//cout << i << ": " << s << endl;
	}
	//cout << "end R2" << endl;
	i = 0;

	qe->mergeMultiResult(R1, RF, synlist);
	qe->mergeMultiResult(R2, RF, synlist);

	unordered_set<string> PF = qe->printValues(RF, syns);
	//cout << endl << "start RF" << endl;
	BOOST_FOREACH(string s, PF){
		i++;
		//cout << i << ": " << s << endl;
	}
	//cout << "end RF" << endl;
	i = 0;
}