#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class PatternWhileClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( PatternWhileClauseTest );
	CPPUNIT_TEST( evaluateVarWild );
	CPPUNIT_TEST( evaluateVarFixed );
	CPPUNIT_TEST( evaluateVarSyn );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void evaluateVarWild();
	void evaluateVarFixed();
	void evaluateVarSyn();

};
