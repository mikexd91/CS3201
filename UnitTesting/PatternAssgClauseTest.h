#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class PatternAssgClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( PatternAssgClauseTest );
	CPPUNIT_TEST( testMatchVar );
	CPPUNIT_TEST( testIsFollowsStar );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testMatchVar();

	void testIsFollowsStar();

};
    