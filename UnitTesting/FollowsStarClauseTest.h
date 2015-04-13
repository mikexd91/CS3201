#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( FollowsStarClauseTest );
	CPPUNIT_TEST( testFollowsStarFixedFixed );
	CPPUNIT_TEST( testFollowsStarSynFixed );
	CPPUNIT_TEST( testFollowsStarFixedSyn );
	CPPUNIT_TEST( testFollowsStarSynSyn );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testFollowsStarFixedFixed();
	void testFollowsStarFixedSyn();
	void testFollowsStarSynFixed();
	void testFollowsStarSynSyn();

};
    