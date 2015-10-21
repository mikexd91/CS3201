#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsStarClauseTest );
	CPPUNIT_TEST( testFixedFixedEasyPass );
	CPPUNIT_TEST( testFixedFixedPass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixedFixedEasyPass();
	void testFixedFixedPass();

};
    