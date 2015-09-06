#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/Results.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/WhileNode.h"
#include "../SPA/IfNode.h"
#include "../SPA/ConstTable.h"
#include "../SPA/CallNode.h"
#include <boost\foreach.hpp>

using namespace stringconst;
using namespace std;

void QueryEvaluatorTest::setUp() {
	/* testing this source

	procedure chocs {
		while i {
			k = 3;
			while j {
				i=1+2;
				j=2+3+4;
			}
			if i {
				i=1;
			} else {
				call hello;
			}
		}	
		}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("chocs");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	WhileNode* while1 = new WhileNode(1);
	VarNode* i1 = new VarNode("i");
	while1->linkVarNode(i1);
	StmtLstNode* whilesl1 = new StmtLstNode();
	while1->linkStmtLstNode(whilesl1);
	procsl->linkStmtNode(while1);

	AssgNode* assg1 = new AssgNode(2);
	VarNode* k1 = new VarNode("k");
	assg1->linkVarNode(k1);
	assg1->linkExprNode(new ConstNode("3"));
	procsl->linkStmtNode(assg1);

	WhileNode* while2 = new WhileNode(3);
	VarNode* j1 = new VarNode("j");
	while2->linkVarNode(j1);
	StmtLstNode* whilesl2 = new StmtLstNode();
	while2->linkStmtLstNode(whilesl2);
	whilesl1->linkStmtNode(while2);

	AssgNode* assg2 = new AssgNode(4);
	VarNode* i2 = new VarNode("i");
	assg2->linkVarNode(i2);
	OpNode* plus1 = new OpNode("+");
	ConstNode* one1 = new ConstNode("1");
	ConstNode* two1 = new ConstNode("2");
	plus1->linkLeftNode(one1);
	plus1->linkRightNode(two1);
	assg2->linkExprNode(plus1);
	whilesl2->linkStmtNode(assg2);

	AssgNode* assg3 = new AssgNode(5);
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
	assg3->linkVarNode(j2);
	assg3->linkExprNode(plus2_1);
	whilesl2->linkStmtNode(assg3);

	IfNode* if1 = new IfNode(6);
	VarNode* i3 = new VarNode("i");
	if1->linkVarNode(i3);
	StmtLstNode* then1 = new StmtLstNode();
	if1->linkThenStmtLstNode(then1);
	StmtLstNode* else1 = new StmtLstNode();
	if1->linkElseStmtLstNode(else1);
	whilesl1->linkStmtNode(if1);

	AssgNode* assg4 = new AssgNode(7);
	VarNode* i4 = new VarNode("i");
	ConstNode* two3 = new ConstNode("2");
	assg3->linkVarNode(i4);
	assg3->linkExprNode(two3);
	then1->linkStmtNode(assg4);

	CallNode* call1 = new CallNode(8, "Hello");
	else1->linkStmtNode(call1);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(WHILE_STMT_);
	string modifiesArray1[] = {"k", "i", "j"};
	unordered_set<string> mods1(modifiesArray1, modifiesArray1 + 3);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(while1);
	int children1[] = {2, 3, 6};
	stmt1->setChildren(unordered_set<int>(children1, children1+3));
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsAfter(3);
	string kvar = "k";
	unordered_set<string> mods2 = unordered_set<string>();
	mods2.emplace(kvar);
	stmt2->setModifies(mods2);
	stmt2->setTNodeRef(assg1);
	stmt2->setParent(1);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(WHILE_STMT_);
	stmt3->setFollowsBefore(2);
	string modifiesArray3[] = {"i", "j"};
	unordered_set<string> mods3(modifiesArray1, modifiesArray1 + 2);
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg3);
	stmt3->setParent(1);
	int children3[] = {4, 5};
	stmt3->setChildren(unordered_set<int>(children3, children3+2));
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	stmt4->setFollowsAfter(5);
	string ivar = "i";
	unordered_set<string> mods4 = unordered_set<string>();
	mods4.emplace(ivar);
	stmt4->setModifies(mods4);
	stmt4->setTNodeRef(assg2);
	stmt4->setParent(3);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsBefore(4);
	string jvar = "j";
	unordered_set<string> mods5 = unordered_set<string>();
	mods5.emplace(jvar);
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(3);
	stable->addStmt(stmt5);
	
	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(IF_STMT_);
	stmt6->setFollowsBefore(3);
	unordered_set<string> uses6 = unordered_set<string>();
	uses6.emplace("i");
	stmt6->setUses(uses6);
	stmt6->setTNodeRef(if1);
	stmt6->setParent(1);
	unordered_set<int> children6 = unordered_set<int>();
	children6.insert(7);
	children6.insert(8);
	//int children6[] = {7, 8};
	stmt6->setChildren(children6);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	unordered_set<string> mods7 = unordered_set<string>();
	mods5.emplace("i");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg4);
	stmt7->setParent(6);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(CALL_STMT_);
	stmt8->setTNodeRef(call1);
	stmt8->setParent(6);
	stable->addStmt(stmt8);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addModifyingStmt(4);
	vi->addUsingStmt(1);
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(1);
	vj->addModifyingStmt(3);
	vj->addModifyingStmt(5);
	vj->addUsingStmt(3);
	vj->addTNode(j1);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(1);
	vk->addModifyingStmt(2);
	vk->addTNode(k1);
	vtable->addVariable(vk);
}

void QueryEvaluatorTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectAssignSyn() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("a");
	p->setSecond(ARG_ASSIGN);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("a") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 4);
	
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
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("b") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 8);
	
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
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("c") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 2);
	
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
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("d") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
	
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
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("e") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectVarSyn() {

}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectProcSyn() {
	/*
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("g");
	p->setSecond(ARG_PROCEDURE);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("g") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete result;
	*/
}

void QueryEvaluatorTest::testEvalauteEmptyClauseListSelectConstSyn() {
	/*
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("g");
	p->setSecond(ARG_CONSTANT);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::NONE_IN_CLAUSE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("g") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 7);
	
	delete qe;
	delete p;
	delete q;
	delete result;
	*/
}

void QueryEvaluatorTest::testEvaluateParentFixedFixedPass() {

}

void QueryEvaluatorTest::testEvaluateParentFixedFixedFail() {

}

void QueryEvaluatorTest::testEvaluateParentSynFixedPass() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("4");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	q->addClause(m1);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("s") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentSynFixedFail() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("1");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	q->addClause(m1);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("s") == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentFixedSynPass() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	q->addClause(m1);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("s") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 3);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentFixedSynPassWithWhile() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("w");
	p->setSecond(ARG_WHILE);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	q->addClause(m1);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("w") == true);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 1);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentFixedSynFail() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	q->addClause(m1);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("s") == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentFixedSynFailWithWhile() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p = new StringPair();
	p->setFirst("w");
	p->setSecond(ARG_WHILE);
	Query *q = new Query();
	q->addSelectSynonym(*p);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("4");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("w") == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentSynSynPass() {
	// Dont understand pn's code
}

void QueryEvaluatorTest::testEvaluateParentSynSynPassWithWhile() {
	QueryEvaluator *qe = new QueryEvaluator();
	
	StringPair *p1 = new StringPair();
	p1->setFirst("s1");
	p1->setSecond(ARG_STATEMENT);
	StringPair *p2 = new StringPair();
	p2->setFirst("w1");
	p2->setSecond(ARG_WHILE);

	Query *q = new Query();
	q->addSelectSynonym(*p1);
	q->addSelectSynonym(*p2);
	qe->setCategory(SynListConstants::ALL_IN_CLAUSE);

	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("4");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);

	Results* result = qe->evaluateQuery(*q);
	CPPUNIT_ASSERT(result->hasResults("w") == false);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
	
	delete qe;
	delete p;
	delete q;
	delete m1;
	delete result;
}

void QueryEvaluatorTest::testEvaluateParentSynSynPassWithIf() {

}

void QueryEvaluatorTest::testEvaluateParentFirstUnderscorePass() {

}

void QueryEvaluatorTest::testEvaluateParentSecondUnderscorePass() {

}

void QueryEvaluatorTest::testEvaluateParentBothUnderscorePass() {

}

void QueryEvaluatorTest::testEvaluateParentCallPass() {

}

