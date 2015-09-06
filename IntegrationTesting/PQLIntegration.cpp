#include <cppunit/config/SourcePrefix.h>
#include "PQLIntegration.h"
#include "../SPA/PQLController.h"
#include "../SPA/AST.h"
#include "../SPA/WhileNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/ConstTable.h"
#include "../SPA/IfNode.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <boost\foreach.hpp>
#include <boost\unordered_set.hpp>

using namespace stringconst;
using namespace std;
using boost::unordered_set;

void PQLIntegration::setUp() {
	/* testing this source

	procedure chocs {
		a=4;				//1
		while i {			//2
			k = 3;			//3
			while j {		//4
				i=1;		//5
				j=2; 		//6
			}
			b=5;			//7
			j=2+3+4;		//8
			z=(10+11)*12; 	//9
		}
		--- new stuff TODO ---
		if a then {			//10
		} else {
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
	ConstNode* c1 = new ConstNode("4");
	assg1->linkVarNode(a1);
	assg1->linkExprNode(c1);
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
	ConstNode* c2 = new ConstNode("3");
	assg2->linkExprNode(c2);
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
	ConstNode* c3 = new ConstNode("1");
	assg3->linkExprNode(c3);
	whilesl2->linkStmtNode(assg3);

	AssgNode* assg4 = new AssgNode(6);
	VarNode* j2 = new VarNode("j");
	assg4->linkVarNode(j2);
	ConstNode* c4 = new ConstNode("2");
	assg4->linkExprNode(c4);
	whilesl2->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(7);
	VarNode* b1 = new VarNode("b");
	assg5->linkVarNode(b1);
	ConstNode* c5 = new ConstNode("5");
	assg5->linkExprNode(c5);
	whilesl1->linkStmtNode(assg5);

	AssgNode* assg6 = new AssgNode(8);
	VarNode* j6 = new VarNode("j");
	OpNode* plus2_1 = new OpNode("+");
	ConstNode* four2 = new ConstNode("4");
	OpNode* plus2_2 = new OpNode("+");
	ConstNode* three2 = new ConstNode("3");
	ConstNode* two2 = new ConstNode("2");
	plus2_2->linkRightNode(three2);
	plus2_2->linkLeftNode(two2);
	plus2_1->linkRightNode(four2);
	plus2_1->linkLeftNode(plus2_2);
	assg6->linkVarNode(j6);
	assg6->linkExprNode(plus2_1);
	procsl->linkStmtNode(assg6);

	//z=(10+11)*12
	AssgNode* assg7 = new AssgNode(9);
	VarNode* j7 = new VarNode("z");
	OpNode* plus3_1 = new OpNode("+");
	ConstNode* c10 = new ConstNode("10");
	ConstNode* c11 = new ConstNode("11");
	OpNode* multiply = new OpNode("*");
	ConstNode* c12 = new ConstNode("12");
	plus3_1->linkRightNode(c11);
	plus3_1->linkLeftNode(c10);
	multiply->linkRightNode(c12);
	multiply->linkLeftNode(plus3_1);
	assg7->linkVarNode(j7);
	assg7->linkExprNode(multiply);
	procsl->linkStmtNode(assg7);

	/// if a then {} else {}	//10
	IfNode* if1 = new IfNode(10);
	VarNode* a2 = new VarNode("a");
	if1->linkVarNode(a2);
	StmtLstNode* thensl1 = new StmtLstNode();
	if1->linkThenStmtLstNode(thensl1);
	StmtLstNode* elsesl1 = new StmtLstNode();
	if1->linkElseStmtLstNode(elsesl1);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string modifiesArray1[] = {"a"};
	unordered_set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(10);
	string modifiesArray2[] = {"k", "i", "j", "b"};
	unordered_set<string> mods2(modifiesArray2, modifiesArray2 + 4);
	string usesArray2[] = {"i", "j"};
	unordered_set<string> uses2(usesArray2, usesArray2 + 2);
	stmt2->setModifies(mods2);
	stmt2->setUses(uses2);
	stmt2->setTNodeRef(while1);
	int children2[] = {3, 4, 7};
	stmt2->setChildren(unordered_set<int>(children2, children2+3));
	stable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsAfter(4);
	unordered_set<string> mods3 = unordered_set<string>();
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
	unordered_set<string> mods4 = unordered_set<string>();
	mods4.emplace("i");
	mods4.emplace("j");
	unordered_set<string> uses4 = unordered_set<string>();
	uses4.emplace("j");
	stmt4->setModifies(mods4);
	stmt4->setUses(uses4);
	unordered_set<int> children3 = *new unordered_set<int>();
	children3.emplace(5);
	children3.emplace(6);
	stmt4->setChildren(children3);
	stmt4->setTNodeRef(while2);
	stmt4->setParent(2);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsAfter(6);
	unordered_set<string> mods5= unordered_set<string>();
	mods5.emplace("i");
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(4);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	unordered_set<string> mods6= unordered_set<string>();
	mods6.emplace("j");
	stmt6->setModifies(mods6);
	stmt6->setTNodeRef(assg4);
	stmt6->setParent(4);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	stmt7->setFollowsBefore(4);
	stmt7->setFollowsAfter(8);
	unordered_set<string> mods7= unordered_set<string>();
	mods7.emplace("b");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg5);
	stmt7->setParent(2);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	unordered_set<string> mods8 = unordered_set<string>();
	mods8.emplace("j");
	stmt8->setModifies(mods8);
	stmt8->setFollowsBefore(7);
	stmt8->setFollowsAfter(9);
	stmt8->setTNodeRef(assg6);
	stmt8->setParent(2);
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(ASSIGN_STMT_);
	unordered_set<string> mods9 = unordered_set<string>();
	mods9.emplace("z");
	stmt9->setModifies(mods9);
	stmt9->setFollowsBefore(8);
	stmt9->setTNodeRef(assg7);
	stmt9->setParent(2);
	stable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	stmt10->setStmtNum(10);
	stmt10->setType(IF_STMT_);
	unordered_set<string> uses10 = unordered_set<string>();
	uses10.emplace("a");
	stmt10->setUses(uses10);
	stmt10->setFollowsBefore(2);
	stable->addStmt(stmt10);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* va = new Variable("a");
	va->addModifyingStmt(1);
	va->addUsingStmt(10);
	va->addUsingProc("chocs");
	va->addTNode(a1);
	vtable->addVariable(va);

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(2);
	vi->addModifyingStmt(5);
	vi->addUsingStmt(2);
	vi->addUsingProc("chocs");
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addModifyingStmt(4);
	vj->addModifyingStmt(6);
	vj->addModifyingStmt(8);
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

	ConstTable* ctable = ConstTable::getInstance();
	//Populate Constant Table
	Constant* const1 = new Constant();
	const1->addAppearsIn(5);
	const1->addTNodeRef(c3);
	const1->setConstName("1");
	ctable->addConst(const1);
	
	Constant* const2 = new Constant();
	const2->addAppearsIn(6);
	const2->addAppearsIn(8);
	const2->addTNodeRef(c4);
	const2->addTNodeRef(two2);
	const2->setConstName("2");
	ctable->addConst(const2);

	Constant* const3 = new Constant();
	const3->addAppearsIn(3);
	const3->addAppearsIn(8);
	const3->addTNodeRef(c2);
	const3->addTNodeRef(three2);
	const3->setConstName("3");
	ctable->addConst(const3);

	Constant* const4 = new Constant();
	const4->addAppearsIn(1);
	const4->addAppearsIn(8);
	const4->addTNodeRef(c1);
	const4->addTNodeRef(four2);
	const4->setConstName("4");
	ctable->addConst(const4);

	Constant* const5 = new Constant();
	const5->addAppearsIn(7);
	const5->addTNodeRef(c5);
	const5->setConstName("5");
	ctable->addConst(const5);
	
	Constant* const10 = new Constant();
	const10->addAppearsIn(9);
	const10->addTNodeRef(c10);
	const10->setConstName("10");
	ctable->addConst(const10);

	Constant* const11 = new Constant();
	const11->addAppearsIn(9);
	const11->addTNodeRef(c11);
	const11->setConstName("11");
	ctable->addConst(const11);
	
	Constant* const12 = new Constant();
	const12->addAppearsIn(9);
	const12->addTNodeRef(c12);
	const12->setConstName("12");
	ctable->addConst(const12);
}

void PQLIntegration::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
	ConstTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PQLIntegration );

void PQLIntegration::testSelectOnly() {
	string QUERY_STRING = "assign a; Select a";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(6 == r.size());

	// new test for if
	string QUERY_STRING2 = "if i; Select i";
	pcc = new PQLController();
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testSelectModifies() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\")";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(6 == r2.size());
}

void PQLIntegration::testSelectUses() {
	string QUERY_STRING = "while w; variable v; Select v such that Uses(w, v)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select v such that Uses(a, v)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(0 == r2.size());

	string QUERY_STRING3 = "stmt s; variable v; Select v such that Uses(s, v)";
	set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(2 == r3.size());
}

void PQLIntegration::testSelectFollows() {
	string QUERY_STRING = "assign a, a1; Select a such that Follows(a, a1)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING2 = "stmt s; Select s such that Follows(s, _)";
	r = pcc->parse(QUERY_STRING2);
	// answer supposed to be 1,3,4,5,7
    CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING3 = "stmt s; Select s such that Follows(_, s)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Follows(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectFollowsStar() {
	string QUERY_STRING = "assign a, a1; Select a1 such that Follows*(a, a1)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(3 == r.size());

	string QUERY_STRING2 = "stmt s; Select s such that Follows*(s, _)";
	r = pcc->parse(QUERY_STRING2);
	// answer: 1,3,4,5,7
	/*BOOST_FOREACH(auto p, r) {
		cout << p << " ";
	}*/
	CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING3 = "stmt s; Select s such that Follows*(_, s)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Follows*(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectParent() {
	string QUERY_STRING = "while w; assign a; Select a such that Parent(w, a)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(4 == r.size());

	string QUERY_STRING2 = "while w; Select w such that Parent(w, _)";
	r = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING3 = "while w; Select w such that Parent(_, w)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Parent(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());

	string QUERY_STRING5 = "assign a; Select a such that Parent(3, _)";
	r = pcc->parse(QUERY_STRING5);
	CPPUNIT_ASSERT(0 == r.size());

	string QUERY_STRING6 = "assign a; Select a such that Parent(2, _)";
	r = pcc->parse(QUERY_STRING6);
	CPPUNIT_ASSERT(6 == r.size());

}

void PQLIntegration::testSelectParentStar() {
	string QUERY_STRING = "while w; Select w such that Parent*(w, 6)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(2 == r.size()); 

	string QUERY_STRING2 = "while w; Select w such that Parent*(w, _)";
	r = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING3 = "while w; Select w such that Parent*(_, w)";
	r = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Parent*(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectPattern() {
	string QUERY_STRING = "assign a; Select a pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a pattern a(v, _\"4\"_)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	//cout << r2.size() << endl;
	CPPUNIT_ASSERT(2 == r2.size());

	string QUERY_STRING3 = "assign a; variable v; Select a pattern a(v, _\"2 + 3 + 4\"_)";
	set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r3.size());
	CPPUNIT_ASSERT("8" == *r3.begin());
}

void PQLIntegration::testSelectModifiesPattern() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\") pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"2 + 3 + 4\"_)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(1 == r2.size());

	//TODO: fix parsing of brackets in query parser
	string QUERY_STRING3 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"( 10 + 11 ) * 12\"_)";
	set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(1 == r3.size());
}

void PQLIntegration::testSelectFollowsPattern() {
	string QUERY_STRING = "assign a1, a2; Select a1 such that Follows(a1, a2) pattern a2(v, _\"2 + 3\"_)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
}

void PQLIntegration::testSelectUsesPattern() {
	string QUERY_STRING = "variable v; assign a; Select v such that Uses(_, v) pattern a(v, _)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	// answer: i, j
	BOOST_FOREACH(auto p, r) {
		cout << p << " ";
	}
	//CPPUNIT_ASSERT(2 == r.size());
}

void PQLIntegration::testSelectConstant() {
	string QUERY_STRING = "constant c; Select c such that Follows(1, 2)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(8 == r.size());

	string QUERY_STRING_2 = "constant c; assign a; Select c such that Follows(a, 1)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(0 == r2.size());
}

void PQLIntegration::testSelectProgLine() {
	string QUERY_STRING = "prog_line p; Select p such that Follows(1, p)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
	CPPUNIT_ASSERT(r.find("2") != r.end());

	string QUERY_STRING_2 = "prog_line p; Select p such that Follows(9, p)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(0 == r2.size());
}

void PQLIntegration::testFailParent(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "assign a; variable v; Select a such that Parent(v, a)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}

void PQLIntegration::testFailUses(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "variable v, v1; Select v such that Uses(v, v1)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}