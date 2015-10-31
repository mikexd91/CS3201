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

void QueryOptimiserTest::testMutiSingleSyn1Component() {
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

	vector<int> compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize.size() == 1);
}

void QueryOptimiserTest::testMultiComponent() {
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

	vector<int> compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize.size() == 3);
	CPPUNIT_ASSERT(compSize.at(0) == 1);
	CPPUNIT_ASSERT(compSize.at(1) == 3);
	CPPUNIT_ASSERT(compSize.at(2) == 4);
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

	vector<int> compSize = qo->optimizeQuery(q);
	CPPUNIT_ASSERT(compSize.size() == 1);

	/*
	BOOST_FOREACH(auto i, q->getClauseList()) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}
	*/
}