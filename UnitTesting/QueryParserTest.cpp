#include "QueryParserTest.h"
#include "../SPA/QueryParser.h"
#include "../SPA/Query.h"
#include "../SPA/StringPair.h"
#include "../SPA/Utils.h"
#include "../SPA/PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "../SPA/Clause.h"
#include "../SPA/SuchThatClause.h"
#include "../SPA/PatternClause.h"
#include "../SPA/WithClause.h"
#include "../SPA/CallsClause.h"
#include "../SPA/CallsStarClause.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/FollowsStarClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/ParentClause.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/UsesClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/PatternIfClause.h"
#include "../SPA/PatternWhileClause.h"
#include "boost/unordered_map.hpp"
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <iostream>
#include <string>
using std::string;
using std::iostream;
using std::queue;
using std::exception;
using std::stringstream;
using std::vector;
using boost::unordered_map;
using namespace boost;

QueryParser* parser;

void QueryParserTest::setUp(){
	parser = QueryParser::getInstance();
}

void QueryParserTest::tearDown(){
}

CPPUNIT_TEST_SUITE_REGISTRATION( QueryParserTest);

void QueryParserTest::testDeclaration(){
	parser = QueryParser::getInstance();
	string const DEC = "variable v; constant c; while w; stmt s; assign a; procedure p; if i; call cl";
	Query* Q = new Query();
	vector<string>* split = new vector<string>();
	parser->tokeniser(DEC, ';', split);
	parser->parseDeclarations(Q, split);
	unordered_map<string,string> declist = Q->getDeclarationList();
	CPPUNIT_ASSERT(declist.at("v") == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(declist.at("c") == "constant");
	CPPUNIT_ASSERT(declist.at("cl") == "call");
	CPPUNIT_ASSERT(declist.at("s") == "stmt");
	CPPUNIT_ASSERT(declist.at("w") == "while");
	CPPUNIT_ASSERT(declist.at("a") == "assign");
	CPPUNIT_ASSERT(declist.at("p") == "procedure");
	CPPUNIT_ASSERT(declist.at("i") == "if");
}

void QueryParserTest::testSelectSingle(){
	parser = QueryParser::getInstance();
	string const DEC = "variable v; constant c; while w; stmt s; assign a; procedure p; if i; call cl";
	string const SEL = "Select v such that";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	vector<StringPair> L_SEL = Q->getSelectList();
	StringPair SEL_V = L_SEL.at(0);
	CPPUNIT_ASSERT(SEL_V.getFirst() == "v");
	CPPUNIT_ASSERT(SEL_V.getSecond() == "variable");
}

void QueryParserTest::testSelectBoolean(){
	parser = QueryParser::getInstance();
	string const DEC = "variable v; constant c; while w; stmt s; assign a; procedure p; if i; call cl";
	string const SEL = "Select BOOLEAN with";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	vector<StringPair> L_SEL = Q->getSelectList();
	StringPair SEL_V = L_SEL.at(0);
	CPPUNIT_ASSERT(SEL_V.getFirst() == "BOOLEAN");
	CPPUNIT_ASSERT(SEL_V.getSecond() == stringconst::ARG_BOOLEAN);
}

void QueryParserTest::testSelectTuple(){
	parser = QueryParser::getInstance();
	string const DEC = "variable v; constant c; while w; stmt s; assign a; procedure p; if i; call cl";
	string const SEL = "Select <v, c> with";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	vector<StringPair> L_SEL = Q->getSelectList();
	StringPair SEL_V1 = L_SEL.at(0);
	StringPair SEL_V2 = L_SEL.at(1);
	CPPUNIT_ASSERT(SEL_V1.getFirst() == "v");
	CPPUNIT_ASSERT(SEL_V1.getSecond() == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(SEL_V2.getFirst() == "c");
	CPPUNIT_ASSERT(SEL_V2.getSecond() == stringconst::ARG_CONSTANT);
}

void QueryParserTest::testParseClauseUses(){
	parser = QueryParser::getInstance();
	string const DEC = "stmt s";
	string const SEL = "Select s";
	string const CLS = "Uses(1,s)";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	queue<string>* Q_CLS = new queue<string>();
	parser->queueBuilder(CLS, Q_CLS);
	parser->parseClause(Q, Q_CLS);
	vector<Clause*> V_Q_CLS = Q->getClauseList();
	CPPUNIT_ASSERT(V_Q_CLS.size() == 1);
	Clause* CL0 = V_Q_CLS.at(0);
	ClauseType CT0 = CL0->getClauseType();
	CPPUNIT_ASSERT(CT0 == USES_);
	SuchThatClause* SCL0 = dynamic_cast<SuchThatClause*>(CL0);
}

void QueryParserTest::testParseParentStar(){
	parser = QueryParser::getInstance();
	string const DEC = "stmt s";
	string const SEL = "Select s";
	string const CLS = "Parent*(1,s)";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	queue<string>* Q_CLS = new queue<string>();
	parser->queueBuilder(CLS, Q_CLS);
	parser->parseClause(Q, Q_CLS);
	vector<Clause*> V_Q_CLS = Q->getClauseList();
	CPPUNIT_ASSERT(V_Q_CLS.size() == 1);
	Clause* CL0 = V_Q_CLS.at(0);
	ClauseType CT0 = CL0->getClauseType();
	CPPUNIT_ASSERT(CT0 == PARENTSTAR_);
	//ParentStarClause* PSCL0 = ParentStarClause(CL0);
	//PSCL0->getFirstArg();
}

void QueryParserTest::testWith(){
	string const DECLARATION = "constant c;";
	string const USER_IN = "c.value = 1";
	vector<string> DEC_LIST = vector<string>();
	parser->tokeniser(DECLARATION, ';', &DEC_LIST);
	queue<string>* WITH_Q = new queue<string>();
	parser->queueBuilder(USER_IN, WITH_Q);
	Query* ASSERTION = new Query();
	parser->parseDeclarations(ASSERTION, &DEC_LIST);
	parser->parseWith(ASSERTION, WITH_Q);
	vector<Clause*> cl = ASSERTION->getClauseList();
	WithClause* w = dynamic_cast<WithClause*>(cl.at(0));
	ClauseType type = w->getClauseType();
	//CPPUNIT_ASSERT(type == WITH_);
	WithClauseRef wl = w->getLeftRef();
	WithClauseRef wr = w->getRightRef();
	CPPUNIT_ASSERT(wl.getEntity() == "c");
	CPPUNIT_ASSERT(wl.getRefType() == ATTRREF_);
	CPPUNIT_ASSERT(wl.getAttrType() == CONSTVALUE_);
	
	CPPUNIT_ASSERT(wr.getEntity() == "1");
	CPPUNIT_ASSERT(wr.getRefType() == INTEGER_);
	CPPUNIT_ASSERT(wr.getAttrType() == NULLATTR_);
}

void QueryParserTest::testParser(){
	string const INPUTLINE1 = "variable v; stmt s; Select s such that Uses(s,v) Follows*(1,2)";
	parser = QueryParser::getInstance();
	Query* QUERY = new Query();
	QUERY = parser->parseQuery(INPUTLINE1);
	vector<Clause*> VC = QUERY->getClauseList();
	CPPUNIT_ASSERT(VC.size() == 2);
}