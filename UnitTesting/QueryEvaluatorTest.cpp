#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/FollowsStarClause.h"
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
#include "../SPA/Constant.h"

using namespace stringconst;
using namespace std;

void QueryEvaluatorTest::setUp() {
	/* testing this source

	procedure chocs {
		a=4;
		while i {
			k = 3;
			while j {
				i=1;
				j=2;
			}
			b=5;
		}	
	}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("chocs");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	AssgNode* assg1 = new AssgNode(1);
	VarNode* a1 = new VarNode("a");
	assg1->linkVarNode(a1);
	assg1->linkExprNode(new ConstNode("4"));
	procsl->linkStmtNode(assg1);

	WhileNode* while1 = new WhileNode(2);
	VarNode* i1 = new VarNode("i");
	while1->linkVarNode(i1);
	StmtLstNode* whilesl1 = new StmtLstNode();
	while1->linkStmtLstNode(whilesl1);
	procsl->linkStmtNode(while1);

	AssgNode* assg2 = new AssgNode(3);
	VarNode* k1 = new VarNode("k");
	assg2->linkVarNode(k1);
	assg2->linkExprNode(new ConstNode("3"));
	whilesl1->linkStmtNode(assg2);

	WhileNode* while2 = new WhileNode(4);
	VarNode* j1 = new VarNode("j");
	while2->linkVarNode(j1);
	StmtLstNode* whilesl2 = new StmtLstNode();
	while2->linkStmtLstNode(whilesl2);
	whilesl1->linkStmtNode(while2);

	AssgNode* assg3 = new AssgNode(5);
	VarNode* i2 = new VarNode("i");
	assg3->linkVarNode(i2);
	assg3->linkExprNode(new ConstNode("1"));
	whilesl2->linkStmtNode(assg3);

	AssgNode* assg4 = new AssgNode(6);
	VarNode* j2 = new VarNode("j");
	assg4->linkVarNode(j2);
	assg4->linkExprNode(new ConstNode("4"));
	whilesl2->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(7);
	VarNode* b1 = new VarNode("b");
	assg5->linkVarNode(b1);
	assg5->linkExprNode(new ConstNode("5"));
	whilesl1->linkStmtNode(assg5);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string modifiesArray1[] = {"a"};
	set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	stmt2->setFollowsBefore(1);
	string modifiesArray2[] = {"k", "i", "j", "b"};
	set<string> mods2(modifiesArray2, modifiesArray2 + 4);
	string usesArray2[] = {"i", "j"};
	set<string> uses2(usesArray2, usesArray2 + 2);
	stmt2->setModifies(mods2);
	stmt2->setUses(uses2);
	stmt2->setTNodeRef(while1);
	int children2[] = {3, 4, 7};
	stmt2->setChildren(set<int>(children2, children2+3));
	stable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsAfter(4);
	set<string> mods3 = set<string>();
	mods3.emplace("k");
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg2);
	stmt3->setParent(2);
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(WHILE_STMT_);
	stmt4->setFollowsBefore(3);
	stmt4->setFollowsAfter(7);
	set<string> mods4 = set<string>();
	mods4.emplace("i");
	mods4.emplace("j");
	set<string> uses4 = set<string>();
	uses4.emplace("j");
	stmt4->setModifies(mods4);
	stmt4->setUses(uses4);
	stmt4->setTNodeRef(while2);
	stmt4->setParent(2);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsAfter(6);
	set<string> mods5= set<string>();
	mods5.emplace("i");
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(4);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	set<string> mods6= set<string>();
	mods6.emplace("j");
	stmt6->setModifies(mods6);
	stmt6->setTNodeRef(assg4);
	stmt6->setParent(4);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	stmt7->setFollowsBefore(4);
	set<string> mods7= set<string>();
	mods7.emplace("b");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg5);
	stmt7->setParent(2);
	stable->addStmt(stmt7);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* va = new Variable("a");
	va->addModifyingStmt(1);
	va->addTNode(a1);
	vtable->addVariable(va);

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(2);
	vi->addModifyingStmt(5);
	vi->addUsingStmt(2);
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addModifyingStmt(4);
	vj->addModifyingStmt(6);
	vj->addUsingStmt(4);
	vj->addTNode(j1);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(2);
	vk->addModifyingStmt(3);
	vk->addTNode(k1);
	vtable->addVariable(vk);

	Variable* vb = new Variable("b");
	vb->addModifyingStmt(2);
	vb->addModifyingStmt(7);
	vb->addTNode(b1);
	vtable->addVariable(vb);
}

void QueryEvaluatorTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

// Test single clauses
void QueryEvaluatorTest::testEvaluator() {
	QueryEvaluator e = *new QueryEvaluator();

	FollowsClause* fol = new FollowsClause();
	fol->setFirstArg("a1");
	fol->setSecondArg("a2");
	fol->setFirstArgFixed(false);
	fol->setSecondArgFixed(false);
	fol->setFirstArgType(ARG_ASSIGN);
	fol->setSecondArgType(ARG_ASSIGN);
	
	StringPair p = *new StringPair();
	p.setFirst("a2");
	p.setSecond(ARG_STATEMENT);

	Query q = *new Query();
	q.addSelectSynonym(p);
	q.addClause(fol);

	set<string> res = e.evaluateQuery(q);
	
	CPPUNIT_ASSERT(res.size() == 1);

	//test ParentStar FixedSyn With While
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("w");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("5");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_WHILE);
	
	StringPair p2 = *new StringPair();
	p2.setFirst("w");
	p2.setSecond(ARG_WHILE);

	Query q2 = *new Query();
	q2.addSelectSynonym(p2);
	q2.addClause(m1);

	Results r = m1->evaluate();

	set<string> res2 = e.evaluateQuery(q2);

	CPPUNIT_ASSERT(res2.size() == 2);

	//test ParentStar FixedSyn With While
	ParentStarClause* m2 = new ParentStarClause();
	m2->setFirstArg("w1");
	m2->setFirstArgFixed(false);
	m2->setFirstArgType(ARG_STATEMENT);
	m2->setSecondArg("5");
	m2->setSecondArgFixed(true);
	m2->setSecondArgType(ARG_WHILE);
	
	StringPair p3 = *new StringPair();
	p3.setFirst("w");
	p3.setSecond(ARG_WHILE);

	Query q3 = *new Query();
	q3.addSelectSynonym(p3);
	q3.addClause(m2);

	set<string> res3 = e.evaluateQuery(q3);
	
	CPPUNIT_ASSERT(res3.size() == 2);

	// Test FollowsStar
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("3");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("a");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_STATEMENT);

	StringPair p4 = *new StringPair();
	p4.setFirst("a");
	p4.setSecond(ARG_STATEMENT);

	Query q4 = *new Query();
	q4.addSelectSynonym(p4);
	q4.addClause(f1);

	set<string> res4 = e.evaluateQuery(q4);

	CPPUNIT_ASSERT(res4.size() == 2);

	// Test FollowsStar with select type constant
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("3");
	f2->setFirstArgFixed(true);
	f2->setFirstArgType(ARG_STATEMENT);
	f2->setSecondArg("a");
	f2->setSecondArgFixed(false);
	f2->setSecondArgType(ARG_STATEMENT);

	StringPair p5 = *new StringPair();
	p5.setFirst("c");
	p5.setSecond(ARG_CONSTANT);

	Query q5 = *new Query();
	q5.addSelectSynonym(p5);
	q5.addClause(f2);

	set<string> res5 = e.evaluateQuery(q5);

	for (set<string>::iterator iter=res5.begin(); iter != res5.end(); iter++) {
		cout << "result: " << *iter << "!";
	}
	// to check
	CPPUNIT_ASSERT(res4.size() == 2);

}

// Test 2 clauses
void QueryEvaluatorTest::testEvaluator2() {
	QueryEvaluator e = *new QueryEvaluator();
	// Test all syn different 
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("w");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_WHILE);
	m1->setSecondArg("5");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);

	PatternAssgClause* p1 = new PatternAssgClause("a");
	p1->setVar("_");
	p1->setVarFixed(true);
	p1->setExpression("_");

	StringPair pr1 = *new StringPair();
	pr1.setFirst("s");
	pr1.setSecond(ARG_ASSIGN);

	Query q1 = *new Query();
	q1.addSelectSynonym(pr1);
	q1.addClause(m1);
	q1.addClause(p1);

	set<string> res = e.evaluateQuery(q1);
	
	CPPUNIT_ASSERT(res.size() == 5);

	// Test 1 same fixed syn between 2 clauses
	// Select a s.t. Modifies(5, "i") pattern a("i",_)
	ModifiesClause* m2 = new ModifiesClause();
	m2->setFirstArg("5");
	m2->setFirstArgFixed(true);
	m2->setFirstArgType(ARG_STATEMENT);
	m2->setSecondArg("i");
	m2->setSecondArgFixed(true);
	m2->setSecondArgType(ARG_VARIABLE);

	PatternAssgClause* p2 = new PatternAssgClause("a");
	p2->setVar("i");
	p2->setVarFixed(true);
	p2->setExpression("_");

	StringPair pr2 = *new StringPair();
	pr2.setFirst("a");
	pr2.setSecond(ARG_VARIABLE);

	Query q2 = *new Query();
	q2.addSelectSynonym(pr2);
	q2.addClause(m2);
	q2.addClause(p2);

	set<string> res2 = e.evaluateQuery(q2);

	CPPUNIT_ASSERT(res2.size() == 5);

	// Test 1 same unfixed syn between 2 clauses
	// Select a s.t. Modifies(a,v1) pattern a(v2,_)
	ModifiesClause* m3 = new ModifiesClause();
	m3->setFirstArg("a");
	m3->setFirstArgFixed(false);
	m3->setFirstArgType(ARG_STATEMENT);
	m3->setSecondArg("v1");
	m3->setSecondArgFixed(false);
	m3->setSecondArgType(ARG_VARIABLE);

	PatternAssgClause* p3 = new PatternAssgClause("a");
	p3->setVar("v2");
	p3->setVarFixed(false);
	p3->setExpression("_");

	StringPair pr3 = *new StringPair();
	pr3.setFirst("a");
	pr3.setSecond(ARG_VARIABLE);

	Query q3 = *new Query();
	q3.addSelectSynonym(pr3);
	q3.addClause(m3);
	q3.addClause(p3);

	set<string> res3 = e.evaluateQuery(q3);
	
	CPPUNIT_ASSERT(res3.size() == 5);

	// Test 2 same unfixed syn between 2 clauses
	// Select a s.t. Modifies(a,v) pattern a(v,_)
	ModifiesClause* m4 = new ModifiesClause();
	m4->setFirstArg("a");
	m4->setFirstArgFixed(false);
	m4->setFirstArgType(ARG_STATEMENT);
	m4->setSecondArg("v");
	m4->setSecondArgFixed(false);
	m4->setSecondArgType(ARG_VARIABLE);

	PatternAssgClause* p4 = new PatternAssgClause("a");
	p4->setVar("v");
	p4->setVarFixed(false);
	p4->setExpression("_");

	StringPair pr4 = *new StringPair();
	pr4.setFirst("a");
	pr4.setSecond(ARG_STATEMENT);

	Query q4 = *new Query();
	q4.addSelectSynonym(pr4);
	q4.addClause(m4);
	q4.addClause(p4);

	set<string> res4 = e.evaluateQuery(q4);
	
	CPPUNIT_ASSERT(res4.size() == 5);

	// Test 2 same unfixed syn between 2 clauses
	// Select v s.t. Follows*(a1,a2) pattern a2(v,_)
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("a1");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("a2");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	PatternAssgClause* p5 = new PatternAssgClause("a2");
	p5->setVar("v");
	p5->setVarFixed(false);
	p5->setExpression("_");
	CPPUNIT_ASSERT(p5->isValid());

	StringPair pr5 = *new StringPair();
	pr5.setFirst("v");
	pr5.setSecond(ARG_VARIABLE);

	Query q5 = *new Query();
	q5.addSelectSynonym(pr5);
	q5.addClause(f1);
	q5.addClause(p5);

	set<string> res5 = e.evaluateQuery(q5);
	
	CPPUNIT_ASSERT(res5.size() == 2);

	// Test 2 same unfixed syn between 2 clauses
	// Select v s.t. Follows*(a1,a2) pattern a2(v,_)
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("a1");
	f2->setFirstArgFixed(false);
	f2->setFirstArgType(ARG_STATEMENT);
	f2->setSecondArg("a2");
	f2->setSecondArgFixed(false);
	f2->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	PatternAssgClause* p6 = new PatternAssgClause("a2");
	p6->setVar("v");
	p6->setVarFixed(false);
	p6->setExpression("_");
	CPPUNIT_ASSERT(p6->isValid());

	StringPair pr6 = *new StringPair();
	pr6.setFirst("c");
	pr6.setSecond(ARG_CONSTANT);

	Query q6 = *new Query();
	q6.addSelectSynonym(pr6);
	q6.addClause(f1);
	q6.addClause(p5);

	set<string> res6 = e.evaluateQuery(q6);
	// To be checked
	CPPUNIT_ASSERT(res6.size() == 2);
}

