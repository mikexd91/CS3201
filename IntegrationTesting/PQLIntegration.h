#pragma once

#include <cppunit/extensions/HelperMacros.h>

class PQLIntegration : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( PQLIntegration );
	CPPUNIT_TEST( testSelectOnly );
	CPPUNIT_TEST( testSelectModifies );
	CPPUNIT_TEST( testSelectUses );
	CPPUNIT_TEST( testSelectFollows );
	CPPUNIT_TEST( testSelectFollowsStar );
	CPPUNIT_TEST( testSelectParent );
	CPPUNIT_TEST( testSelectParentStar );
	CPPUNIT_TEST( testSelectPattern );
	CPPUNIT_TEST( testSelectModifiesPattern );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSelectOnly();
	void testSelectModifies();
	void testSelectUses();
	void testSelectFollows();
	void testSelectFollowsStar();
	void testSelectParent();
	void testSelectParentStar();
	void testSelectPattern();
	void testSelectModifiesPattern();

};