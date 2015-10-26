#include <cppunit/config/SourcePrefix.h>
#include "SynGraphTest.h"
#include "../SPA/SynGraph.h"
#include "../SPA/Clause.h"
#include "../SPA/Utils.h"

using namespace stringconst;

void SynGraphTest::setUp() {
}

void SynGraphTest::tearDown() {
}

CPPUNIT_TEST_SUITE_REGISTRATION( SynGraphTest );

void SynGraphTest::testPopulateGraphTableSelectBoolNoClause() {
	SynGraph *graph = new SynGraph();

	StringPair *p = new StringPair();
	p->setFirst("BOOLEAN");
	p->setSecond(ARG_BOOLEAN);

}
