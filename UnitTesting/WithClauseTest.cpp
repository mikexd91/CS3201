#include <cppunit/config/SourcePrefix.h>
#include "WithClauseTest.h"
#include "../SPA/WithClause.h"
#include "../SPA/WithClauseBuilder.h"
#include "../SPA/WithClauseRef.h"
#include "../SPA/AST.h"
#include "../SPA/IfNode.h"
#include "../SPA/WhileNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/ProcTable.h"
#include "../SPA/ConstTable.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>


using namespace std;
using namespace stringconst;

void WithClauseTest::setUp() {
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

		call proc2; //12
	}

	procedure proc2 {
		a = 2;		//13
		b = a;		//14
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
	stable->clearTable();

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

	Statement* stmt12 = new Statement();
	stmt12->setStmtNum(12);
	stmt12->setType(CALL_STMT_);
	string stmt12Uses[] = {"a"};
	string stmt12Modifies[] = {"a", "b"};
	unordered_set<string> stmt12UsesSet(stmt12Uses, stmt12Uses + 1);
	unordered_set<string> stmt12ModifiesSet(stmt12Modifies, stmt12Modifies + 2);
	stmt12->setUses(stmt12UsesSet);
	stmt12->setModifies(stmt12ModifiesSet);
	stable->addStmt(stmt12);

	Statement* stmt13 = new Statement();
	stmt13->setStmtNum(13);
	stmt13->setType(ASSIGN_STMT_);
	unordered_set<string> mods13;
	mods13.insert("a");
	stmt13->setModifies(mods13);
	stable->addStmt(stmt13);

	Statement* stmt14 = new Statement();
	stmt14->setStmtNum(14);
	stmt14->setType(ASSIGN_STMT_);
	unordered_set<string> mods14;
	unordered_set<string> uses14;
	mods14.insert("b");
	uses14.insert("a");
	stmt14->setModifies(mods14);
	stmt14->setUses(uses14);
	stable->addStmt(stmt14);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();
	vtable->reset();

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

	Variable* va = new Variable("a");
	va->addModifyingProc("zumba");
	va->addModifyingProc("proc2");
	va->addModifyingStmt(13);
	va->addModifyingStmt(12);
	va->addUsingStmt(14);
	va->addUsingStmt(12);
	vtable->addVariable(va);

	Variable* vb = new Variable("b");
	vb->addModifyingProc("zumba");
	vb->addModifyingProc("proc2");
	vb->addModifyingStmt(14);
	vb->addModifyingStmt(12);
	vtable->addVariable(vb);

	// set procedure for modifies
	ProcTable* procTable = ProcTable::getInstance();
	procTable->clearTable();
	Procedure* procedure = new Procedure("zumba");
	string procUsesArr[] = {"i", "w", "k", "j", "a"};
	unordered_set<string> procUses(procUsesArr, procUsesArr + 5);
	string procModsArr[] = {"i", "j", "k", "w", "x", "z", "y", "a", "b"};
	unordered_set<string> procModifies(procModsArr, procModsArr + 9);
	procedure->setUses(procUses);
	procedure->setModifies(procModifies);
	procTable->addProc(procedure);

	Procedure* proc2 = new Procedure("proc2");
	string proc2Uses[] = {"a"};
	string proc2Modifies[] = {"a", "b"};
	unordered_set<string> proc2UsesSet(proc2Uses, proc2Uses + 1);
	unordered_set<string> proc2ModifiesSet(proc2Modifies, proc2Modifies + 2);
	proc2->setUses(proc2UsesSet);
	proc2->setModifies(proc2ModifiesSet);
	procTable->addProc(proc2);

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

		call proc2; //12
	}

	procedure proc2 {
		a = 2;		//13
		b = a;		//14
	}
	*/
	ConstTable* constTable = ConstTable::getInstance();
	constTable->clearTable();
	Constant* c1 = new Constant("1");
	c1->addAppearsIn(1);
	c1->addTNodeRef(one1);
	constTable->addConst(c1);

	Constant* c2 = new Constant("2");
	c2->addAppearsIn(1);
	c2->addAppearsIn(2);
	c2->addTNodeRef(two1);
	c2->addTNodeRef(two2);
	constTable->addConst(c2);

	Constant* c3 = new Constant("3");
	c3->addAppearsIn(2);
	c3->addAppearsIn(3);
	c3->addTNodeRef(three2);
	c3->addTNodeRef(three3);
	constTable->addConst(c3);

	Constant* c4 = new Constant("4");
	c4->addAppearsIn(2);
	c4->addTNodeRef(four2);
	constTable->addConst(c4);

	Constant* c6 = new Constant("6");
	c6->addAppearsIn(11);
	//c6->addTNodeRef(six11); missing cause no constnode made for this
	constTable->addConst(c6);
}

void WithClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	ConstTable::getInstance()->clearTable();
	VarTable::getInstance()->reset();
	ProcTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( WithClauseTest );

void WithClauseTest::testIntEqualsInt() {
	// 1=1 (true, result# = 0)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, INTEGER_);
	withBuilder->setEntity(1, "1");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));

	//1=2 (false, result# = 0)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, INTEGER_);
	withBuilder2->setEntity(1, "1");
	withBuilder2->setAttrType(1, NULLATTR_);
	withBuilder2->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder2->setRefType(2, INTEGER_);
	withBuilder2->setEntity(2, "2");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2) == false);
}

void WithClauseTest::testStringEqualsString() {
	// asd=asd (true, result# = 0)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, IDENT_);
	withBuilder->setEntity(1, "asd");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);
	withBuilder->setRefType(2, IDENT_);
	withBuilder->setEntity(2, "asd");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));

	//asd=dsa (false, result# = 0)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, IDENT_);
	withBuilder2->setEntity(1, "asd");
	withBuilder2->setAttrType(1, NULLATTR_);
	withBuilder2->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);
	withBuilder2->setRefType(2, IDENT_);
	withBuilder2->setEntity(2, "dsa");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2) == false);
}

void WithClauseTest::testStmtEqualsInt() {
	// s.stmt# = 1 (true, result# = 1)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	//a.stmt#=1 (true, result# = 1)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "a");
	withBuilder2->setAttrType(1, STMTNUM_);
	withBuilder2->setEntityType(1, stringconst::ARG_ASSIGN);
	withBuilder2->setRefType(2, INTEGER_);
	withBuilder2->setEntity(2, "2");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 1);

	//i.stmt#=9 (true, result# = 1)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "i");
	withBuilder3->setAttrType(1, STMTNUM_);
	withBuilder3->setEntityType(1, stringconst::ARG_IF);
	withBuilder3->setRefType(2, INTEGER_);
	withBuilder3->setEntity(2, "9");
	withBuilder3->setAttrType(2, NULLATTR_);
	withBuilder3->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3));
	CPPUNIT_ASSERT(r3->getResultTableSize() == 1);

	//w.stmt#=7 (true, result# = 1)
	WithClauseBuilder* withBuilder4 = new WithClauseBuilder(WITH_);
	withBuilder4->setRefType(1, ATTRREF_);
	withBuilder4->setEntity(1, "w");
	withBuilder4->setAttrType(1, STMTNUM_);
	withBuilder4->setEntityType(1, stringconst::ARG_WHILE);
	withBuilder4->setRefType(2, INTEGER_);
	withBuilder4->setEntity(2, "7");
	withBuilder4->setAttrType(2, NULLATTR_);
	withBuilder4->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w4 = withBuilder4->build();
	CPPUNIT_ASSERT(w4->isValid());
	Result* r4 = new Result();
	CPPUNIT_ASSERT(w4->evaluate(r4));
	CPPUNIT_ASSERT(r4->getResultTableSize() == 1);

	//call.stmt#=12 (true, result# = 1)
	WithClauseBuilder* withBuilder5 = new WithClauseBuilder(WITH_);
	withBuilder5->setRefType(1, ATTRREF_);
	withBuilder5->setEntity(1, "cz");
	withBuilder5->setAttrType(1, STMTNUM_);
	withBuilder5->setEntityType(1, stringconst::ARG_CALL);
	withBuilder5->setRefType(2, INTEGER_);
	withBuilder5->setEntity(2, "12");
	withBuilder5->setAttrType(2, NULLATTR_);
	withBuilder5->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w5 = withBuilder5->build();
	CPPUNIT_ASSERT(w5->isValid());
	Result* r5 = new Result();
	CPPUNIT_ASSERT(w5->evaluate(r5));
	CPPUNIT_ASSERT(r5->getResultTableSize() == 1);
}

void WithClauseTest::testVNameEqualsString(){
	// v.varName = "i" (true, result# = 1)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "v");
	withBuilder->setAttrType(1, VARNAME_);
	withBuilder->setEntityType(1, stringconst::ARG_VARIABLE);
	withBuilder->setRefType(2, IDENT_);
	withBuilder->setEntity(2, "i");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	// v.varName = "asd" (false, result# = 0)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "v");
	withBuilder2->setAttrType(1, VARNAME_);
	withBuilder2->setEntityType(1, stringconst::ARG_VARIABLE);
	withBuilder2->setRefType(2, IDENT_);
	withBuilder2->setEntity(2, "p");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(r2->getResultTableSize() == 0);

	// "i" = v.varName (true, result# = 1)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(2, ATTRREF_);
	withBuilder3->setEntity(2, "v");
	withBuilder3->setAttrType(2, VARNAME_);
	withBuilder3->setEntityType(2, stringconst::ARG_VARIABLE);
	withBuilder3->setRefType(1, IDENT_);
	withBuilder3->setEntity(1, "i");
	withBuilder3->setAttrType(1, NULLATTR_);
	withBuilder3->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3));
	CPPUNIT_ASSERT(r3->getResultTableSize() == 1);
}

void WithClauseTest::testPNameEqualsString(){
	// p.procName = "proc2" (true, result# = 1)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "p");
	withBuilder->setAttrType(1, PROCNAME_);
	withBuilder->setEntityType(1, stringconst::ARG_PROCEDURE);
	withBuilder->setRefType(2, IDENT_);
	withBuilder->setEntity(2, "proc2");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	//"proc2" = p.procName (true, result# = 1)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(2, ATTRREF_);
	withBuilder2->setEntity(2, "p");
	withBuilder2->setAttrType(2, PROCNAME_);
	withBuilder2->setEntityType(2, stringconst::ARG_PROCEDURE);
	withBuilder2->setRefType(1, IDENT_);
	withBuilder2->setEntity(1, "proc2");
	withBuilder2->setAttrType(1, NULLATTR_);
	withBuilder2->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 1);

	// p.procName = "what" (false, result# = 0)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "p");
	withBuilder3->setAttrType(1, PROCNAME_);
	withBuilder3->setEntityType(1, stringconst::ARG_PROCEDURE);
	withBuilder3->setRefType(2, IDENT_);
	withBuilder3->setEntity(2, "what");
	withBuilder3->setAttrType(2, NULLATTR_);
	withBuilder3->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3) == false);
	CPPUNIT_ASSERT(r3->getResultTableSize() == 0);

}

void WithClauseTest::testCallEqualsString(){

	// call.procName = "proc2" (true, result# = 1)
	WithClauseBuilder* withBuilder4 = new WithClauseBuilder(WITH_);
	withBuilder4->setRefType(1, ATTRREF_);
	withBuilder4->setEntity(1, "c");
	withBuilder4->setAttrType(1, PROCNAME_);
	withBuilder4->setEntityType(1, stringconst::ARG_PROCEDURE);
	withBuilder4->setRefType(2, IDENT_);
	withBuilder4->setEntity(2, "proc2");
	withBuilder4->setAttrType(2, NULLATTR_);
	withBuilder4->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w4 = withBuilder4->build();
	CPPUNIT_ASSERT(w4->isValid());
	Result* r4 = new Result();
	CPPUNIT_ASSERT(w4->evaluate(r4));
	CPPUNIT_ASSERT(r4->getResultTableSize() == 1);

	// call.procName = "what" (false, result# = 0)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "c");
	withBuilder3->setAttrType(1, PROCNAME_);
	withBuilder3->setEntityType(1, stringconst::ARG_PROCEDURE);
	withBuilder3->setRefType(2, IDENT_);
	withBuilder3->setEntity(2, "what");
	withBuilder3->setAttrType(2, NULLATTR_);
	withBuilder3->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3) == false);
	CPPUNIT_ASSERT(r3->getResultTableSize() == 0);
}

void WithClauseTest::testPNameEqualsPName(){
	// p.procName = q.procName (true, result# = 2)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "p");
	withBuilder->setAttrType(1, PROCNAME_);
	withBuilder->setEntityType(1, stringconst::ARG_PROCEDURE);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "q");
	withBuilder->setAttrType(2, PROCNAME_);
	withBuilder->setEntityType(2, stringconst::ARG_PROCEDURE);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 2);
}

void WithClauseTest::testVNameEqualsVName(){
	// b.varName = a.varName (true, result# = 9)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "a");
	withBuilder->setAttrType(1, VARNAME_);
	withBuilder->setEntityType(1, stringconst::ARG_VARIABLE);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "b");
	withBuilder->setAttrType(2, VARNAME_);
	withBuilder->setEntityType(2, stringconst::ARG_VARIABLE);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 9);
}

void WithClauseTest::testSynEqualsInt(){
	// progline = 1 (true, result# = 1)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, SYNONYM_);
	withBuilder->setEntity(1, "progline");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	// s.stmt# = 1 (true, result# = 1)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "s");
	withBuilder2->setAttrType(1, STMTNUM_);
	withBuilder2->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder2->setRefType(2, INTEGER_);
	withBuilder2->setEntity(2, "1");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 1);

	// w.stmt# = 1 (false, result# = 0)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "w");
	withBuilder3->setAttrType(1, STMTNUM_);
	withBuilder3->setEntityType(1, stringconst::ARG_WHILE);
	withBuilder3->setRefType(2, INTEGER_);
	withBuilder3->setEntity(2, "1");
	withBuilder3->setAttrType(2, NULLATTR_);
	withBuilder3->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3) == false);
	CPPUNIT_ASSERT(r3->getResultTableSize() == 0);
}

void WithClauseTest::testConstEqualsInt(){
	// c.value = 1 (true, result# = 1)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "c");
	withBuilder->setAttrType(1, CONSTVALUE_);
	withBuilder->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	// c.value = 999 (false, result# = 0)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "c");
	withBuilder2->setAttrType(1, CONSTVALUE_);
	withBuilder2->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder2->setRefType(2, INTEGER_);
	withBuilder2->setEntity(2, "999");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2) == false);
	CPPUNIT_ASSERT(r2->getResultTableSize() == 0);
}

void WithClauseTest::testConstEqualsConst(){
	// c.value = d.value (true, result# = 4)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "c");
	withBuilder->setAttrType(1, CONSTVALUE_);
	withBuilder->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "d");
	withBuilder->setAttrType(2, CONSTVALUE_);
	withBuilder->setEntityType(2, stringconst::ARG_CONSTANT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 5);
}

void WithClauseTest::testConstEqualsStmt(){
	// c.value = s.stmt# (true, result# = 4)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "c");
	withBuilder->setAttrType(1, CONSTVALUE_);
	withBuilder->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 5);

	// c.value = a.stmt# (true, result# = 4)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "c");
	withBuilder2->setAttrType(1, CONSTVALUE_);
	withBuilder2->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder2->setRefType(2, ATTRREF_);
	withBuilder2->setEntity(2, "s");
	withBuilder2->setAttrType(2, STMTNUM_);
	withBuilder2->setEntityType(2, stringconst::ARG_ASSIGN);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 5);

	// c.value = w.stmt# (true, result# = 4)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "c");
	withBuilder3->setAttrType(1, CONSTVALUE_);
	withBuilder3->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder3->setRefType(2, ATTRREF_);
	withBuilder3->setEntity(2, "s");
	withBuilder3->setAttrType(2, STMTNUM_);
	withBuilder3->setEntityType(2, stringconst::ARG_WHILE);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	//issue
	CPPUNIT_ASSERT(w3->evaluate(r3) == false);
	CPPUNIT_ASSERT(r3->getResultTableSize() == 0);
}

void WithClauseTest::testSynEqualsSyn(){
	// progline = progline2 (true, result# = 14)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, SYNONYM_);
	withBuilder->setEntity(1, "progline");
	withBuilder->setAttrType(1, NULLATTR_);
	withBuilder->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder->setRefType(2, SYNONYM_);
	withBuilder->setEntity(2, "progline2");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ARG_PROGLINE);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 14);
}

void WithClauseTest::testStmtEqualsStmt(){
	// s.stmt# = s1.stmt# (true, result# = 14)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s1");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 14);

	// s.stmt# = a.stmt# (true, result# = 11)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, ATTRREF_);
	withBuilder2->setEntity(1, "s");
	withBuilder2->setAttrType(1, STMTNUM_);
	withBuilder2->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder2->setRefType(2, ATTRREF_);
	withBuilder2->setEntity(2, "a");
	withBuilder2->setAttrType(2, STMTNUM_);
	withBuilder2->setEntityType(2, stringconst::ARG_ASSIGN);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 11);

	// s.stmt# = i.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, ATTRREF_);
	withBuilder3->setEntity(1, "s");
	withBuilder3->setAttrType(1, STMTNUM_);
	withBuilder3->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder3->setRefType(2, ATTRREF_);
	withBuilder3->setEntity(2, "i");
	withBuilder3->setAttrType(2, STMTNUM_);
	withBuilder3->setEntityType(2, stringconst::ARG_IF);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3));
	CPPUNIT_ASSERT(r3->getResultTableSize() == 1);

	// s.stmt# = w.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder4 = new WithClauseBuilder(WITH_);
	withBuilder4->setRefType(1, ATTRREF_);
	withBuilder4->setEntity(1, "s");
	withBuilder4->setAttrType(1, STMTNUM_);
	withBuilder4->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder4->setRefType(2, ATTRREF_);
	withBuilder4->setEntity(2, "s1");
	withBuilder4->setAttrType(2, STMTNUM_);
	withBuilder4->setEntityType(2, stringconst::ARG_WHILE);

	WithClause* w4 = withBuilder4->build();
	CPPUNIT_ASSERT(w4->isValid());
	Result* r4 = new Result();
	CPPUNIT_ASSERT(w4->evaluate(r4));
	CPPUNIT_ASSERT(r4->getResultTableSize() == 1);

	// s.stmt# = ca.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder5 = new WithClauseBuilder(WITH_);
	withBuilder5->setRefType(1, ATTRREF_);
	withBuilder5->setEntity(1, "s");
	withBuilder5->setAttrType(1, STMTNUM_);
	withBuilder5->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder5->setRefType(2, ATTRREF_);
	withBuilder5->setEntity(2, "ca");
	withBuilder5->setAttrType(2, STMTNUM_);
	withBuilder5->setEntityType(2, stringconst::ARG_CALL);

	WithClause* w5 = withBuilder5->build();
	CPPUNIT_ASSERT(w5->isValid());
	Result* r5 = new Result();
	CPPUNIT_ASSERT(w5->evaluate(r5));
	CPPUNIT_ASSERT(r5->getResultTableSize() == 1);
}

void WithClauseTest::testSynEqualsValue(){
	// p = c.value# (true, result# = 5)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, SYNONYM_);
	withBuilder->setEntity(1, "p");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "c");
	withBuilder->setAttrType(2, CONSTVALUE_);
	withBuilder->setEntityType(2, stringconst::ARG_CONSTANT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 5);
}

void WithClauseTest::testSynEqualsStmt(){
	// p = s.stmt# (true, result# = 14)
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, SYNONYM_);
	withBuilder->setEntity(1, "p");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder->setRefType(2, ATTRREF_);
	withBuilder->setEntity(2, "s");
	withBuilder->setAttrType(2, STMTNUM_);
	withBuilder->setEntityType(2, stringconst::ARG_STATEMENT);

	WithClause* w1 = withBuilder->build();
	CPPUNIT_ASSERT(w1->isValid());
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 14);

	// p = a.stmt# (true, result# = 11)
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, SYNONYM_);
	withBuilder2->setEntity(1, "p");
	withBuilder2->setAttrType(1, STMTNUM_);
	withBuilder2->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder2->setRefType(2, ATTRREF_);
	withBuilder2->setEntity(2, "a");
	withBuilder2->setAttrType(2, STMTNUM_);
	withBuilder2->setEntityType(2, stringconst::ARG_ASSIGN);

	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
	CPPUNIT_ASSERT(r2->getResultTableSize() == 11);

	// p = c.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, SYNONYM_);
	withBuilder3->setEntity(1, "p");
	withBuilder3->setAttrType(1, STMTNUM_);
	withBuilder3->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder3->setRefType(2, ATTRREF_);
	withBuilder3->setEntity(2, "c");
	withBuilder3->setAttrType(2, STMTNUM_);
	withBuilder3->setEntityType(2, stringconst::ARG_CALL);

	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3));
	CPPUNIT_ASSERT(r3->getResultTableSize() == 1);

	// p = i.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder4 = new WithClauseBuilder(WITH_);
	withBuilder4->setRefType(1, SYNONYM_);
	withBuilder4->setEntity(1, "p");
	withBuilder4->setAttrType(1, STMTNUM_);
	withBuilder4->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder4->setRefType(2, ATTRREF_);
	withBuilder4->setEntity(2, "s");
	withBuilder4->setAttrType(2, STMTNUM_);
	withBuilder4->setEntityType(2, stringconst::ARG_IF);

	WithClause* w4 = withBuilder4->build();
	CPPUNIT_ASSERT(w4->isValid());
	Result* r4 = new Result();
	CPPUNIT_ASSERT(w4->evaluate(r4));
	CPPUNIT_ASSERT(r4->getResultTableSize() == 1);

	// p = w.stmt# (true, result# = 1)
	WithClauseBuilder* withBuilder5 = new WithClauseBuilder(WITH_);
	withBuilder5->setRefType(1, SYNONYM_);
	withBuilder5->setEntity(1, "p");
	withBuilder5->setAttrType(1, STMTNUM_);
	withBuilder5->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder5->setRefType(2, ATTRREF_);
	withBuilder5->setEntity(2, "s");
	withBuilder5->setAttrType(2, STMTNUM_);
	withBuilder5->setEntityType(2, stringconst::ARG_WHILE);

	WithClause* w5 = withBuilder5->build();
	CPPUNIT_ASSERT(w5->isValid());
	Result* r5 = new Result();
	CPPUNIT_ASSERT(w5->evaluate(r5));
	CPPUNIT_ASSERT(r5->getResultTableSize() == 1);
}