#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( FollowsClauseTest );
	CPPUNIT_TEST( testIsFollows );
	CPPUNIT_TEST( testIsFollows2);
	CPPUNIT_TEST( testIsFollows3);
	CPPUNIT_TEST( testIsFollows4);
	CPPUNIT_TEST( testIsFollowsStar );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testIsFollows();
	void testIsFollows2();
	void testIsFollows3();
	void testIsFollows4();
	void testIsFollowsStar();

};
    