#include <cppunit/config/SourcePrefix.h>
#include "ParentClauseTest.h"
#include "../SPA/ParentClause.h"
#include "../SPA/AST.h"
#include "../SPA/WhileNode.h"
#include "../SPA/IfNode.h"
#include "../SPA/CallNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;

void ParentClauseTest::setUp() {
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

void ParentClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParentClauseTest );


void ParentClauseTest::testParentFixedFixedPass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("6");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);
}

void ParentClauseTest::testParentFixedFixedFail() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("3");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);

}

void ParentClauseTest::testParentSynFixedPass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("4");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 1);
	CPPUNIT_ASSERT(res.hasResults("s"));
	unordered_set<string> s = res.selectSyn("s");
	CPPUNIT_ASSERT(s.size() == 1);
	CPPUNIT_ASSERT(s.find("3") != s.end());
}

void ParentClauseTest::testParentSynFixedFail() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("1");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool result =  m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);
}

void ParentClauseTest::testParentFixedSynPass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool r1 = m1->evaluate(&res);
	CPPUNIT_ASSERT(r1);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 3);

	unordered_set<string> s = res.selectSyn("s");
	CPPUNIT_ASSERT(s.size() == 3);
	CPPUNIT_ASSERT(s.find("2") != s.end());
	CPPUNIT_ASSERT(s.find("3") != s.end());
	CPPUNIT_ASSERT(s.find("6") != s.end());
}

void ParentClauseTest::testParentFixedSynPassWithWhile() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 1);
	unordered_set<string> s = res.selectSyn("w");
	CPPUNIT_ASSERT(s.size() == 1);
	CPPUNIT_ASSERT(s.find("3") != s.end());
}


void ParentClauseTest::testParentFixedSynFail() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	bool r1 = m1->evaluate(&res);
	CPPUNIT_ASSERT(!r1);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);
}


void ParentClauseTest::testParentFixedSynFailWithWhile() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("4");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	bool r1 = m1->evaluate(&res);
	CPPUNIT_ASSERT(!r1);
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);
}


void ParentClauseTest::testParentSynSynPass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());
	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 7);

	CPPUNIT_ASSERT(res.hasResults("s1"));
	CPPUNIT_ASSERT(res.hasResults("s2"));
	unordered_set<string> syns;
	syns.insert("s1");
	syns.insert("s2");
	//TODO: change this monstrosity
	//we want to check if the row exists, and also prevent duplicates from happening
	bool rowAppeared[7];
	fill(rowAppeared, rowAppeared + 7, false);
	Results::ResultsTable pairTable = res.selectMultiSyn(syns);
	for (Results::ResultsTable::iterator i = pairTable.begin(); i != pairTable.end(); ++i) {
		Results::Row row = *(*i);
		bool isRow1 = row["s1"] == "1" && row["s2"] == "2" && !rowAppeared[0];
		bool isRow2 = row["s1"] == "1" && row["s2"] == "3" && !rowAppeared[1];
		bool isRow3 = row["s1"] == "3" && row["s2"] == "4" && !rowAppeared[2];
		bool isRow4 = row["s1"] == "3" && row["s2"] == "5" && !rowAppeared[3];
		bool isRow5 = row["s1"] == "1" && row["s2"] == "6" && !rowAppeared[4];
		bool isRow6 = row["s1"] == "6" && row["s2"] == "7" && !rowAppeared[5];
		bool isRow7 = row["s1"] == "6" && row["s2"] == "8" && !rowAppeared[6];
		if (isRow1) {
			rowAppeared[0] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow2) {
			rowAppeared[1] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow3) {
			rowAppeared[2] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow4) {
			rowAppeared[3] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow5) {
			rowAppeared[4] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow6) {
			rowAppeared[5] = true;
			CPPUNIT_ASSERT(true);
		} else if (isRow7) {
			rowAppeared[6] = true;
			CPPUNIT_ASSERT(true);
		} else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void ParentClauseTest::testParentSynSynPassWithWhile() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());
	CPPUNIT_ASSERT(m1->evaluate(&res));
	
	CPPUNIT_ASSERT(res.getResultsTableSize() == 1);
	CPPUNIT_ASSERT(res.hasResults("s1"));
	CPPUNIT_ASSERT(res.hasResults("s2"));
	unordered_set<string> syns;
	syns.insert("s1");
	syns.insert("s2");
	Results::ResultsTable pairTable = res.selectMultiSyn(syns);
	Results::Row row = *(*pairTable.begin());
	CPPUNIT_ASSERT(row["s1"] == "1" && row["s2"] == "3");
}

void ParentClauseTest::testParentSynSynPassWithIf() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_IF);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());
	CPPUNIT_ASSERT(m1->evaluate(&res));
	
	CPPUNIT_ASSERT(res.getResultsTableSize() == 2);
	CPPUNIT_ASSERT(res.hasResults("s1"));
	CPPUNIT_ASSERT(res.hasResults("s2"));
	unordered_set<string> syns;
	syns.insert("s1");
	syns.insert("s2");
	Results::ResultsTable pairTable = res.selectMultiSyn(syns);

	for (Results::ResultsTable::iterator i = pairTable.begin(); i != pairTable.end(); ++i) {
		Results::Row row = *(*i);
		CPPUNIT_ASSERT((row["s1"] == "6" && row["s2"] == "7")  ||  (row["s1"] == "6" && row["s2"] == "8"));
	}
}


void ParentClauseTest::testParentFirstUnderscorePass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("_");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_GENERIC);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 7);
	unordered_set<string> s = res.selectSyn("s2");
	CPPUNIT_ASSERT(s.size() == 7);
	CPPUNIT_ASSERT(s.find("2") != s.end());
	CPPUNIT_ASSERT(s.find("3") != s.end());
	CPPUNIT_ASSERT(s.find("4") != s.end());
	CPPUNIT_ASSERT(s.find("5") != s.end());
	CPPUNIT_ASSERT(s.find("6") != s.end());
	CPPUNIT_ASSERT(s.find("7") != s.end());
	CPPUNIT_ASSERT(s.find("8") != s.end());
}

void ParentClauseTest::testParentSecondUnderscorePass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s2");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("_");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 3);
	unordered_set<string> s = res.selectSyn("s2");
	CPPUNIT_ASSERT(s.size() == 3);
	CPPUNIT_ASSERT(s.find("1") != s.end());
	CPPUNIT_ASSERT(s.find("3") != s.end());
	CPPUNIT_ASSERT(s.find("6") != s.end());
}

void ParentClauseTest::testParentBothUnderscorePass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("_");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_GENERIC);
	m1->setSecondArg("_");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 0);
}

void ParentClauseTest::testParentCallPass() {
	Results res = Results();
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("c");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_CALL);
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.hasResults("s1"));
	CPPUNIT_ASSERT(res.hasResults("c"));
	CPPUNIT_ASSERT(res.getResultsTableSize() == 1);
	unordered_set<string> syns;
	syns.insert("s1");
	syns.insert("c");
	Results::ResultsTable pairTable = res.selectMultiSyn(syns);
	Results::Row row = *(*pairTable.begin());
	CPPUNIT_ASSERT(row["s1"] == "6" && row["c"] == "8");
}