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
#include "../SPA/PatternWhileClause.h"
#include "../SPA/PatternIfClause.h"
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
	string testString = "  this is  the   string     to be   split";
	vector<string> testResult = QueryParser::tokeniser(testString, ' ');
	CPPUNIT_ASSERT("this" == testResult.at(0));
	CPPUNIT_ASSERT("is" == testResult.at(1));
	CPPUNIT_ASSERT("the" == testResult.at(2));
	CPPUNIT_ASSERT("string" == testResult.at(3));
	CPPUNIT_ASSERT("to" == testResult.at(4));
	CPPUNIT_ASSERT("be" == testResult.at(5));
	CPPUNIT_ASSERT("split" == testResult.at(6));
	//string test = "assign a, a1; Select my balls";
	//vector<string> result = QueryParser::tokeniser(test, ';');
	//CPPUNIT_ASSERT(result.at(0) == "assign a, a1");
	//CPPUNIT_ASSERT(result.at(1) == " Select my balls");
}

void QueryParserTest::testQueueBuilder(){
	string testString = "Select a such that pattern a(_  , _) and a1(_,_)";
	queue<string> testQueue = QueryParser::queueBuilder(testString);
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "Select");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "a");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "such");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "that");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "pattern");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "a");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "(");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "_");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ",");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "_");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ")");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "and");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "a1");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "(");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "_");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ",");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == "_");
	CPPUNIT_ASSERT(Utils::getWordAndPop(testQueue) == ")");
}

void QueryParserTest::testDeclaration(){
	
	Query* result2 = new Query();
	string const USER_INPUT2 = "constant c; prog_line p; assign a, a1;";
	
	vector<string> testList2 = QueryParser::tokeniser(USER_INPUT2, ';');
	QueryParser::parseDeclarations(result2, testList2);

	boost::unordered_map<string, string> decList2 = result2->getDeclarationList();
	CPPUNIT_ASSERT(decList2.at("c") == stringconst::ARG_CONSTANT);
	CPPUNIT_ASSERT(decList2.at("p") == stringconst::ARG_STATEMENT);
	CPPUNIT_ASSERT(decList2.at("a") == stringconst::ARG_ASSIGN);
	CPPUNIT_ASSERT(decList2.at("a1") == stringconst::ARG_ASSIGN);
}

void QueryParserTest::testSelect(){
	Query* result = new Query();
	string const USER_INPUT1 = "prog_line a, a1;";
	vector<string> testList = QueryParser::tokeniser(USER_INPUT1, ';');
	QueryParser::parseDeclarations(result, testList);
	unordered_map<string, string> decs = result->getDeclarationList();
	CPPUNIT_ASSERT(decs.at("a") == stringconst::ARG_STATEMENT);

	//string const USER_INPUT2 = "Select BOOLEAN";
	string const USER_INPUT2 = "Select <a, a1>";
	//string const USER_INPUT2 = "Select a";
	//string const USER_INPUT2 = "Select a.dongers";
	queue<string> line = QueryParser::queueBuilder(USER_INPUT2);
	/*
	while (!line.empty()){
		string asd = Utils::getWordAndPop(line);
		cout << asd << "\n";
	}
	*/
	QueryParser::parseSelectSynonyms(result, line);
	vector<StringPair> asd = result->getSelectList();
	
	//test bool
	//StringPair boolpair = asd.at(0);
	//CPPUNIT_ASSERT(boolpair.getFirst() == "BOOLEAN");
	//CPPUNIT_ASSERT(boolpair.getSecond() == stringconst::ARG_BOOLEAN);
	
	//test tuple
	StringPair tuplepair1 = asd.at(0);
	StringPair tuplepair2 = asd.at(1);
	CPPUNIT_ASSERT(tuplepair1.getFirst() == "a");
	CPPUNIT_ASSERT(tuplepair1.getSecond() == stringconst::ARG_STATEMENT);
	CPPUNIT_ASSERT(tuplepair2.getFirst() == "a1");
	CPPUNIT_ASSERT(tuplepair2.getSecond() == stringconst::ARG_STATEMENT);

	//test single
	//StringPair single = asd.at(0);
	//CPPUNIT_ASSERT(single.getFirst() == "a");
	//CPPUNIT_ASSERT(single.getSecond() == stringconst::ARG_STATEMENT);

	//test attr
	//StringPair single = asd.at(0);
	//CPPUNIT_ASSERT(single.getFirst() == "a");
	//CPPUNIT_ASSERT(single.getSecond() == stringconst::ARG_STATEMENT);
	//CPPUNIT_ASSERT(single.getAttribute() == "dongers");
}

void QueryParserTest::testClause(){
	Query* ASSERTION = new Query();
	string const DEC_LINE = "assign a, a1;";
	string const SEL_LINE = "Select a";
	string const CLS_LINE = "Follows(\"a\", \"a1\")";
	
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
	CPPUNIT_ASSERT(sel.at(0).getFirst() == "a");
	CPPUNIT_ASSERT(sel.at(0).getSecond() == stringconst::ARG_ASSIGN);
	
	Clause* TEST = cls.at(0);
	TEST->getClauseType();
}

void QueryParserTest::testPatternAssign(){
	string const DECLARATION = "assign a;";
	string const SELECT = "Select a such that";
	string const PATTERN = "a(\"x\", \"x + y \")";

	Query* ASSERTION = new Query();

	vector<string> DEC_LIST = QueryParser::tokeniser(DECLARATION, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SELECT);
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);

	queue<string> PAT_Q = QueryParser::queueBuilder(PATTERN);
	QueryParser::parsePattern(ASSERTION, PAT_Q);
	
	Clause* c = ASSERTION->getClauseList().at(0);
	PatternAssgClause* PATTERN_ASS = dynamic_cast<PatternAssgClause*>(c);
	CPPUNIT_ASSERT(PATTERN_ASS->getSyn() == "a");
	CPPUNIT_ASSERT(PATTERN_ASS->getVar() == "x");
	CPPUNIT_ASSERT(PATTERN_ASS->isVarFixed() == true);
	CPPUNIT_ASSERT(PATTERN_ASS->getExpression() == "\"x y +\"");
}

void QueryParserTest::testPatternIf(){
	string const DECLARATION = "if i;";
	string const SELECT = "Select i such that";
	string const PATTERN = "i (\"x\", _,_)";

	Query* ASSERTION = new Query();
	
	vector<string> DEC_LIST = QueryParser::tokeniser(DECLARATION, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SELECT);
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);

	queue<string> PAT_Q = QueryParser::queueBuilder(PATTERN);
	QueryParser::parsePattern(ASSERTION, PAT_Q);

	Clause* c = ASSERTION->getClauseList().at(0);
	PatternIfClause* PATTERN_IF = dynamic_cast<PatternIfClause*>(c);
	CPPUNIT_ASSERT(PATTERN_IF->getThenList() == stringconst::STRING_EMPTY);
	CPPUNIT_ASSERT(PATTERN_IF->getElseList() == stringconst::STRING_EMPTY);
	CPPUNIT_ASSERT(PATTERN_IF->getSyn() == "i");
	CPPUNIT_ASSERT(PATTERN_IF->getVar() == "x");
	CPPUNIT_ASSERT(PATTERN_IF->getVarType() == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(PATTERN_IF->isVarFixed() == true);
}

void QueryParserTest::testPatternWhile(){
	string const DECLARATION = "while w;";
	string const SELECT = "Select w such that";
	string const PATTERN = "w (\"x\", _)";

	Query* ASSERTION = new Query();
	
	vector<string> DEC_LIST = QueryParser::tokeniser(DECLARATION, ';');
	QueryParser::parseDeclarations(ASSERTION, DEC_LIST);

	queue<string> SEL_Q = QueryParser::queueBuilder(SELECT);
	QueryParser::parseSelectSynonyms(ASSERTION, SEL_Q);

	queue<string> PAT_Q = QueryParser::queueBuilder(PATTERN);
	QueryParser::parsePattern(ASSERTION, PAT_Q);

	Clause* c = ASSERTION->getClauseList().at(0);
	PatternWhileClause* PATTERN_W = dynamic_cast<PatternWhileClause*>(c);
	CPPUNIT_ASSERT(PATTERN_W->getSyn() == "w");
	CPPUNIT_ASSERT(PATTERN_W->getVar() == "x");
	CPPUNIT_ASSERT(PATTERN_W->isVarFixed() == true);
	CPPUNIT_ASSERT(PATTERN_W->getStmtList() == stringconst::STRING_EMPTY);
}

void QueryParserTest::testWith(){
	string const USER_IN = "p.progline = 2";
	queue<string> WITH_Q = QueryParser::queueBuilder(USER_IN);
	Query* ASSERTION = new Query();
	QueryParser::parseWith(ASSERTION, WITH_Q);
}

void QueryParserTest::testParser(){
	//string const USER_INPUT1 = "assign a; variable v; Select a pattern a(\"v\", _\"x+y\"_) and Modifies(a, v) and pattern a(v, _)";
	string const USER_INPUT1 = "assign a, a1; Select a such that pattern a(_, _) and a1(_, _)";
	
	Query output = QueryParser::parseQuery(USER_INPUT1);
	unordered_map<string, string> decList = output.getDeclarationList();
	CPPUNIT_ASSERT(decList.at("a") == stringconst::ARG_ASSIGN);
	CPPUNIT_ASSERT(decList.at("a1") == stringconst::ARG_ASSIGN);

	Clause* c1 = output.getClauseList().at(0);
	Clause* c2 = output.getClauseList().at(1);
	PatternAssgClause* a1 = dynamic_cast<PatternAssgClause*>(c1);
	PatternAssgClause* a2 = dynamic_cast<PatternAssgClause*>(c2);

	//CPPUNIT_ASSERT(a1->getFirstArg() == "a");
	//CPPUNIT_ASSERT(a1->getFirstArgType() == stringconst::ARG_ASSIGN);
	//CPPUNIT_ASSERT(a1->getSecondArg() == stringconst::STRING_EMPTY);
	//CPPUNIT_ASSERT(a1->getExpression() == stringconst::STRING_EMPTY);

	//CPPUNIT_ASSERT(a2->getFirstArg() == "a1");
	//CPPUNIT_ASSERT(a2->getFirstArgType() == stringconst::ARG_ASSIGN);
	//CPPUNIT_ASSERT(a2->getSecondArg() == stringconst::STRING_EMPTY);
	//CPPUNIT_ASSERT(a2->getExpression() == stringconst::STRING_EMPTY);
}