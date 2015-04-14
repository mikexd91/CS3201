#pragma once

#include <cppunit/extensions/HelperMacros.h>

class UsesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( UsesClauseTest );
	CPPUNIT_TEST( testUsesAssignFixedFixedPass );
	CPPUNIT_TEST( testUsesAssignFixedFixedFail );
	CPPUNIT_TEST( testUsesAssignSynFixedPass );
	CPPUNIT_TEST( testUsesAssignSynFixedFail );
	CPPUNIT_TEST( testUsesAssignFixedSynPass );
	CPPUNIT_TEST( testUsesAssignFixedSynFail );
	CPPUNIT_TEST( testUsesAssignSynSynPass );
	CPPUNIT_TEST( testUsesAssignSynSynFail );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testUsesAssignFixedFixedPass();
	void testUsesAssignFixedFixedFail();
	void testUsesAssignSynFixedPass();
	void testUsesAssignSynFixedFail();
	void testUsesAssignFixedSynPass();
	void testUsesAssignFixedSynFail();
	void testUsesAssignSynSynPass();
	void testUsesAssignSynSynFail();
};