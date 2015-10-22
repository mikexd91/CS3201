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
	string const DEC = "variable v; constant c; while w; stmt s, v; assign a; procedure p; if i; call cl";
	Query* Q = new Query();
	vector<string>* split = new vector<string>();
	parser->tokeniser(DEC, ';', split);
	CPPUNIT_ASSERT_THROW(parser->parseDeclarations(Q, split), DuplicateDeclarationException);
	/*unordered_map<string,string> declist = Q->getDeclarationList();
	CPPUNIT_ASSERT(declist.at("v") == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(declist.at("c") == "constant");
	CPPUNIT_ASSERT(declist.at("cl") == "call");
	CPPUNIT_ASSERT(declist.at("s") == "stmt");
	CPPUNIT_ASSERT(declist.at("w") == "while");
	CPPUNIT_ASSERT(declist.at("a") == "assign");
	CPPUNIT_ASSERT(declist.at("p") == "procedure");
	CPPUNIT_ASSERT(declist.at("i") == "if");*/
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

void QueryParserTest::testSelectAttribute(){
	parser = QueryParser::getInstance();
	string const DEC = "variable v; constant c; while w; stmt s; assign a; procedure p; if i; call cl";
	string const SEL = "Select v.varName with";
	Query* Q = new Query();
	vector<string>* V_DEC = new vector<string>();
	parser->tokeniser(DEC, ';', V_DEC);
	parser->parseDeclarations(Q, V_DEC);
	queue<string>* Q_SEL = new queue<string>();
	parser->queueBuilder(SEL, Q_SEL);
	parser->parseSelectSynonyms(Q, Q_SEL);
	vector<StringPair> L_SEL = Q->getSelectList();
	StringPair SEL_V1 = L_SEL.at(0);
	CPPUNIT_ASSERT(SEL_V1.getFirst() == "v");
	CPPUNIT_ASSERT(SEL_V1.getSecond() == stringconst::ARG_VARIABLE);
	CPPUNIT_ASSERT(SEL_V1.getAttribute() == stringconst::ATTR_COND_VARNAME);
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
	string const DECLARATION = "stmt s;";
	string const USER_IN = "s.stmt# = 1";
	vector<string> DEC_LIST = vector<string>();
	parser->tokeniser(DECLARATION, ';', &DEC_LIST);
	queue<string>* WITH_Q = new queue<string>();
	parser->queueBuilder(USER_IN, WITH_Q);
	Query* ASSERTION = new Query();
	parser->parseDeclarations(ASSERTION, &DEC_LIST);
	parser->parseWith(ASSERTION, WITH_Q);
	vector<Clause*> cl = ASSERTION->getClauseList();
	Clause* w = cl.at(0);
	//ClauseType type = w->getClauseType();
	//if (type == FOLLOWS_){
	//	cout << 1;
	//} else if (type == FOLLOWSSTAR_){
	//	cout << 2;
	//} else if (type == PARENT_){
	//	cout << 3;
	//} else if (type == PARENTSTAR_){
	//	cout << 4;
	//} else if (type == USES_){
	//	cout << 5;
	//}  else if (type == MODIFIES_){
	//	cout << 6;
	//} else if (type == CALLS_){
	//	cout << 7;
	//} else if (type == CALLSSTAR_){
	//	cout << 8;
	//} else if (type == NEXT_){
	//	cout << 9;
	//} else if (type == NEXTSTAR_){
	//	cout << 10;
	//} else if (type == AFFECTS_){
	//	cout << 11;
	//} else if (type == AFFECTSSTAR_){
	//	cout << 12;
	//}  else if (type == PATTERNASSG_){
	//	cout << 13;
	//}  else if (type == PATTERNIF_){
	//	cout << 14;
	//}  else if (type == PATTERNWHILE_){
	//	cout << 15;
	//}  else if (type == WITH_){
	//	cout << 16;
	//} else {
	//	cout << "nope";
	//}
	//CPPUNIT_ASSERT(type == WITH_);
	//WithClauseRef wl = w->getLeftRef();
	//WithClauseRef wr = w->getRightRef();
	//CPPUNIT_ASSERT(wl.getEntity() == "c");
	//CPPUNIT_ASSERT(wl.getRefType() == ATTRREF_);
	//CPPUNIT_ASSERT(wl.getAttrType() == CONSTVALUE_);
	//
	//CPPUNIT_ASSERT(wr.getEntity() == "1");
	//CPPUNIT_ASSERT(wr.getRefType() == INTEGER_);
	//CPPUNIT_ASSERT(wr.getAttrType() == NULLATTR_);
	
}

void QueryParserTest::testParser(){
	string const INPUTLINE1 = "variable v; assign a; while w; Select a such that Uses(a,v) and Follows*(1,2) and Parent(4,5) and pattern w(_,_)";
	parser = QueryParser::getInstance();
	Query* QUERY = new Query();
	QUERY = parser->parseQuery(INPUTLINE1);
	vector<Clause*> VC = QUERY->getClauseList();
	CPPUNIT_ASSERT(VC.size() == 4);
	CPPUNIT_ASSERT(VC.at(0)->getClauseType() == USES_);
	CPPUNIT_ASSERT(VC.at(1)->getClauseType() == FOLLOWSSTAR_);
	CPPUNIT_ASSERT(VC.at(2)->getClauseType() == PARENT_);
	//CPPUNIT_ASSERT(VC.at(3)->getClauseType() == PATTERNWHILE_);
	//ClauseType type = VC.at(3)->getClauseType();
	//if (type == FOLLOWS_){
	//	cout << 1;
	//} else if (type == FOLLOWSSTAR_){
	//	cout << 2;
	//} else if (type == PARENT_){
	//	cout << 3;
	//} else if (type == PARENTSTAR_){
	//	cout << 4;
	//} else if (type == USES_){
	//	cout << 5;
	//}  else if (type == MODIFIES_){
	//	cout << 6;
	//} else if (type == CALLS_){
	//	cout << 7;
	//} else if (type == CALLSSTAR_){
	//	cout << 8;
	//} else if (type == NEXT_){
	//	cout << 9;
	//} else if (type == NEXTSTAR_){
	//	cout << 10;
	//} else if (type == AFFECTS_){
	//	cout << 11;
	//} else if (type == AFFECTSSTAR_){
	//	cout << 12;
	//}  else if (type == PATTERNASSG_){
	//	cout << 13;
	//}  else if (type == PATTERNIF_){
	//	cout << 14;
	//}  else if (type == PATTERNWHILE_){
	//	cout << 15;
	//}  else if (type == WITH_){
	//	cout << 16;
	//} else {
	//	cout << "nope";
	//}
}

void QueryParserTest::debugTests(){
	string INPUT = "stmt s; Select s with 5 = s.stmt#";
	Query* QUERY = new Query();
	parser = QueryParser::getInstance();
	QUERY = parser->parseQuery(INPUT);
	vector<Clause*> VC = QUERY->getClauseList();
	WithClause* WC = dynamic_cast<WithClause*>(VC.at(0));
	WithClauseRef WCRint = WC->getLeftRef();
	WithClauseRef WCRstmt = WC->getRightRef();

	CPPUNIT_ASSERT(WCRstmt.getEntity() == "s");
	CPPUNIT_ASSERT(WCRstmt.getEntityType() == stringconst::ARG_STATEMENT);
	CPPUNIT_ASSERT(WCRstmt.getAttrType() == STMTNUM_);
	CPPUNIT_ASSERT(WCRstmt.getRefType() == ATTRREF_);
	
	CPPUNIT_ASSERT(WCRint.getEntity() == "5");
	CPPUNIT_ASSERT(WCRint.getEntityType() == stringconst::ENTITY_TYPE_INTEGER);
	CPPUNIT_ASSERT(WCRint.getAttrType() == NULLATTR_);
	CPPUNIT_ASSERT(WCRint.getRefType() == INTEGER_);
}