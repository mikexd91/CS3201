 #include <cppunit/config/SourcePrefix.h>
#include "QueryOptimiserTest.h"
#include "../SPA/QueryOptimiser.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/FollowsStarClause.h"
#include "../SPA/ParentClause.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/UsesClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/CallsClause.h"
#include "../SPA/CallsStarClause.h"
#include "../SPA/PatternIfClause.h"
#include "../SPA/PatternWhileClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/NextClause.h"
#include "../SPA/NextStarClause.h"
#include "../SPA/AffectsClause.h"
#include "../SPA/AffectsStarClause.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/PatternClauseBuilder.h"
#include "../SPA/WithClause.h"
#include "../SPA/WithClauseBuilder.h"
#include "../SPA/WithClauseRef.h"
#include "../SPA/Clause.h"
#include "../SPA/Utils.h"
#include "../SPA/SynGraph.h"
#include <boost\foreach.hpp>

using namespace stringconst;
using namespace std;

void QueryOptimiserTest::setUp() {

}

void QueryOptimiserTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryOptimiserTest );

void QueryOptimiserTest::testGettingStartingSynonym() {
	QueryOptimiser *qo = new QueryOptimiser();
	
	StringPair *s1 = new StringPair();
	s1->setFirst("v1");
	s1->setSecond(ARG_VARIABLE);
	StringPair *s2 = new StringPair();
	s2->setFirst("a1");
	s2->setSecond(ARG_ASSIGN);

	Query *q = new Query();
	q->addSelectSynonym(*s1);
	q->addSelectSynonym(*s2);

	SuchThatClauseBuilder* usesBuilder = new SuchThatClauseBuilder(USES_);
	usesBuilder->setArg(1, "a1");
	usesBuilder->setArgFixed(1, false);
	usesBuilder->setArgType(1, ARG_ASSIGN);
	usesBuilder->setArg(2, "v1");
	usesBuilder->setArgFixed(2, false);
	usesBuilder->setArgType(2, ARG_VARIABLE);
	UsesClause* c1 = (UsesClause*) usesBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "a1");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_ASSIGN);
	modifiesBuilder->setArg(2, "v1");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* c2 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* affectsStarBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsStarBuilder->setArg(1, "_");
	affectsStarBuilder->setArgFixed(1, false);
	affectsStarBuilder->setArgType(1, ARG_GENERIC);
	affectsStarBuilder->setArg(2, "a2");
	affectsStarBuilder->setArgFixed(2, false);
	affectsStarBuilder->setArgType(2, ARG_ASSIGN);
	AffectsClause* c3 = (AffectsClause*) affectsStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	PatternClauseBuilder* assgBuilder1 = new PatternClauseBuilder(PATTERNASSG_);
	assgBuilder1->setSynonym("a2");
	assgBuilder1->setVar("i");
	assgBuilder1->setVarType(ARG_VARIABLE);
	assgBuilder1->setVarFixed(true);
	assgBuilder1->setExpr(1, "_");
	PatternAssgClause* c4 = (PatternAssgClause*) assgBuilder1->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	PatternClauseBuilder* assgBuilder2 = new PatternClauseBuilder(PATTERNASSG_);
	assgBuilder2->setSynonym("a3");
	assgBuilder2->setVar("v1");
	assgBuilder2->setVarType(ARG_VARIABLE);
	assgBuilder2->setVarFixed(false);
	assgBuilder2->setExpr(1, "_");
	PatternAssgClause* c5 = (PatternAssgClause*) assgBuilder2->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	SuchThatClauseBuilder* nextStarBuilder1 = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder1->setArg(1, "a1");
	nextStarBuilder1->setArg(2, "a2");
	nextStarBuilder1->setArgType(1, ARG_ASSIGN);
	nextStarBuilder1->setArgType(2, ARG_ASSIGN);
	nextStarBuilder1->setArgFixed(1, false);
	nextStarBuilder1->setArgFixed(2, false);
	NextStarClause* c6 = (NextStarClause*) nextStarBuilder1->build();
	CPPUNIT_ASSERT(c6->isValid());

	q->addClause((Clause*) c6);

	SuchThatClauseBuilder* nextStarBuilder2 = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder2->setArg(1, "a2");
	nextStarBuilder2->setArg(2, "a3");
	nextStarBuilder2->setArgType(1, ARG_ASSIGN);
	nextStarBuilder2->setArgType(2, ARG_ASSIGN);
	nextStarBuilder2->setArgFixed(1, false);
	nextStarBuilder2->setArgFixed(2, false);
	NextStarClause* c7 = (NextStarClause*) nextStarBuilder2->build();
	CPPUNIT_ASSERT(c7->isValid());

	q->addClause((Clause*) c7);

	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);
	withBuilder->setRefType(1, ATTRREF_);
	withBuilder->setEntity(1, "a3");
	withBuilder->setAttrType(1, STMTNUM_);
	withBuilder->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder->setRefType(2, INTEGER_);
	withBuilder->setEntity(2, "1");
	withBuilder->setAttrType(2, NULLATTR_);
	withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* c8 = withBuilder->build();
	CPPUNIT_ASSERT(c8->isValid());

	q->addClause((Clause*) c8);

	SuchThatClauseBuilder* nextStarBuilder3 = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder3->setArg(1, "s1");
	nextStarBuilder3->setArg(2, "s2");
	nextStarBuilder3->setArgType(1, ARG_STATEMENT);
	nextStarBuilder3->setArgType(2, ARG_STATEMENT);
	nextStarBuilder3->setArgFixed(1, false);
	nextStarBuilder3->setArgFixed(2, false);
	NextStarClause* c9 = (NextStarClause*) nextStarBuilder3->build();
	CPPUNIT_ASSERT(c9->isValid());

	q->addClause((Clause*) c9);

	vector<int>* compSize = qo->optimizeQuery(q);
	
	CPPUNIT_ASSERT(compSize->size() == 2);
	CPPUNIT_ASSERT(compSize->at(0) == 1);
	CPPUNIT_ASSERT(compSize->at(1) == 8);

	/*
	BOOST_FOREACH(auto i, q->getClauseList()) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}
	*/
}

void QueryOptimiserTest::testWithCombination() {
	QueryOptimiser *qo = new QueryOptimiser();
	
	StringPair *s1 = new StringPair();
	s1->setFirst("s1");
	s1->setSecond(ARG_STATEMENT);
	StringPair *s2 = new StringPair();
	s2->setFirst("p");
	s2->setSecond(ARG_PROGLINE);
	StringPair *s3 = new StringPair();
	s3->setFirst("a1");
	s3->setSecond(ARG_ASSIGN);

	Query *q = new Query();
	q->addSelectSynonym(*s1);
	q->addSelectSynonym(*s2);
	q->addSelectSynonym(*s3);

	// fixed with clause, integer
	WithClauseBuilder* withBuilder1 = new WithClauseBuilder(WITH_);
	withBuilder1->setRefType(1, INTEGER_);
	withBuilder1->setEntity(1, "1");
	withBuilder1->setAttrType(1, NULLATTR_);
	withBuilder1->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
	withBuilder1->setRefType(2, INTEGER_);
	withBuilder1->setEntity(2, "1");
	withBuilder1->setAttrType(2, NULLATTR_);
	withBuilder1->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* w1 = withBuilder1->build();
	CPPUNIT_ASSERT(w1->isValid());

	q->addClause((Clause*) w1);

	// fixed with clause, string
	WithClauseBuilder* withBuilder2 = new WithClauseBuilder(WITH_);
	withBuilder2->setRefType(1, IDENT_);
	withBuilder2->setEntity(1, "asd");
	withBuilder2->setAttrType(1, NULLATTR_);
	withBuilder2->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);
	withBuilder2->setRefType(2, IDENT_);
	withBuilder2->setEntity(2, "asd");
	withBuilder2->setAttrType(2, NULLATTR_);
	withBuilder2->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);
	WithClause* w2 = withBuilder2->build();
	CPPUNIT_ASSERT(w2->isValid());

	q->addClause((Clause*) w2);

	// single syn fixed with clause
	//asd=dsa
	WithClauseBuilder* withBuilder3 = new WithClauseBuilder(WITH_);
	withBuilder3->setRefType(1, IDENT_);
	withBuilder3->setEntity(1, "asd");
	withBuilder3->setAttrType(1, NULLATTR_);
	withBuilder3->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);
	withBuilder3->setRefType(2, IDENT_);
	withBuilder3->setEntity(2, "dsa");
	withBuilder3->setAttrType(2, NULLATTR_);
	withBuilder3->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);
	WithClause* w3 = withBuilder3->build();
	CPPUNIT_ASSERT(w3->isValid());

	q->addClause((Clause*) w3);

	// s.stmt# = 1
	WithClauseBuilder* withBuilder4 = new WithClauseBuilder(WITH_);
	withBuilder4->setRefType(1, ATTRREF_);
	withBuilder4->setEntity(1, "s1");
	withBuilder4->setAttrType(1, STMTNUM_);
	withBuilder4->setEntityType(1, stringconst::ARG_STATEMENT);
	withBuilder4->setRefType(2, INTEGER_);
	withBuilder4->setEntity(2, "1");
	withBuilder4->setAttrType(2, NULLATTR_);
	withBuilder4->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* w4 = withBuilder4->build();
	CPPUNIT_ASSERT(w4->isValid());

	q->addClause((Clause*) w4);

	//w.stmt#=7
	WithClauseBuilder* withBuilder5 = new WithClauseBuilder(WITH_);
	withBuilder5->setRefType(1, ATTRREF_);
	withBuilder5->setEntity(1, "w");
	withBuilder5->setAttrType(1, STMTNUM_);
	withBuilder5->setEntityType(1, stringconst::ARG_WHILE);
	withBuilder5->setRefType(2, INTEGER_);
	withBuilder5->setEntity(2, "7");
	withBuilder5->setAttrType(2, NULLATTR_);
	withBuilder5->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* w5 = withBuilder5->build();
	CPPUNIT_ASSERT(w5->isValid());

	q->addClause((Clause*) w5);

	//call.stmt#=12
	WithClauseBuilder* withBuilder6 = new WithClauseBuilder(WITH_);
	withBuilder6->setRefType(1, ATTRREF_);
	withBuilder6->setEntity(1, "cz");
	withBuilder6->setAttrType(1, STMTNUM_);
	withBuilder6->setEntityType(1, stringconst::ARG_CALL);
	withBuilder6->setRefType(2, INTEGER_);
	withBuilder6->setEntity(2, "12");
	withBuilder6->setAttrType(2, NULLATTR_);
	withBuilder6->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
	WithClause* w6 = withBuilder6->build();
	CPPUNIT_ASSERT(w6->isValid());

	q->addClause((Clause*) w6);

	// multi syn with clause
	// c.value = s.stmt#
	WithClauseBuilder* withBuilder7 = new WithClauseBuilder(WITH_);
	withBuilder7->setRefType(1, ATTRREF_);
	withBuilder7->setEntity(1, "c");
	withBuilder7->setAttrType(1, CONSTVALUE_);
	withBuilder7->setEntityType(1, stringconst::ARG_CONSTANT);
	withBuilder7->setRefType(2, ATTRREF_);
	withBuilder7->setEntity(2, "s1");
	withBuilder7->setAttrType(2, STMTNUM_);
	withBuilder7->setEntityType(2, stringconst::ARG_STATEMENT);
	WithClause* w7 = withBuilder7->build();
	CPPUNIT_ASSERT(w7->isValid());

	q->addClause((Clause*) w7);

	vector<int>* compSize = qo->optimizeQuery(q);
	
	CPPUNIT_ASSERT(compSize->size() == 6);

	/*
	BOOST_FOREACH(auto i, compSize) {
		cout << "component size: ";
		cout << i;
		cout << " | ";
		cout << endl;
	}
	*/
}

void QueryOptimiserTest::testPatternAssignWhileIfCombination() {
	QueryOptimiser *qo = new QueryOptimiser();

	StringPair *w1 = new StringPair();
	w1->setFirst("w1");
	w1->setSecond(ARG_WHILE);

	Query *q = new Query();
	q->addSelectSynonym(*w1);

	// assignPattern, single syn
	PatternClauseBuilder* assgBuilder1 = new PatternClauseBuilder(PATTERNASSG_);
	assgBuilder1->setSynonym("a1");
	assgBuilder1->setVar("i");
	assgBuilder1->setVarType(ARG_VARIABLE);
	assgBuilder1->setVarFixed(true);
	assgBuilder1->setExpr(1, "_");
	PatternAssgClause* c1 = (PatternAssgClause*) assgBuilder1->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	// assignPattern, muti syn
	PatternClauseBuilder* assgBuilder2 = new PatternClauseBuilder(PATTERNASSG_);
	assgBuilder2->setSynonym("a1");
	assgBuilder2->setVar("v");
	assgBuilder2->setVarType(ARG_VARIABLE);
	assgBuilder2->setVarFixed(false);
	assgBuilder2->setExpr(1, "_");
	PatternAssgClause* c2 = (PatternAssgClause*) assgBuilder2->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	// ifPattern, single syn
	PatternClauseBuilder* ifBuilder1 = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder1->setSynonym("if");
	ifBuilder1->setVar("_");
	ifBuilder1->setVarType(ARG_GENERIC);
	ifBuilder1->setVarFixed(false);
	ifBuilder1->setExpr(1, "_");
	ifBuilder1->setExpr(2, "_");
	PatternIfClause* c3 = (PatternIfClause*) ifBuilder1->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	// ifPattern, multi syn
	PatternClauseBuilder* ifBuilder2 = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder2->setSynonym("if");
	ifBuilder2->setVar("v");
	ifBuilder2->setVarType(ARG_VARIABLE);
	ifBuilder2->setVarFixed(false);
	ifBuilder2->setExpr(1, "_");
	ifBuilder2->setExpr(2, "_");
	PatternIfClause* c4 = (PatternIfClause*) ifBuilder2->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	// whilePattern, single syn
	PatternClauseBuilder* whileBuilder1 = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder1->setSynonym("w1");
	whileBuilder1->setVar("_");
	whileBuilder1->setVarType(stringconst::ARG_GENERIC);
	whileBuilder1->setVarFixed(false);
	whileBuilder1->setExpr(1, "_");
	PatternWhileClause* c5 = (PatternWhileClause*) whileBuilder1->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	// whilePattern, multi syn
	PatternClauseBuilder* whileBuilder2 = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder2->setSynonym("w1");
	whileBuilder2->setVar("v");
	whileBuilder2->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder2->setVarFixed(false);
	whileBuilder2->setExpr(1, "_");
	PatternWhileClause* c6 = (PatternWhileClause*) whileBuilder2->build();
	CPPUNIT_ASSERT(c6->isValid());

	q->addClause((Clause*) c6);

	SuchThatClauseBuilder* parentBuilder1 = new SuchThatClauseBuilder(PARENT_);
	parentBuilder1->setArg(1, "if");
	parentBuilder1->setArgFixed(1, false);
	parentBuilder1->setArgType(1, ARG_IF);
	parentBuilder1->setArg(2, "a1");
	parentBuilder1->setArgFixed(2, false);
	parentBuilder1->setArgType(2, ARG_ASSIGN);
	ParentClause* c7 = (ParentClause*) parentBuilder1->build();
	CPPUNIT_ASSERT(c7->isValid());

	q->addClause((Clause*) c7);

	SuchThatClauseBuilder* parentBuilder2 = new SuchThatClauseBuilder(PARENT_);
	parentBuilder2->setArg(1, "w1");
	parentBuilder2->setArgFixed(1, false);
	parentBuilder2->setArgType(1, ARG_WHILE);
	parentBuilder2->setArg(2, "if");
	parentBuilder2->setArgFixed(2, false);
	parentBuilder2->setArgType(2, ARG_IF);
	ParentClause* c8 = (ParentClause*) parentBuilder2->build();
	CPPUNIT_ASSERT(c8->isValid());

	q->addClause((Clause*) c8);

	vector<int>* compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize->size() == 1);
	/*
	BOOST_FOREACH(auto i, q->getClauseList()) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}
	*/
}

void QueryOptimiserTest::testManySingleSyn() {
	QueryOptimiser *qo = new QueryOptimiser();

	StringPair *s1 = new StringPair();
	s1->setFirst("a1");
	s1->setSecond(ARG_ASSIGN);

	Query *q = new Query();
	q->addSelectSynonym(*s1);

	SuchThatClauseBuilder* usesBuilder1 = new SuchThatClauseBuilder(USES_);
	usesBuilder1->setArg(1, "a1");
	usesBuilder1->setArgFixed(1, false);
	usesBuilder1->setArgType(1, ARG_ASSIGN);
	usesBuilder1->setArg(2, "z");
	usesBuilder1->setArgFixed(2, true);
	usesBuilder1->setArgType(2, ARG_VARIABLE);
	UsesClause* c1 = (UsesClause*) usesBuilder1->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "a1");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_ASSIGN);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsClause* c2 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* nextStarBuilder = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder->setArg(1, "a1");
	nextStarBuilder->setArg(2, "_");
	nextStarBuilder->setArgType(1, ARG_ASSIGN);
	nextStarBuilder->setArgType(2, ARG_GENERIC);
	nextStarBuilder->setArgFixed(1, false);
	nextStarBuilder->setArgFixed(2, false);
	NextStarClause* c3 = (NextStarClause*) nextStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* modifiesBuilder2 = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder2->setArg(1, "a1");
	modifiesBuilder2->setArgFixed(1, false);
	modifiesBuilder2->setArgType(1, ARG_ASSIGN);
	modifiesBuilder2->setArg(2, "_");
	modifiesBuilder2->setArgFixed(2, false);
	modifiesBuilder2->setArgType(2, ARG_GENERIC);
	ModifiesClause* c4 = (ModifiesClause*) modifiesBuilder2->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "a1");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_ASSIGN);
	FollowsClause* c5 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	vector<int>* compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize->size() == 1);
}

void QueryOptimiserTest::testManyComponent() {
	QueryOptimiser *qo = new QueryOptimiser();

	StringPair *s1 = new StringPair();
	s1->setFirst("z");
	s1->setSecond(ARG_VARIABLE);
	StringPair *s2 = new StringPair();
	s2->setFirst("z2");
	s2->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*s1);
	q->addSelectSynonym(*s2);

	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "_");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_GENERIC);
	callsBuilder->setArg(2, "childProc1");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "a1");
	followsStarBuilder->setArgFixed(1, false);
	followsStarBuilder->setArgType(1, ARG_ASSIGN);
	followsStarBuilder->setArg(2, "w1");
	followsStarBuilder->setArgFixed(2, false);
	followsStarBuilder->setArgType(2, ARG_WHILE);	
	FollowsStarClause* c2 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "a2");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_ASSIGN);
	FollowsClause* c3 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* nextBuilder = new SuchThatClauseBuilder(NEXT_);
	nextBuilder->setArg(1, "a2");
	nextBuilder->setArg(2, "_");
	nextBuilder->setArgType(1, ARG_ASSIGN);
	nextBuilder->setArgType(2, ARG_GENERIC);
	nextBuilder->setArgFixed(1, false);
	nextBuilder->setArgFixed(2, false);
	NextClause* c4 = (NextClause*) nextBuilder->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* nextStarBuilder = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder->setArg(1, "a1");
	nextStarBuilder->setArg(2, "s");
	nextStarBuilder->setArgType(1, ARG_ASSIGN);
	nextStarBuilder->setArgType(2, ARG_STATEMENT);
	nextStarBuilder->setArgFixed(1, false);
	nextStarBuilder->setArgFixed(2, false);
	NextStarClause* c5 = (NextStarClause*) nextStarBuilder->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "_");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_GENERIC);
	affectsBuilder->setArg(2, "a2");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_ASSIGN);
	AffectsClause* c6 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(c6->isValid());

	q->addClause((Clause*) c6);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "s");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_STATEMENT);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* c7 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(c7->isValid());

	q->addClause((Clause*) c7);

	SuchThatClauseBuilder* usesBuilder = new SuchThatClauseBuilder(USES_);
	usesBuilder->setArg(1, "w1");
	usesBuilder->setArgFixed(1, false);
	usesBuilder->setArgType(1, ARG_WHILE);
	usesBuilder->setArg(2, "v");
	usesBuilder->setArgFixed(2, false);
	usesBuilder->setArgType(2, ARG_VARIABLE);
	UsesClause* c8 = (UsesClause*) usesBuilder->build();
	CPPUNIT_ASSERT(c8->isValid());

	q->addClause((Clause*) c8);

	vector<int>* compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize->size() == 3);
	CPPUNIT_ASSERT(compSize->at(0) == 1);
	CPPUNIT_ASSERT(compSize->at(1) == 3);
	CPPUNIT_ASSERT(compSize->at(2) == 4);
}

void QueryOptimiserTest::testTrickyCase() {
	QueryOptimiser *qo = new QueryOptimiser();
	
	StringPair *s1 = new StringPair();
	s1->setFirst("a1");
	s1->setSecond(ARG_ASSIGN);
	StringPair *s2 = new StringPair();
	s2->setFirst("w1");
	s2->setSecond(ARG_WHILE);

	Query *q = new Query();
	q->addSelectSynonym(*s1);
	q->addSelectSynonym(*s2);

	SuchThatClauseBuilder* nextBuilder = new SuchThatClauseBuilder(NEXT_);
	nextBuilder->setArg(1, "w1");
	nextBuilder->setArg(2, "s");
	nextBuilder->setArgType(1, ARG_WHILE);
	nextBuilder->setArgType(2, ARG_STATEMENT);
	nextBuilder->setArgFixed(1, false);
	nextBuilder->setArgFixed(2, false);
	NextClause* c1 = (NextClause*) nextBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* affectsBuilder1 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder1->setArg(1, "a1");
	affectsBuilder1->setArgFixed(1, false);
	affectsBuilder1->setArgType(1, ARG_ASSIGN);
	affectsBuilder1->setArg(2, "s1");
	affectsBuilder1->setArgFixed(2, false);
	affectsBuilder1->setArgType(2, ARG_STATEMENT);
	AffectsClause* c2 = (AffectsClause*) affectsBuilder1->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "a1");
	followsStarBuilder->setArgFixed(1, false);
	followsStarBuilder->setArgType(1, ARG_ASSIGN);
	followsStarBuilder->setArg(2, "_");
	followsStarBuilder->setArgFixed(2, false);
	followsStarBuilder->setArgType(2, ARG_GENERIC);	
	FollowsStarClause* c3 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "a1");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_ASSIGN);
	FollowsClause* c4 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "w1");
	parentStarBuilder->setArgFixed(1, false);
	parentStarBuilder->setArgType(1, ARG_WHILE);
	parentStarBuilder->setArg(2, "w3");
	parentStarBuilder->setArgFixed(2, false);
	parentStarBuilder->setArgType(2, ARG_WHILE);
	ParentStarClause* c5 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder2->setArg(1, "a1");
	affectsBuilder2->setArgFixed(1, false);
	affectsBuilder2->setArgType(1, ARG_ASSIGN);
	affectsBuilder2->setArg(2, "a2");
	affectsBuilder2->setArgFixed(2, false);
	affectsBuilder2->setArgType(2, ARG_ASSIGN);
	AffectsClause* c6 = (AffectsClause*) affectsBuilder2->build();
	CPPUNIT_ASSERT(c6->isValid());

	q->addClause((Clause*) c6);

	SuchThatClauseBuilder* nextStarBuilder = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder->setArg(1, "a2");
	nextStarBuilder->setArg(2, "w1");
	nextStarBuilder->setArgType(1, ARG_ASSIGN);
	nextStarBuilder->setArgType(2, ARG_WHILE);
	nextStarBuilder->setArgFixed(1, false);
	nextStarBuilder->setArgFixed(2, false);
	NextStarClause* c7 = (NextStarClause*) nextStarBuilder->build();
	CPPUNIT_ASSERT(c7->isValid());

	q->addClause((Clause*) c7);

	vector<int>* compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize->size() == 1);

	/*
	BOOST_FOREACH(auto i, q->getClauseList()) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}
	*/
}