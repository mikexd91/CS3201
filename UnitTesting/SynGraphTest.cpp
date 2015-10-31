#include <cppunit/config/SourcePrefix.h>
#include "SynGraphTest.h"
#include "../SPA/SynGraph.h"
#include "../SPA/SynEdge.h"
#include "../SPA/SynNode.h"
#include "../SPA/Clause.h"
#include "../SPA/Utils.h"

#include "../SPA/ParentClause.h"
#include "../SPA/ParentStarClause.h"
#include "../SPA/PatternClause.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/PatternIfClause.h"
#include "../SPA/PatternWhileClause.h"
#include "../SPA/ModifiesClause.h"
#include "../SPA/UsesClause.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/FollowsStarClause.h"
#include "../SPA/AffectsClause.h"
#include "../SPA/NextClause.h"
#include "../SPA/NextStarClause.h"
#include "../SPA/CallsClause.h"
#include "../SPA/CallsStarClause.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/PatternClauseBuilder.h"
#include <boost\foreach.hpp>

using namespace stringconst;
using namespace std;

void SynGraphTest::setUp() {
}

void SynGraphTest::tearDown() {
}

CPPUNIT_TEST_SUITE_REGISTRATION( SynGraphTest );

void SynGraphTest::testMultiClauseContainsDesiredNeighbourClause() {
	SynGraph *graph = new SynGraph();

	StringPair *s1 = new StringPair();
	s1->setFirst("s");
	s1->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*s1);

	SuchThatClauseBuilder* usesBuilder = new SuchThatClauseBuilder(USES_);
	usesBuilder->setArg(1, "1");
	usesBuilder->setArgFixed(1, true);
	usesBuilder->setArgType(1, ARG_STATEMENT);
	usesBuilder->setArg(2, "v");
	usesBuilder->setArgFixed(2, false);
	usesBuilder->setArgType(2, ARG_VARIABLE);
	UsesClause* c1 = (UsesClause*) usesBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "_");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_GENERIC);
	followsBuilder->setArg(2, "a1");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_ASSIGN);
	FollowsClause* c2 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* modifiesBuilder = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder->setArg(1, "a2");
	modifiesBuilder->setArgFixed(1, false);
	modifiesBuilder->setArgType(1, ARG_ASSIGN);
	modifiesBuilder->setArg(2, "v");
	modifiesBuilder->setArgFixed(2, false);
	modifiesBuilder->setArgType(2, ARG_VARIABLE);
	ModifiesClause* c3 = (ModifiesClause*) modifiesBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* nextBuilder = new SuchThatClauseBuilder(NEXT_);
	nextBuilder->setArg(1, "s");
	nextBuilder->setArg(2, "_");
	nextBuilder->setArgType(1, ARG_STATEMENT);
	nextBuilder->setArgType(2, ARG_GENERIC);
	nextBuilder->setArgFixed(1, false);
	nextBuilder->setArgFixed(2, false);
	NextClause* c4 = (NextClause*) nextBuilder->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "a1");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_ASSIGN);
	affectsBuilder->setArg(2, "a2");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_ASSIGN);
	AffectsClause* c5 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testMultiClauseOnlyUndesiredNeighbourClause() {
	SynGraph *graph = new SynGraph();

	StringPair *s1 = new StringPair();
	s1->setFirst("s2");
	s1->setSecond(ARG_STATEMENT);
	Query *q = new Query();
	q->addSelectSynonym(*s1);

	SuchThatClauseBuilder* nextStarBuilder1 = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder1->setArg(1, "s1");
	nextStarBuilder1->setArg(2, "s2");
	nextStarBuilder1->setArgType(1, ARG_STATEMENT);
	nextStarBuilder1->setArgType(2, ARG_STATEMENT);
	nextStarBuilder1->setArgFixed(1, false);
	nextStarBuilder1->setArgFixed(2, false);
	NextStarClause* c1 = (NextStarClause*) nextStarBuilder1->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* nextStarBuilder2 = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder2->setArg(1, "s1");
	nextStarBuilder2->setArg(2, "s2");
	nextStarBuilder2->setArgType(1, ARG_STATEMENT);
	nextStarBuilder2->setArgType(2, ARG_STATEMENT);
	nextStarBuilder2->setArgFixed(1, false);
	nextStarBuilder2->setArgFixed(2, false);
	NextStarClause* c2 = (NextStarClause*) nextStarBuilder2->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "s1");
	followsStarBuilder->setArgFixed(1, false);
	followsStarBuilder->setArgType(1, ARG_ASSIGN);
	followsStarBuilder->setArg(2, "_");
	followsStarBuilder->setArgFixed(2, false);
	followsStarBuilder->setArgType(2, ARG_GENERIC);
	FollowsStarClause* c3 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testMultiClauseOnlyUndesiredSingleSynClause() {
	SynGraph *graph = new SynGraph();

	StringPair *s1 = new StringPair();
	s1->setFirst("BOOLEAN");
	s1->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*s1);

	SuchThatClauseBuilder* affectsBuilder1 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder1->setArg(1, "a1");
	affectsBuilder1->setArgFixed(1, false);
	affectsBuilder1->setArgType(1, ARG_ASSIGN);
	affectsBuilder1->setArg(2, "2");
	affectsBuilder1->setArgFixed(2, true);
	affectsBuilder1->setArgType(2, ARG_STATEMENT);
	AffectsClause* c1 = (AffectsClause*) affectsBuilder1->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder2->setArg(1, "a3");
	affectsBuilder2->setArgFixed(1, false);
	affectsBuilder2->setArgType(1, ARG_ASSIGN);
	affectsBuilder2->setArg(2, "_");
	affectsBuilder2->setArgFixed(2, false);
	affectsBuilder2->setArgType(2, ARG_GENERIC);
	AffectsClause* c2 = (AffectsClause*) affectsBuilder2->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* affectsBuilder3 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder3->setArg(1, "a1");
	affectsBuilder3->setArgFixed(1, false);
	affectsBuilder3->setArgType(1, ARG_ASSIGN);
	affectsBuilder3->setArg(2, "a2");
	affectsBuilder3->setArgFixed(2, false);
	affectsBuilder3->setArgType(2, ARG_ASSIGN);
	AffectsClause* c3 = (AffectsClause*) affectsBuilder3->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* nextStarBuilder = new SuchThatClauseBuilder(NEXTSTAR_);
	nextStarBuilder->setArg(1, "a4");
	nextStarBuilder->setArg(2, "a3");
	nextStarBuilder->setArgType(1, ARG_ASSIGN);
	nextStarBuilder->setArgType(2, ARG_ASSIGN);
	nextStarBuilder->setArgFixed(1, false);
	nextStarBuilder->setArgFixed(2, false);
	NextStarClause* c4 = (NextStarClause*) nextStarBuilder->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "a2");
	followsStarBuilder->setArgFixed(1, false);
	followsStarBuilder->setArgType(1, ARG_ASSIGN);
	followsStarBuilder->setArg(2, "a3");
	followsStarBuilder->setArgFixed(2, false);
	followsStarBuilder->setArgType(2, ARG_ASSIGN);
	FollowsStarClause* c5 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "f1");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "a2");
	parentBuilder->setArgFixed(2, false);
	parentBuilder->setArgType(2, ARG_ASSIGN);
	ParentClause* c6 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(c6->isValid());

	q->addClause((Clause*) c6);

	SuchThatClauseBuilder* followsBuilder = new SuchThatClauseBuilder(FOLLOWS_);
	followsBuilder->setArg(1, "a1");
	followsBuilder->setArgFixed(1, false);
	followsBuilder->setArgType(1, ARG_ASSIGN);
	followsBuilder->setArg(2, "a4");
	followsBuilder->setArgFixed(2, false);
	followsBuilder->setArgType(2, ARG_ASSIGN);
	FollowsClause* c7 = (FollowsClause*) followsBuilder->build();
	CPPUNIT_ASSERT(c7->isValid());

	q->addClause((Clause*) c7);

	SuchThatClauseBuilder* affectsBuilder4 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder4->setArg(1, "a5");
	affectsBuilder4->setArgFixed(1, false);
	affectsBuilder4->setArgType(1, ARG_ASSIGN);
	affectsBuilder4->setArg(2, "8");
	affectsBuilder4->setArgFixed(2, true);
	affectsBuilder4->setArgType(2, ARG_STATEMENT);
	AffectsClause* c8 = (AffectsClause*) affectsBuilder4->build();
	CPPUNIT_ASSERT(c8->isValid());

	q->addClause((Clause*) c8);

	SuchThatClauseBuilder* affectsBuilder5 = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder5->setArg(1, "_");
	affectsBuilder5->setArgFixed(1, false);
	affectsBuilder5->setArgType(1, ARG_GENERIC);
	affectsBuilder5->setArg(2, "a5");
	affectsBuilder5->setArgFixed(2, false);
	affectsBuilder5->setArgType(2, ARG_ASSIGN);
	AffectsClause* c9 = (AffectsClause*) affectsBuilder5->build();
	CPPUNIT_ASSERT(c9->isValid());

	q->addClause((Clause*) c9);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testSynSelectNotUsedByMultiArgUnfixedClause() {
	SynGraph *graph = new SynGraph();

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

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testSynSelectUsedByMultiArgUnfixedClauseSecondArg() {
	SynGraph *graph = new SynGraph();

	StringPair *s1 = new StringPair();
	s1->setFirst("z");
	s1->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*s1);

	SuchThatClauseBuilder* usesBuilder1 = new SuchThatClauseBuilder(USES_);
	usesBuilder1->setArg(1, "a1");
	usesBuilder1->setArgFixed(1, false);
	usesBuilder1->setArgType(1, ARG_ASSIGN);
	usesBuilder1->setArg(2, "z");
	usesBuilder1->setArgFixed(2, false);
	usesBuilder1->setArgType(2, ARG_VARIABLE);
	UsesClause* c1 = (UsesClause*) usesBuilder1->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testSynSelectUsedByMultiArgUnfixedClauseFirstArg() {
	SynGraph *graph = new SynGraph();

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

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testSynSelectUsedBySingleArgUnfixedClause() {
	SynGraph *graph = new SynGraph();

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

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableManySynSelect1UnFixedClauseCircle() {
	SynGraph *graph = new SynGraph();

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

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

// Failing due to patternwhile
void SynGraphTest::testPopulateGraphTableManySynSelectManyUnFixedClauseCircle() {
	SynGraph *graph = new SynGraph();

	StringPair *s1 = new StringPair();
	s1->setFirst("a1");
	s1->setSecond(ARG_ASSIGN);
	StringPair *s2 = new StringPair();
	s2->setFirst("w1");
	s2->setSecond(ARG_WHILE);

	Query *q = new Query();
	q->addSelectSynonym(*s1);
	q->addSelectSynonym(*s2);

	PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder->setSynonym("w1");
	whileBuilder->setVar("v");
	whileBuilder->setVarType(ARG_VARIABLE);
	whileBuilder->setVarFixed(true);
	whileBuilder->setExpr(1, "_");
	PatternWhileClause* c1 = (PatternWhileClause*) whileBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* modifiesBuilder1 = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder1->setArg(1, "a1");
	modifiesBuilder1->setArgFixed(1, false);
	modifiesBuilder1->setArgType(1, ARG_ASSIGN);
	modifiesBuilder1->setArg(2, "v2");
	modifiesBuilder1->setArgFixed(2, false);
	modifiesBuilder1->setArgType(2, ARG_VARIABLE);
	ModifiesClause* c2 = (ModifiesClause*) modifiesBuilder1->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* usesBuilder1 = new SuchThatClauseBuilder(USES_);
	usesBuilder1->setArg(1, "a1");
	usesBuilder1->setArgFixed(1, false);
	usesBuilder1->setArgType(1, ARG_ASSIGN);
	usesBuilder1->setArg(2, "v1");
	usesBuilder1->setArgFixed(2, false);
	usesBuilder1->setArgType(2, ARG_VARIABLE);
	UsesClause* c3 = (UsesClause*) usesBuilder1->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	SuchThatClauseBuilder* modifiesBuilder2 = new SuchThatClauseBuilder(MODIFIES_);
	modifiesBuilder2->setArg(1, "w1");
	modifiesBuilder2->setArgFixed(1, false);
	modifiesBuilder2->setArgType(1, ARG_WHILE);
	modifiesBuilder2->setArg(2, "v1");
	modifiesBuilder2->setArgFixed(2, false);
	modifiesBuilder2->setArgType(2, ARG_VARIABLE);
	ModifiesClause* c4 = (ModifiesClause*) modifiesBuilder2->build();
	CPPUNIT_ASSERT(c4->isValid());

	q->addClause((Clause*) c4);

	SuchThatClauseBuilder* usesBuilder2 = new SuchThatClauseBuilder(USES_);
	usesBuilder2->setArg(1, "w1");
	usesBuilder2->setArgFixed(1, false);
	usesBuilder2->setArgType(1, ARG_WHILE);
	usesBuilder2->setArg(2, "v2");
	usesBuilder2->setArgFixed(2, false);
	usesBuilder2->setArgType(2, ARG_VARIABLE);
	UsesClause* c5 = (UsesClause*) usesBuilder2->build();
	CPPUNIT_ASSERT(c5->isValid());

	q->addClause((Clause*) c5);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableManySynSelectManyUnFixedClauseTree() {
	SynGraph *graph = new SynGraph();

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

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableManySynSelect1FixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p1 = new StringPair();
	p1->setFirst("s");
	p1->setSecond(ARG_STATEMENT);
	StringPair *p2 = new StringPair();
	p2->setFirst("v");
	p2->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*p1);
	q->addSelectSynonym(*p2);

	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "_");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_GENERIC);
	callsStarBuilder->setArg(2, "main");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableManySynSelectManyFixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *n1 = new StringPair();
	n1->setFirst("n1");
	n1->setSecond(ARG_STATEMENT);
	StringPair *v1 = new StringPair();
	v1->setFirst("v1");
	v1->setSecond(ARG_VARIABLE);

	Query *q = new Query();
	q->addSelectSynonym(*n1);
	q->addSelectSynonym(*v1);

	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "_");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_GENERIC);
	callsStarBuilder->setArg(2, "proc");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "_");
	followsStarBuilder->setArgFixed(1, false);
	followsStarBuilder->setArgType(1, ARG_GENERIC);
	followsStarBuilder->setArg(2, "_");
	followsStarBuilder->setArgFixed(2, false);
	followsStarBuilder->setArgType(2, ARG_GENERIC);	
	FollowsStarClause* c2 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "9");
	parentStarBuilder->setArgFixed(1, true);
	parentStarBuilder->setArgType(1, ARG_IF);
	parentStarBuilder->setArg(2, "11");
	parentStarBuilder->setArgFixed(2, true);
	parentStarBuilder->setArgType(2, ARG_WHILE);
	ParentStarClause* c3 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTable1SynSelect1FixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);

	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "9");
	followsStarBuilder->setArgFixed(1, true);
	followsStarBuilder->setArgType(1, ARG_STATEMENT);
	followsStarBuilder->setArg(2, "10");
	followsStarBuilder->setArgFixed(2, true);
	followsStarBuilder->setArgType(2, ARG_STATEMENT);	
	FollowsStarClause* c1 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTable1SynSelectManyFixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p = new StringPair();
	p->setFirst("s");
	p->setSecond(ARG_STATEMENT);

	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* followsStarBuilder = new SuchThatClauseBuilder(FOLLOWSSTAR_);
	followsStarBuilder->setArg(1, "9");
	followsStarBuilder->setArgFixed(1, true);
	followsStarBuilder->setArgType(1, ARG_STATEMENT);
	followsStarBuilder->setArg(2, "10");
	followsStarBuilder->setArgFixed(2, true);
	followsStarBuilder->setArgType(2, ARG_STATEMENT);	
	FollowsStarClause* c1 = (FollowsStarClause*) followsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	q->addClause((Clause*) c1);

	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "main");
	callsBuilder->setArgFixed(1, true);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "childProc1");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c2 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c2->isValid());

	q->addClause((Clause*) c2);

	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "a");
	callsStarBuilder->setArgFixed(1, true);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "main");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c3 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c3->isValid());

	q->addClause((Clause*) c3);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableBoolSelect1FixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);

	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m1 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	q->addClause((Clause*) m1);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}

void SynGraphTest::testPopulateGraphTableBoolSelectManyFixedClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);
	Query *q = new Query();
	q->addSelectSynonym(*p);

	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "9");
	parentBuilder->setArgFixed(1, true);
	parentBuilder->setArgType(1, ARG_IF);
	parentBuilder->setArg(2, "11");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m1 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	q->addClause((Clause*) m1);
	
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

	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "_");
	parentStarBuilder->setArgFixed(1, false);
	parentStarBuilder->setArgType(1, ARG_GENERIC);
	parentStarBuilder->setArg(2, "_");
	parentStarBuilder->setArgFixed(2, false);
	parentStarBuilder->setArgType(2, ARG_GENERIC);
	ParentStarClause* m3 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m3->isValid());

	q->addClause((Clause*) m3);

	vector<int> queryCompSize = graph->reorderQuery(q);

	/*
	cout << endl;
	BOOST_FOREACH(auto i, graph->synTable) {
		cout << "Syn: ";
		cout << i.first << ", ";
		cout << "SynNode syn name: ";
		cout << (i.second)->getSynonym() << endl;
	}
	
	BOOST_FOREACH(auto j, graph->graphTable) {
		SynGraph::EdgeList edgelist = j.second;
		
		BOOST_FOREACH(auto k, edgelist) {
			cout << "Syn: ";
			cout << j.first << ", ";
			cout << "Edge clauseIndex: " << (k.first)->getClauseIndex();
			cout << ", ";
			cout << "Attached syn: " << (k.second)->getSynonym();
			cout << ", ";
			cout << "Component Num: " << (k.second)->getComponentNum();
			cout << endl;
		}
	}

	BOOST_FOREACH(auto i, graph->compWeightList) {
		cout << "component index: ";
		cout << i.first;
		cout << ", ";
		cout << "component weight: ";
		cout << i.second;
		cout << endl;
	}
	
	vector<Clause*> clauselist = q->getClauseList();
	BOOST_FOREACH(auto i, clauselist) {
		cout << "clause type: ";
		cout << i->getClauseType();
		cout << endl;
	}

	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "Query component size: ";
		cout << i;
		cout << endl;
	}
	*/
}
