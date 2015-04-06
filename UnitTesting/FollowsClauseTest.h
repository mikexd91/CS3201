#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( FollowsClauseTest );
	CPPUNIT_TEST( testFollowsA2 );
	CPPUNIT_TEST( testFollows1A );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFollowsA2();

	void testFollows1A();

	//void testLinks();
};
    