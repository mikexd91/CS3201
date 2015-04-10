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
//#include "../SPA/UsesClause.h"
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

void QueryParserTest::setUp(){
}

void QueryParserTest::tearDown(){
}

CPPUNIT_TEST_SUITE_REGISTRATION( QueryParserTest);

void QueryParserTest::testParser(void){
	string const USER_INPUT1 = "Assign a; Variable v; Select a";// such that Uses(a, v)";
	string const USER_INPUT2 = "Assign a, a1; Select a such that Follows(a, a1)";
	string const USER_INPUT3 = "Assign a; Variable v; Select a such that Pattern a(v, _)";
	string const USER_INPUT4 = "Assign a; Variable v; Select a such that Pattern a(v, _\" x + y \"_)";

	Query q1 = QueryParser::queryProcessor(USER_INPUT1);
	Query q2 = QueryParser::queryProcessor(USER_INPUT2);
	Query q3 = QueryParser::queryProcessor(USER_INPUT3);
	Query q4 = QueryParser::queryProcessor(USER_INPUT4);

	Query* Q1 = new Query();
	StringPair dec1a = StringPair();
	StringPair dec1b = StringPair();
	StringPair syn1a = StringPair();
	syn1a.setFirst("a"); syn1a.setSecond(stringconst::ARG_ASSIGN);
	dec1a.setFirst("a"); dec1a.setSecond(stringconst::ARG_ASSIGN);
	dec1b.setFirst("v"); dec1b.setSecond(stringconst::ARG_VARIABLE);
	/*UsesClause* clause1a = new UsesClause();
	clause1a->setFirstArg("a"); clause1a->setFirstArgFixed(false); clause1a->setFirstArgType(stringconst::ARG_ASSIGN);
	clause1a->setFirstArg("v"); clause1a->setFirstArgFixed(false); clause1a->setFirstArgType(stringconst::ARG_VARIABLE);
	*/Q1->addSelectSynonym(syn1a);
	Q1->addDeclaration(dec1a);
	Q1->addDeclaration(dec1b);
	//Q1->addClause(clause1a);

	unordered_map<string, string> dec_Q1 = Q1->getDeclarationList();
	unordered_map<string, string> dec_q1 = q1.getDeclarationList();

	string q1_assign = dec_q1.at("a");
	string Q1_assign = dec_Q1.at("a");
	//CPPUNIT_ASSERT(q1_assign == Q1_assign);
}
