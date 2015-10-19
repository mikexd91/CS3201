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
#include "../SPA/AffectsClause.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/PatternClauseBuilder.h"
#include "../SPA/Clause.h"
#include "../SPA/Utils.h"

using namespace stringconst;

void QueryOptimiserTest::setUp() {

}

void QueryOptimiserTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryOptimiserTest );

void QueryOptimiserTest::testSortQueryDifferentBuilder() {
	QueryOptimiser *qo = new QueryOptimiser();
	
	//Query:
	//Select BOOLEAN s.t. affects(1,4) and follows(7,_) and 
	//parent*(9,2) and calls*(p,q) and pattern if("x", "_", "_")
	//and pattern w("x", "_");

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	//affects(1,4)
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "1");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "4");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());
	q->addClause((Clause*) m1);

	// Follows(7, _)
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "7");
	followsBuilder->setArgFixed(1, true);
	followsBuilder->setArgType(1, ARG_WHILE);
	followsBuilder->setArg(2, "_");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_GENERIC);
	FollowsClause* m2 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m2->isValid());
	q->addClause((Clause*) m2);

	// Parent*(9,2)
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "9");
	parentStarBuilder->setArgFixed(1, true);
	parentStarBuilder->setArgType(1, ARG_WHILE);
	parentStarBuilder->setArg(2, "2");
	parentStarBuilder->setArgFixed(2, true);
	parentStarBuilder->setArgType(2, ARG_ASSIGN);
	ParentStarClause* m3 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());
	q->addClause((Clause*) m3);

	// Calls*(p,q)
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "p");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "q");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* m4 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(m4->isValid());
	q->addClause((Clause*) m4);

	// pattern if("x", "_", "_")
	PatternClauseBuilder* ifBuilder = new PatternClauseBuilder(PATTERNIF_);
	string syn1 = "if";
	ifBuilder->setSynonym(syn1);
	ifBuilder->setVar("x");
	ifBuilder->setVarType(ARG_VARIABLE);
	ifBuilder->setVarFixed(true);
	ifBuilder->setExpr(1, "_");
	ifBuilder->setExpr(2, "_");
	PatternIfClause* m5 = (PatternIfClause*) ifBuilder->build();
	q->addClause((Clause*) m5);
	
	// pattern w("x", "_");
	PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder->setSynonym("w");
	whileBuilder->setVar("x");
	whileBuilder->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder->setVarFixed(true);
	whileBuilder->setExpr(1, "_");
	PatternWhileClause* m6 = (PatternWhileClause*) whileBuilder->build();
	q->addClause((Clause*) m6);
	
	qo->sortQuery(q);

	Clause* c0 = q->getClauseList().at(0);
	ClauseType type0 = c0->getClauseType();
	int weight0 = c0->getWeight();
	CPPUNIT_ASSERT(type0 == FOLLOWS_);
	CPPUNIT_ASSERT(weight0 == 2);

	Clause* c1 = q->getClauseList().at(1);
	ClauseType type1 = c1->getClauseType();
	int weight1 = c1->getWeight();
	CPPUNIT_ASSERT(type1 == CALLSSTAR_);
	CPPUNIT_ASSERT(weight1 == 4);

	Clause* c2 = q->getClauseList().at(2);
	ClauseType type2 = c2->getClauseType();
	int weight2 = c2->getWeight();
	CPPUNIT_ASSERT(type2 == PARENTSTAR_);
	CPPUNIT_ASSERT(weight2 == 6);

	Clause* c3 = q->getClauseList().at(3);
	ClauseType type3 = c3->getClauseType();
	int weight3 = c3->getWeight();
	CPPUNIT_ASSERT(type3 == PATTERNIF_);
	CPPUNIT_ASSERT(weight3 == 8);

	Clause* c4 = q->getClauseList().at(4);
	ClauseType type4 = c4->getClauseType();
	int weight4 = c4->getWeight();
	CPPUNIT_ASSERT(type4 == PATTERNWHILE_);
	CPPUNIT_ASSERT(weight4 == 9);

	Clause* c5 = q->getClauseList().at(5);
	ClauseType type5 = c5->getClauseType();
	int weight5 = c5->getWeight();
	CPPUNIT_ASSERT(type5 == AFFECTS_);
	CPPUNIT_ASSERT(weight5 == 14);
}

void QueryOptimiserTest::testSortQuerySuchThatBuilder() {
	QueryOptimiser *qo = new QueryOptimiser();

	//Query
	//Select BOOLEAN s.t. Affects(4,4) and Affects(5,9) and Modifies("hello", v)
	//and Follows(a, 1) and Next(s, 5) and Modifies("hello", v)
	//and Uses(s, "i") and Uses(5, "w")

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	// Affects(4,4)
	SuchThatClauseBuilder* affectsBuilder1 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder1->setArg(1, "4");
	affectsBuilder1->setArgFixed(1, true);
	affectsBuilder1->setArgType(1, ARG_PROGLINE);
	affectsBuilder1->setArg(2, "4");
	affectsBuilder1->setArgFixed(2, true);
	affectsBuilder1->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder1->build();
	CPPUNIT_ASSERT(m1->isValid());
	q->addClause((Clause*) m1);

	// Affects(5,9)
	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder2->setArg(1, "5");
	affectsBuilder2->setArgFixed(1, true);
	affectsBuilder2->setArgType(1, ARG_PROGLINE);
	affectsBuilder2->setArg(2, "9");
	affectsBuilder2->setArgFixed(2, true);
	affectsBuilder2->setArgType(2, ARG_PROGLINE);
	AffectsClause* m2 = (AffectsClause*) affectsBuilder2->build();
	CPPUNIT_ASSERT(m2->isValid());
	q->addClause((Clause*) m2);

	// Modifies("hello", v);
	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "hello");
	modifiesBuilder->setArgFixed(1, true);
	modifiesBuilder->setArgType(1, ARG_PROCEDURE);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m3 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());
	q->addClause((Clause*) m3);

	// Follows(a, 1);
	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "a");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_STATEMENT);
	followsBuilder->setArg(2, "1");
	followsBuilder->setArgFixed(2, true);
	followsBuilder->setArgType(2, ARG_STATEMENT);
	FollowsClause* m4 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(m4->isValid());
	q->addClause((Clause*) m4);

	// Next(s, 5)
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "5");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* m5 = (NextClause*) builder->build();
	CPPUNIT_ASSERT(m5->isValid());
	q->addClause((Clause*) m5);

	// Modifies("hello", v);
	SuchThatClauseBuilder* modifiesBuilder2 = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder2->setArg(1, "hello");
	modifiesBuilder2->setArgFixed(1, true);
	modifiesBuilder2->setArgType(1, ARG_PROCEDURE);
	modifiesBuilder2->setArg(2, "v");
	modifiesBuilder2->setArgFixed(2, false);
	modifiesBuilder2->setArgType(2, ARG_VARIABLE);
	ModifiesClause* m6 = (ModifiesClause*) modifiesBuilder2->build();
	CPPUNIT_ASSERT(m6->isValid());
	q->addClause((Clause*) m6);

	// Uses(s, "i");
	SuchThatClauseBuilder* usesBuilder = new SuchThatClauseBuilder(USES_);
	usesBuilder->setArg(1, "s");
	usesBuilder->setArgFixed(1, false);
	usesBuilder->setArgType(1, ARG_STATEMENT);
	usesBuilder->setArg(2, "i");
	usesBuilder->setArgFixed(2, true);
	usesBuilder->setArgType(2, ARG_VARIABLE);
	UsesClause* m7 = (UsesClause*) usesBuilder->build();
	CPPUNIT_ASSERT(m7->isValid());
	q->addClause((Clause*) m7);

	// Uses(5, "w");
	SuchThatClauseBuilder* usesBuilder2 = new SuchThatClauseBuilder(USES_);
	usesBuilder2->setArg(1, "5");
	usesBuilder2->setArgFixed(1, true);
	usesBuilder2->setArgType(1, ARG_STATEMENT);
	usesBuilder2->setArg(2, "w");
	usesBuilder2->setArgFixed(2, true);
	usesBuilder2->setArgType(2, ARG_VARIABLE);
	UsesClause* m8 = (UsesClause*) usesBuilder2->build();
	CPPUNIT_ASSERT(m8->isValid());
	q->addClause((Clause*) m8);

	qo->sortQuery(q);

	Clause* c0 = q->getClauseList().at(0);
	ClauseType type0 = c0->getClauseType();
	int weight0 = c0->getWeight();
	CPPUNIT_ASSERT(type0 == FOLLOWS_);
	CPPUNIT_ASSERT(weight0 == 2);

	Clause* c1 = q->getClauseList().at(1);
	ClauseType type1 = c1->getClauseType();
	int weight1 = c1->getWeight();
	CPPUNIT_ASSERT(type1 == USES_);
	CPPUNIT_ASSERT(weight1 == 10);

	Clause* c2 = q->getClauseList().at(2);
	ClauseType type2 = c2->getClauseType();
	int weight2 = c2->getWeight();
	CPPUNIT_ASSERT(type2 == USES_);
	CPPUNIT_ASSERT(weight2 == 10);

	Clause* c3 = q->getClauseList().at(3);
	ClauseType type3 = c3->getClauseType();
	int weight3 = c3->getWeight();
	CPPUNIT_ASSERT(type3 == MODIFIES_);
	CPPUNIT_ASSERT(weight3 == 11);

	Clause* c4 = q->getClauseList().at(4);
	ClauseType type4 = c4->getClauseType();
	int weight4 = c4->getWeight();
	CPPUNIT_ASSERT(type4 == MODIFIES_);
	CPPUNIT_ASSERT(weight4 == 11);

	Clause* c5 = q->getClauseList().at(5);
	ClauseType type5 = c5->getClauseType();
	int weight5 = c5->getWeight();
	CPPUNIT_ASSERT(type5 == NEXT_);
	CPPUNIT_ASSERT(weight5 == 12);

	Clause* c6 = q->getClauseList().at(6);
	ClauseType type6 = c6->getClauseType();
	int weight6 = c6->getWeight();
	CPPUNIT_ASSERT(type6 == AFFECTS_);
	CPPUNIT_ASSERT(weight6 == 14);

	Clause* c7 = q->getClauseList().at(7);
	ClauseType type7 = c7->getClauseType();
	int weight7 = c7->getWeight();
	CPPUNIT_ASSERT(type7 == AFFECTS_);
	CPPUNIT_ASSERT(weight7 == 14);
}

void QueryOptimiserTest::testSoryQueryPatternBuilder() {

}