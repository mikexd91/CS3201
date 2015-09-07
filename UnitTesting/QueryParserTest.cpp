#include "QueryParserTest.h"
#include "../SPA/QueryParser.h"
#include "../SPA/Query.h"
#include "../SPA/StringPair.h"
#include "../SPA/Utils.h"
#include "../SPA/PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "../SPA/Clause.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/FollowsStarClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/ParentClause.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/PatternClause.h"
#include "../SPA/UsesClause.h"
#include "../SPA/PatternAssgClause.h"
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

void QueryParserTest::setUp(){
}

void QueryParserTest::tearDown(){
}

CPPUNIT_TEST_SUITE_REGISTRATION( QueryParserTest);

void QueryParserTest::testTokeniser(){
	string testString = "this is  the   string     to be   split";
	vector<string> testResult = QueryParser::tokeniser(testString, ' ');
	CPPUNIT_ASSERT("this" == testResult.at(0));
	CPPUNIT_ASSERT("is" == testResult.at(1));
	CPPUNIT_ASSERT("the" == testResult.at(2));
	CPPUNIT_ASSERT("string" == testResult.at(3));
	CPPUNIT_ASSERT("to" == testResult.at(4));
	CPPUNIT_ASSERT("be" == testResult.at(5));
	CPPUNIT_ASSERT("split" == testResult.at(6));
}

void QueryParserTest::testQueueBuilder(){
	string testString = "Select a such that wtf(a,b)";
	queue<string> testQueue = QueryParser::queueBuilder(testString);
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "Select");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "a");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "such");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "that");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "wtf");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "(");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "a");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ",");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "b");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ")");
}

void QueryParserTest::testDeclaration(){
	
	Query* result2 = new Query();
	string const USER_INPUT2 = "constant c; prog_line p";
	
	vector<string> testList2 = QueryParser::tokeniser(USER_INPUT2, ';');
	QueryParser::parseDeclarations(result2, testList2);

	boost::unordered_map<string, string> decList2 = result2->getDeclarationList();
	CPPUNIT_ASSERT(decList2.at("c") == stringconst::ARG_CONSTANT);
	CPPUNIT_ASSERT(decList2.at("p") == stringconst::ARG_STATEMENT);

}

void QueryParserTest::testSelect(){
	Query* result = new Query();
	string const USER_INPUT1 = "prog_line a;";
	vector<string> testList = QueryParser::tokeniser(USER_INPUT1, ';');
	QueryParser::parseDeclarations(result, testList);
	unordered_map<string, string> decs = result->getDeclarationList();
	CPPUNIT_ASSERT(decs.at("a") == stringconst::ARG_STATEMENT);

	string const USER_INPUT2 = "Select a";
	queue<string> line = QueryParser::queueBuilder(USER_INPUT2);

	QueryParser::parseSelectSynonyms(result, line);

	unordered_map<string, string> decList = result->getDeclarationList();
	CPPUNIT_ASSERT(decs.at("a") == decList.at("a"));

	string first = Utils::getWordAndPop(line);
	CPPUNIT_ASSERT(first == stringconst::STRING_SELECT);
}

void QueryParserTest::testClause(){
	Query* ASSERTION = new Query();
	string const DEC_LINE = "assign a;";
	string const SEL_LINE = "Select a";
	string const CLS_LINE = "Follows(a, _)";
	
	vector<string> DEC_LIST = QueryParser::tokeniser(DEC_LINE, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);
	boost::unordered_map<string, string> declist = ASSERTION->getDeclarationList();
	CPPUNIT_ASSERT(declist.at("a") == stringconst::ARG_ASSIGN);
	
	queue<string> SEL_Q = QueryParser::queueBuilder(SEL_LINE);
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);
	
	queue<string> CLS_Q = QueryParser::queueBuilder(CLS_LINE);
	QueryParser::parseClause(ASSERTION, CLS_Q);
	
	unordered_map<string, string> dec = ASSERTION->getDeclarationList();
	vector<StringPair> sel = ASSERTION->getSelectList();
	vector<Clause*> cls = ASSERTION->getClauseList();
	
	CPPUNIT_ASSERT(dec.at("a") == stringconst::ARG_ASSIGN);
	CPPUNIT_ASSERT(dec.at("v") == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(sel.at(0).getFirst() == "a");
	CPPUNIT_ASSERT(sel.at(0).getSecond() == stringconst::ARG_ASSIGN);
	
	Clause* TEST = cls.at(0);
	bool FIRST_FIXED = TEST->getFirstArgFixed();
	CPPUNIT_ASSERT(!FIRST_FIXED);
	CPPUNIT_ASSERT(TEST->getFirstArg() == "a");
	CPPUNIT_ASSERT(TEST->getFirstArgType() == stringconst::ARG_ASSIGN);
	bool SECOND_FIXED = TEST->getSecondArgFixed();
	CPPUNIT_ASSERT(!SECOND_FIXED);
	CPPUNIT_ASSERT(TEST->getSecondArg() == "_");
	CPPUNIT_ASSERT(TEST->getSecondArgType() == stringconst::ARG_GENERIC);
	
}

/*
void QueryParserTest::testPattern(){
	string const DECLARATION = "assign a;";
	string const SELECT = "Select a such that";
	string const PATTERN = "pattern a(\"v\", _)";

	Query* ASSERTION = new Query();

	vector<string> DEC_LIST = QueryParser::tokeniser(DECLARATION, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SELECT);
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);

	queue<string> PAT_Q = QueryParser::queueBuilder(PATTERN);
	QueryParser::parsePattern(ASSERTION, PAT_Q);
	
	Clause* c = ASSERTION->getClauseList().at(0);
	PatternAssgClause* PATTERN_ASS = dynamic_cast<PatternAssgClause*>(c);
	string EXPR = PATTERN_ASS->getExpression();
	CPPUNIT_ASSERT(EXPR == stringconst::STRING_EMPTY);
	//fix pattern parsing other expressions.
	string token = "a(v, _\"";
	queue<string> q = QueryParser::queueBuilder(token);
	string one = Utils::getWordAndPop(q);
	string two = Utils::getWordAndPop(q);
	CPPUNIT_ASSERT(! (!contains(two, "_") || !contains(two, "\"")));
}
*/
void QueryParserTest::testParser(){
	//string const USER_INPUT1 = "assign a; variable v; Select a pattern a(\"v\", _\"x+y\"_) and Modifies(a, v) and pattern a(v, _)";
	string const USER_INPUT1 = "prog_line p; variable v; Select p such that Uses(p, v)";
	Query q1 = QueryParser::parseQuery(USER_INPUT1);

	Query* Q1 = new Query();
	StringPair dec1a = StringPair();
	StringPair dec1b = StringPair();
	StringPair syn1a = StringPair();
	syn1a.setFirst("p"); syn1a.setSecond(stringconst::ARG_STATEMENT);
	dec1a.setFirst("p"); dec1a.setSecond(stringconst::ARG_STATEMENT);
	dec1b.setFirst("v"); dec1b.setSecond(stringconst::ARG_VARIABLE);
	
	Q1->addSelectSynonym(syn1a);
	Q1->addDeclaration(dec1a);
	Q1->addDeclaration(dec1b);

	unordered_map<string, string> dec_Q1 = Q1->getDeclarationList();
	unordered_map<string, string> dec_q1 = q1.getDeclarationList();
	vector<StringPair> sel_Q1 = Q1->getSelectList();
	vector<StringPair> sel_q1 = q1.getSelectList();

	string q1_dec_a = dec_q1.at("p");
	string Q1_dec_a = dec_Q1.at("p");
	string q1_dec_b = dec_q1.at("v");
	string Q1_dec_b = dec_Q1.at("v");
	StringPair q1_sel_a = sel_q1.at(0);
	StringPair Q1_sel_a = sel_Q1.at(0); 

	CPPUNIT_ASSERT(q1_dec_a == Q1_dec_a);
	CPPUNIT_ASSERT(q1_dec_b == Q1_dec_b);
	CPPUNIT_ASSERT(q1_sel_a.getFirst() == Q1_sel_a.getFirst());
	CPPUNIT_ASSERT(q1_sel_a.getSecond() == Q1_sel_a.getSecond());
	
	vector<Clause*> cls_q1 = q1.getClauseList();
	
	UsesClause* use1_q1 = dynamic_cast<UsesClause*>(cls_q1.at(0));
	CPPUNIT_ASSERT(use1_q1->getFirstArg() == "p");
	CPPUNIT_ASSERT(use1_q1->getFirstArgType() == stringconst::ARG_STATEMENT);
	CPPUNIT_ASSERT(use1_q1->getSecondArg() == "v");

	PatternAssgClause* pac1_q1 = dynamic_cast<PatternAssgClause*>(cls_q1.at(0));
	CPPUNIT_ASSERT(pac1_q1->getExpression() == "_\"x y +\"_");
	CPPUNIT_ASSERT(pac1_q1->getSynonym() == "a");

	ModifiesClause* mod1_q1 = dynamic_cast<ModifiesClause*>(cls_q1.at(1));
	CPPUNIT_ASSERT(mod1_q1->getSecondArg() == "v");
	CPPUNIT_ASSERT(mod1_q1->getFirstArg() == "a");

	PatternAssgClause* pac2_q1 = dynamic_cast<PatternAssgClause*>(cls_q1.at(2));
	CPPUNIT_ASSERT(pac2_q1->getExpression() == stringconst::STRING_EMPTY);
	CPPUNIT_ASSERT(pac2_q1->getSynonym() == "a");
}