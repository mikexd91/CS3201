#include <cppunit/config/SourcePrefix.h>
#include "WithClauseTest.h"
#include "../SPA/WithClause.h"
#include "../SPA/WithClauseBuilder.h"
#include "../SPA/WithClauseRef.h"
#include "../SPA/AST.h"
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
	procedure i {
		i = 1+2;	//1
		j = 2+3+4;	//2
		k = 3;	//3
	}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("i");
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

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string ivar = "i";
	unordered_set<string> uses1 = unordered_set<string>();
	uses1.emplace(ivar);
	stmt1->setModifies(uses1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(3);
	string jvar = "j";
	unordered_set<string> uses2 = unordered_set<string>();
	uses2.emplace(jvar);
	stmt2->setModifies(uses2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	string kvar = "k";
	unordered_set<string> uses3 = unordered_set<string>();
	uses3.emplace(kvar);
	stmt3->setModifies(uses3);
	stmt3->setTNodeRef(assg3);
	stable->addStmt(stmt3);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addTNode(assg1);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addTNode(assg2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(2);
	vk->addTNode(assg3);
	vtable->addVariable(vk);

	// to set up the proctable manually
	ProcTable* procTable = ProcTable::getInstance();

	Procedure* pi = new Procedure("i");
	procTable->addProc(pi);

	// to set up the constable manually
	ConstTable* constTable = ConstTable::getInstance();

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
}

void WithClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( WithClauseTest );

// 1 = 1
void WithClauseTest::evaluateRightIntLeftInt() {
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::INTEGER_);
	withBuilder->setEntity(1, "1");
	withBuilder->setRefType(2, RefType::INTEGER_);
	withBuilder->setEntity(2, "1");
	WithClause* w1 = withBuilder->build();

	CPPUNIT_ASSERT(w1->isValid());

	Result* r1 = new Result();

	CPPUNIT_ASSERT(w1->evaluate(r1));
	//CPPUNIT_ASSERT(w1->getClauseType() == WITH_);

	// test the contents of r1
	// in this case there are no contents of r1
}

// "asd" = "asd"
void WithClauseTest::evaluateRightStringLeftString()
{
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::IDENT_);
	withBuilder->setEntity(1, "asd");
	withBuilder->setRefType(2, RefType::IDENT_);
	withBuilder->setEntity(2, "asd");
	WithClause* w1 = withBuilder->build();
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));

	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder2->setRefType(1, RefType::IDENT_);
	withBuilder2->setEntity(1, "asd");
	withBuilder2->setRefType(2, RefType::IDENT_);
	withBuilder2->setEntity(2, "www");
	WithClause* w2 = withBuilder2->build();
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2) == false);

	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder3->setRefType(1, RefType::INTEGER_);
	withBuilder3->setEntity(1, "1");
	withBuilder3->setRefType(2, RefType::INTEGER_);
	withBuilder3->setEntity(2, "1");
	WithClause* w3 = withBuilder3->build();
	Result* r3 = new Result();
	CPPUNIT_ASSERT(w3->evaluate(r3));
}

// n1 = n2 where n1, n2 are prog_line
void WithClauseTest::evaluateRightSynLeftSyn()
{
	


}

// n = 1 where n is prog_line
void WithClauseTest::evaluateRightSynLeftInt()
{
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::SYNONYM_);
	withBuilder->setEntityType(1, stringconst::ARG_PROGLINE);
	withBuilder->setEntity(1, "p");
	withBuilder->setAttrType(1, NULLATTR_);

	withBuilder->setRefType(2, RefType::INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setAttrType(2, NULLATTR_);
	WithClause* w1 = withBuilder->build();
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);

	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder2->setRefType(2, RefType::SYNONYM_);
	withBuilder2->setEntityType(2, stringconst::ARG_PROGLINE);
	withBuilder2->setEntity(2, "p");
	withBuilder2->setAttrType(2, NULLATTR_);

	withBuilder2->setRefType(1, RefType::INTEGER_);
	withBuilder2->setEntity(1, "1");
	withBuilder2->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder2->setAttrType(1, NULLATTR_);
	WithClause* w2 = withBuilder2->build();
	Result* r2 = new Result();
	CPPUNIT_ASSERT(w2->evaluate(r2));
}

// s.stmt# = 1 where s is stmt
void WithClauseTest::evaluateRightAttrStmtNumLeftInt()
{
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::ATTRREF_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, STMTNUM_);

	withBuilder->setRefType(2, RefType::INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setAttrType(2, NULLATTR_);
	WithClause* w1 = withBuilder->build();
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);
}

// c.value = 1 where c is constant
void WithClauseTest::evaluateRightAttrValueLeftInt()
{
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::ATTRREF_);
	withBuilder->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, CONSTVALUE_);

	withBuilder->setRefType(2, RefType::INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder->setAttrType(2, NULLATTR_);
	WithClause* w1 = withBuilder->build();
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);
}

// v.varName = "x" where v is var
void WithClauseTest::evaluateRightAttrVarNameLeftString()
{
	WithClauseBuilder* withBuilder = new WithClauseBuilder(ClauseType::WITH_);
	withBuilder->setRefType(1, RefType::ATTRREF_);
	withBuilder->setEntityType(1, stringconst::ARG_VARIABLE);
	withBuilder->setEntity(1, "s");
	withBuilder->setAttrType(1, VARNAME_);

	withBuilder->setRefType(2, RefType::IDENT_);
	withBuilder->setEntity(2, "i");
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);
	withBuilder->setAttrType(2, NULLATTR_);
	WithClause* w1 = withBuilder->build();
	Result* r1 = new Result();
	CPPUNIT_ASSERT(w1->evaluate(r1));
	CPPUNIT_ASSERT(r1->getResultTableSize() == 1);
}

// p.procName = "proc" where p is procedure
void WithClauseTest::evaluateRightAttrProcNameLeftString()
{

}

// p1.procName = p2.procName
void WithClauseTest::evaluateRightAttrProcNameLeftAttrProcName()
{

}

// v1.varName = v2.varName
void WithClauseTest::evaluateRightAttrVarNameLeftAttrVarName()
{

}

// s1.stmt# = s2.stmt#
void WithClauseTest::evaluateRightAttrStmtNumLeftAttrStmtNum()
{

}

// c1.value = c2.value
void WithClauseTest::evaluateRightAttrValueLeftAttrValue()
{

}

// p.procName = v.varName
void WithClauseTest::evaluateRightAttrProcNameLeftAttrVarName()
{

}

// s.stmt# = c.value
void WithClauseTest::evaluateRightAttrStmtNumLeftAttrValue()
{

}
