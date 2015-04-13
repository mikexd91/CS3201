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
	string const WORDS = "One two three four five";
	vector<string> wordList = QueryParser::tokeniser(WORDS, ' ');

	CPPUNIT_ASSERT(wordList.at(0) == "One");
	CPPUNIT_ASSERT(wordList.at(1) == "two");
	CPPUNIT_ASSERT(wordList.at(2) == "three");
	CPPUNIT_ASSERT(wordList.at(3) == "four");
	CPPUNIT_ASSERT(wordList.at(4) == "five");
}

void QueryParserTest::testDeclaration(){
	
	Query* result = new Query();
	string const USER_INPUT1 = "assign a, a1";
	
	vector<string> testList = QueryParser::tokeniser(USER_INPUT1, ';');
	QueryParser::parseDeclarations(result, testList);

	boost::unordered_map<string, string> decList = result->getDeclarationList();
	CPPUNIT_ASSERT(decList.at("a") == stringconst::ARG_ASSIGN);
	CPPUNIT_ASSERT(decList.at("a1") == stringconst::ARG_ASSIGN);
	
	Query* result2 = new Query();
	string const USER_INPUT2 = "assign a; variable v";
	
	vector<string> testList2 = QueryParser::tokeniser(USER_INPUT2, ';');
	QueryParser::parseDeclarations(result2, testList2);

	//boost::unordered_map<string, string> decList2 = result2->getDeclarationList();
	//CPPUNIT_ASSERT(decList2.at("a") == stringconst::ARG_ASSIGN);
	//CPPUNIT_ASSERT(decList2.at("v") == stringconst::ARG_VARIABLE);

}

void QueryParserTest::testSelect(){
	Query* result = new Query();
	string const USER_INPUT1 = "assign a, a1";
	vector<string> testList = QueryParser::tokeniser(USER_INPUT1, ';');
	QueryParser::parseDeclarations(result, testList);
	unordered_map<string, string> decs = result->getDeclarationList();
	CPPUNIT_ASSERT(decs.at("a") == stringconst::ARG_ASSIGN);

	string const USER_INPUT2 = "Select a";
	queue<string> line = QueryParser::queueBuilder(USER_INPUT2, ' ');
	/*
	string const sel1 = "Select a";
	queue<string> string1 = QueryParser::queueBuilder(sel1, ' ');
	QueryParser::parseSelectSynonyms(result, string1);
	vector<StringPair> resultPair = result->getSelectList();
	StringPair pair1 = resultPair.at(0);
	CPPUNIT_ASSERT(pair1.getFirst() == "a");
	CPPUNIT_ASSERT(pair1.getSecond() == stringconst::ARG_ASSIGN);*/
	//CPPUNIT_ASSERT(Utils::getWordAndPop(line) == "Select");
	//CPPUNIT_ASSERT(Utils::getWordAndPop(line) == "a");
	unordered_map<string, string> decList = result->getDeclarationList();
	CPPUNIT_ASSERT(decs.at("a") == decList.at("a"));

	string first = Utils::getWordAndPop(line);
	CPPUNIT_ASSERT(first == stringconst::STRING_SELECT);
}

void QueryParserTest::testClause(){
	Query* ASSERTION = new Query();
	string const DEC_LINE = "assign a;variable v;";
	string const SEL_LINE = "Select a";
	string const CLS_LINE = "Follows(a, v)";

	vector<string> DEC_LIST = QueryParser::tokeniser(DEC_LINE, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SEL_LINE, ' ');
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);
	
	queue<string> CLS_Q = QueryParser::queueBuilder(CLS_LINE, ' ');
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
	CPPUNIT_ASSERT(TEST->getSecondArg() == "v");
	CPPUNIT_ASSERT(TEST->getSecondArgType() == stringconst::ARG_VARIABLE);
}

void QueryParserTest::testPattern(){
	string const DECLARATION = "assign a;";
	string const SELECT = "Select a such that";
	string const PATTERN = "Pattern a(\"v\", _\"x\"_)";

	Query* ASSERTION = new Query();

	vector<string> DEC_LIST = QueryParser::tokeniser(DECLARATION, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SELECT, ' ');
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);

	queue<string> PAT_Q = QueryParser::queueBuilder(PATTERN, ' ');
	QueryParser::parsePattern(ASSERTION, PAT_Q);
	/*
	Clause* c = ASSERTION->getClauseList().at(0);
	PatternAssgClause* PATTERN_ASS = dynamic_cast<PatternAssgClause*>(c);
	string EXPR = PATTERN_ASS->getExpression();
	CPPUNIT_ASSERT(EXPR == stringconst::STRING_EMPTY);*/
	//fix pattern parsing other expressions.
	string token = "a(v, _\"";
	queue<string> q = QueryParser::queueBuilder(token, ' ');
	string one = Utils::getWordAndPop(q);
	string two = Utils::getWordAndPop(q);
	//CPPUNIT_ASSERT(! (!contains(two, "_") || !contains(two, "\"")));
}

void QueryParserTest::testParser(){
	string const USER_INPUT1 = "assign a;variable v;Select a such that Uses(a, v)";
	string const USER_INPUT2 = "Assign a, a1; Select a such that Follows(a, a1)";
	string const USER_INPUT3 = "Assign a; Variable v; Select a such that Pattern a(v, _)";
	string const USER_INPUT4 = "Assign a; Variable v; Select a such that Pattern a(v, _\" x + y \"_)";
	string const USER_INPUT5 = "Assign a, a1; Select a such that Follows*(a, a1)";
	string const USER_INPUT6 = "Assign a, a1; Select a such that Parent(a, a1)";
	string const USER_INPUT7 = "Assign a, a1; Select a such that Parent*(a, a1)";
	string const USER_INPUT8 = "Assign a, a1; Select a such that Modifies(a, a1)";

	Query q1 = QueryParser::parseQuery(USER_INPUT1);
	//Query q2 = QueryParser::queryProcessor(USER_INPUT2);
	//Query q3 = QueryParser::queryProcessor(USER_INPUT3);
	//Query q4 = QueryParser::queryProcessor(USER_INPUT4);

	Query* Q1 = new Query();
	StringPair dec1a = StringPair();
	StringPair dec1b = StringPair();
	StringPair syn1a = StringPair();
	syn1a.setFirst("a"); syn1a.setSecond(stringconst::ARG_ASSIGN);
	dec1a.setFirst("a"); dec1a.setSecond(stringconst::ARG_ASSIGN);
	dec1b.setFirst("v"); dec1b.setSecond(stringconst::ARG_VARIABLE);
	UsesClause* clause1a = new UsesClause();
	clause1a->setFirstArg("a"); clause1a->setFirstArgFixed(false); clause1a->setFirstArgType(stringconst::ARG_ASSIGN);
	clause1a->setFirstArg("v"); clause1a->setFirstArgFixed(false); clause1a->setFirstArgType(stringconst::ARG_VARIABLE);
	Q1->addSelectSynonym(syn1a);
	Q1->addDeclaration(dec1a);
	Q1->addDeclaration(dec1b);
	Q1->addClause(clause1a);

	unordered_map<string, string> dec_Q1 = Q1->getDeclarationList();
	unordered_map<string, string> dec_q1 = q1.getDeclarationList();
	//vector<StringPair> sel_Q1 = Q1->getSelectList();
	//vector<StringPair> sel_q1 = q1.getSelectList();

	string q1_dec_a = dec_q1.at("a");
	string Q1_dec_a = dec_Q1.at("a");
	//string q1_dec_b = dec_q1.at("v");
	//string Q1_dec_b = dec_Q1.at("v");
	//StringPair q1_sel_a = sel_q1.at(0);
	//StringPair Q1_sel_a = sel_Q1.at(0); 
	CPPUNIT_ASSERT(q1_dec_a == Q1_dec_a);
	//CPPUNIT_ASSERT(q1_dec_b == Q1_dec_b);
	//CPPUNIT_ASSERT(q1_sel_a.getFirst() == Q1_sel_a.getFirst());
	//CPPUNIT_ASSERT(q1_sel_a.getSecond() == Q1_sel_a.getSecond());
}