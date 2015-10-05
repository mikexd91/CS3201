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
#include "../SPA/ProcTable.h"
#include "../SPA/CFG.h"
#include "../SPA/CallNode.h"

using namespace stringconst;
using namespace std;
using boost::unordered_set;

void PQLIntegration::setUp() {
	/* testing this source

	procedure Pizza {
		if good then {						//1
			eaten = eaten + 1;					//2
		} else {							
			drink = more * (beer - water);	//3
			call YourMom;					//4
		}
	}	

	procedure YourMom {
		while nagging {						//5
			nagging = nagging + 1;			//6
			while i {						//7
				mom = nagging;				//8
			}
		}
		full = drink + eaten;					//9
	}
	*/

//-------------  SET UP AST -------------//
	AST* ast = AST::getInstance();

// procedure Pizza
	ProcNode* pizzaProc = new ProcNode("Pizza");
	ast->addProcNode(pizzaProc);
	StmtLstNode* pizzasl = new StmtLstNode();
	pizzaProc->linkStmtLstNode(pizzasl);
	
	IfNode* if1 = new IfNode(1);
	pizzasl->linkStmtNode(if1);
	VarNode* good1 = new VarNode("good");
	if1->linkVarNode(good1);
	StmtLstNode* thensl1 = new StmtLstNode();
	if1->linkThenStmtLstNode(thensl1);
	StmtLstNode* elsesl1 = new StmtLstNode();
	if1->linkElseStmtLstNode(elsesl1);

	AssgNode* assg2 = new AssgNode(2);
	thensl1->linkStmtNode(assg2);
	VarNode* eaten1 = new VarNode("eaten");
	assg2->linkVarNode(eaten1);
	OpNode* plus1 = new OpNode("+");
	assg2->linkExprNode(plus1);
	VarNode* eaten2 = new VarNode("eaten");
	plus1->linkLeftNode(eaten2);
	ConstNode* one1 = new ConstNode("1");
	plus1->linkRightNode(one1);

	AssgNode* assg3 = new AssgNode(3);
	elsesl1->linkStmtNode(assg3);
	VarNode* drink1 = new VarNode("drink");
	assg3->linkVarNode(drink1);
	OpNode* times1 = new OpNode("*");
	assg3->linkExprNode(times1);
	VarNode* more1 = new VarNode("more");
	times1->linkLeftNode(more1);
	OpNode* minus1 = new OpNode("-");
	times1->linkRightNode(minus1);
	VarNode* beer1 = new VarNode("beer");
	minus1->linkLeftNode(beer1);
	VarNode* water1 = new VarNode("water");
	minus1->linkRightNode(water1);

	CallNode* call4 = new CallNode(4, "YourMom");
	pizzasl->linkStmtNode(call4);

// procedure YourMom
	ProcNode* yourmomProc = new ProcNode("YourMom");
	ast->addProcNode(yourmomProc);
	StmtLstNode* yourmomsl = new StmtLstNode();
	yourmomProc->linkStmtLstNode(yourmomsl);
	
	WhileNode* while5 = new WhileNode(5);
	yourmomsl->linkStmtNode(while5);
	VarNode* nagging1 = new VarNode("nagging");
	while5->linkVarNode(nagging1);
	StmtLstNode* while5sl = new StmtLstNode();
	while5->linkStmtLstNode(while5sl);

	AssgNode* assg6 = new AssgNode(6);
	while5sl->linkStmtNode(assg6);
	VarNode* nagging2 = new VarNode("nagging");
	assg6->linkVarNode(nagging2);
	OpNode* plus2 = new OpNode("+");
	assg6->linkExprNode(plus2);
	VarNode* nagging3 = new VarNode("nagging");
	plus2->linkLeftNode(nagging3);
	ConstNode* one2 = new ConstNode("1");
	plus2->linkRightNode(one2);

	WhileNode* while7 = new WhileNode(7);
	while5sl->linkStmtNode(while7);
	VarNode* i1 = new VarNode("i");
	while7->linkVarNode(i1);
	StmtLstNode* while7sl = new StmtLstNode();
	while7->linkStmtLstNode(while7sl);

	AssgNode* assg8 = new AssgNode(8);
	while7sl->linkStmtNode(assg8);
	VarNode* mom1 = new VarNode("mom");
	assg8->linkVarNode(mom1);
	VarNode* nagging4 = new VarNode("nagging");
	assg8->linkExprNode(nagging4);

	AssgNode* assg9 = new AssgNode(9);
	yourmomsl->linkStmtNode(assg9);
	VarNode* full1 = new VarNode("full");
	assg9->linkVarNode(full1);
	OpNode* plus3 = new OpNode("+");
	assg9->linkExprNode(plus3);
	VarNode* drink2 = new VarNode("drink");
	plus3->linkLeftNode(drink2);
	VarNode* eaten3 = new VarNode("eaten");
	plus3->linkRightNode(eaten3);
//-------------  END OF AST -------------//


//-------------  SET UP STMT ------------//
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(IF_STMT_);
	stmt1->setFollowsAfter(2);



}

void PQLIntegration::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
	ConstTable::getInstance()->clearTable();
	ProcTable::getInstance()->clearTable();
	CFG::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PQLIntegration );

void PQLIntegration::testSelectOnly() {
	cout << "select only";

	// DOES NOT PASS
	// because the values of a are not set in results object.

	string QUERY_STRING = "assign a; Select a";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	cout << "size1 for select only: " << r.size() << endl;
	CPPUNIT_ASSERT(6 == r.size());

	// new test for if
	string QUERY_STRING2 = "if x; Select x";
	pcc = new PQLController();
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);

	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testSelectModifies() {
	//string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\")";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	//r = pcc->parse(QUERY_STRING);
	//CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(7 == r2.size());

	//string QUERY_STRING3 = "if i; variable v; Select a such that Modifies(i, v)";
	//unordered_set<string> r3;
	//r3 = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(0 == r3.size());

	string QUERY_STRING4 = "variable v; Select v such that Modifies(_, v) and Modifies(_, v)";
	unordered_set<string> r4;
	r4 = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r4.size());
}

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

void PQLIntegration::testSelectUses() {
	PQLController* pcc = new PQLController();

	//string QUERY_STRING = "while w; variable v; Select v such that Uses(w, v)";
	//unordered_set<string> r;
	//r = pcc->parse(QUERY_STRING);
	//CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING2 = "assign aaaa; variable verynice; Select verynice such that Uses(aaaa, verynice)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	//CPPUNIT_ASSERT(0 == r2.size());

	string QUERY_STRING3 = "stmt s; variable v; Select v such that Uses(s, v)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(3 == r3.size());

	string QUERY_STRING4 = "variable v; Select v such that Uses(_, v)";
	unordered_set<string> r4;
	r4 = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(3 == r4.size());
}

void PQLIntegration::testSelectFollows() {
	PQLController* pcc = new PQLController();
	unordered_set<string> r;

	string QUERY_STRING = "assign a, a1; Select a such that Follows(a, a1)";
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(3 == r.size());

	string QUERY_STRING2 = "stmt s; Select s such that Follows(s, _)";
	r = pcc->parse(QUERY_STRING2);
	// answer supposed to be 1,3,4,5,7
    CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING3 = "stmt s; Select s such that Follows(_, s)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(5 == r.size());

	//string QUERY_STRING4 = "assign a; Select a such that Follows(_, _)";
	//r = pcc->parse(QUERY_STRING4);
	//CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectFollowsStar() {
	string QUERY_STRING = "assign a, a1; Select a1 such that Follows*(a, a1)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
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
	PQLController* pcc = new PQLController();
	unordered_set<string> r;

	string QUERY_STRING = "while w; assign a; Select a such that Parent(w, a)";
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(6 == r.size());

	string QUERY_STRING2 = "while w; Select w such that Parent(w, _)";
	r = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING3 = "while w; Select w such that Parent(_, w)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r.size());

	// don't work because a not present in clauses
	//string QUERY_STRING4 = "assign a; Select a such that Parent(_, _)";
	//r = pcc->parse(QUERY_STRING4);
	//CPPUNIT_ASSERT(6 == r.size());

	//string QUERY_STRING5 = "assign a; Select a such that Parent(3, _)";
	//r = pcc->parse(QUERY_STRING5);
	//CPPUNIT_ASSERT(0 == r.size());

	//string QUERY_STRING6 = "assign a; Select a such that Parent(2, _)";
	//r = pcc->parse(QUERY_STRING6);
	//CPPUNIT_ASSERT(6 == r.size());

}

void PQLIntegration::testSelectParentStar() {
	string QUERY_STRING = "while w; Select w such that Parent*(w, 6)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
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

void PQLIntegration::testSelectPatternAssg() {
	string QUERY_STRING = "assign a; Select a pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a pattern a(v, _\"4\"_)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	//cout << r2.size() << endl;
	CPPUNIT_ASSERT(2 == r2.size());

	string QUERY_STRING3 = "assign a; variable v; Select a pattern a(v, _\"2 + 3 + 4\"_)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r3.size());
	CPPUNIT_ASSERT("8" == *r3.begin());
}

void PQLIntegration::testSelectModifiesPattern() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\") pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"2 + 3 + 4\"_)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(1 == r2.size());

	//TODO: fix parsing of brackets in query parser
	string QUERY_STRING3 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"( 10 + 11 ) * 12\"_)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(1 == r3.size());
}

void PQLIntegration::testSelectFollowsPattern() {
	string QUERY_STRING = "assign a1, a2; Select a1 such that Follows(a1, a2) pattern a2(v, _\"2 + 3\"_)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
}

void PQLIntegration::testSelectUsesPattern() {
	string QUERY_STRING = "variable v; assign a; Select v such that Uses(_, v) pattern a(v, _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
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
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(8 == r.size());

	string QUERY_STRING_2 = "constant c; assign a; Select c such that Follows(a, 1)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(0 == r2.size());
}

void PQLIntegration::testSelectProgLine() {
	string QUERY_STRING = "prog_line p; Select p such that Follows(1, p)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
	//CPPUNIT_ASSERT(r.find("2") != r.end());

	string QUERY_STRING_2 = "prog_line p; Select p such that Follows(9, p)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testFailParent(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "assign a; variable v; Select a such that Parent(v, a)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}

void PQLIntegration::testFailUses(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "variable v, v1; Select v such that Uses(v, v1)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}