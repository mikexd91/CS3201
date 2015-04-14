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

#include <iostream>
#include <string>
#include <algorithm>

using namespace stringconst;
using namespace std;

void PQLIntegration::setUp() {
	/* testing this source

	procedure chocs {
		a=4;			//1
		while i {		//2
			k = 3;		//3
			while j {	//4
				i=1;	//5
				j=2;	//6
			}
			b=5;		//7
			j=2+3+4;	//8
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
	assg4->linkExprNode(new ConstNode("2"));
	whilesl2->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(7);
	VarNode* b1 = new VarNode("b");
	assg5->linkVarNode(b1);
	assg5->linkExprNode(new ConstNode("5"));
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
	set<int> children3 = *new set<int>();
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
	stmt7->setFollowsAfter(8);
	set<string> mods7= set<string>();
	mods7.emplace("b");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg5);
	stmt7->setParent(2);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	stmt8->setFollowsBefore(7);
	stmt8->setTNodeRef(assg6);
	stmt8->setParent(2);
	stable->addStmt(stmt8);

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

void PQLIntegration::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PQLIntegration );

void PQLIntegration::testSelectOnly() {
	string QUERY_STRING = "assign a; Select a";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectModifies() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\")";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	//cout << *r.begin() << endl;
	CPPUNIT_ASSERT(1 == r.size());
	//CPPUNIT_ASSERT("1" == *r.begin());
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
	CPPUNIT_ASSERT(2 == r.size());
}

void PQLIntegration::testSelectFollows() {
	string QUERY_STRING = "assign a, a1; Select a such that Follows(a, a1)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	
	CPPUNIT_ASSERT(2 == r.size());
	//CPPUNIT_ASSERT("5" == *r.begin());
}

void PQLIntegration::testSelectFollowsStar() {
	string QUERY_STRING = "assign a, a1; Select a1 such that Follows*(a, a1)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(3 == r.size());
	//CPPUNIT_ASSERT("6" == *r.begin());
}

void PQLIntegration::testSelectParent() {
	string QUERY_STRING = "while w; assign a; Select a such that Parent(w, a)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(4 == r.size());

}

void PQLIntegration::testSelectParentStar() {
	string QUERY_STRING = "while w; Select w such that Parent*(w, 6)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(2 == r.size());
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
	//CPPUNIT_ASSERT("1" == *r2.begin());

	string QUERY_STRING3 = "assign a; variable v; Select a pattern a(v, _\"2 + 3 + 4\"_)";
	set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	//cout << r2.size() << endl;
	CPPUNIT_ASSERT(1 == r3.size());
	CPPUNIT_ASSERT("8" == *r3.begin());
}

void PQLIntegration::testSelectModifiesPattern() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\") pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"2 + 3\"_)";
	set<string> r2;
	r2 = pcc->parse(QUERY_STRING);
	//cout << *r2.begin() << endl;
	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testSelectFollowsPattern() {
	string QUERY_STRING = "assign a1, a2; Select a1 such that Follows(a1, a2) pattern a2(v, _\"2 + 3\"_)";
	PQLController* pcc = new PQLController();
	set<string> r;
	r = pcc->parse(QUERY_STRING);
	//cout << *r.begin() << endl;
	//cout << r.size() << endl;
	CPPUNIT_ASSERT(1 == r.size());
}