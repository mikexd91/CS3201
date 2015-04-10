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

void QueryParserTest::setUp(){
	string const USER_INPUT1 = "Assign a; Variable v; Select a such that Uses(a, v)";
	string const USER_INPUT2 = "Assign a, a1; Select a such that Follows(a, a1)";
	string const USER_INPUT3 = "Assign a; Variable v; Select a such that Pattern a(v, _)";
	string const USER_INPUT4 = "Assign a; Variable v; Select a such that Pattern a(v, _\" x + y \"_)";
}

void QueryParserTest::tearDown(){
}

CPPUNIT_TEST_SUITE_REGISTRATION( QueryParserTest);

void QueryParserTest::testParser(void){
	string const USER_INPUT1 = "Assign a; Variable v; Select a such that Uses(a, v)";
	string const USER_INPUT2 = "Assign a, a1; Select a such that Follows(a, a1)";
	string const USER_INPUT3 = "Assign a; Variable v; Select a such that Pattern a(v, _)";
	string const USER_INPUT4 = "Assign a; Variable v; Select a such that Pattern a(v, _\" x + y \"_)";
	QueryParser qp = QueryParser();
	Query q1 = qp.queryProcessor(USER_INPUT1);
	Query q2 = qp.queryProcessor(USER_INPUT2);
	Query q3 = qp.queryProcessor(USER_INPUT3);
	Query q4 = qp.queryProcessor(USER_INPUT4);
}
