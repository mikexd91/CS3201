#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParentClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ParentClauseTest );
	CPPUNIT_TEST( testParentFixedFixedPass );
	CPPUNIT_TEST( testParentFixedFixedFail );
	CPPUNIT_TEST( testParentSynFixedPass );
	CPPUNIT_TEST( testParentSynFixedFail );
	CPPUNIT_TEST( testParentFixedSynPass );
	CPPUNIT_TEST( testParentFixedSynPassWithWhile );
	CPPUNIT_TEST( testParentFixedSynFail );
	CPPUNIT_TEST( testParentFixedSynFailWithWhile );
	CPPUNIT_TEST( testParentSynSynPass );
	CPPUNIT_TEST( testParentSynSynPassWithWhile );
	CPPUNIT_TEST( testParentFirstUnderscorePass );
	CPPUNIT_TEST( testParentSecondUnderscorePass );
	CPPUNIT_TEST( testParentBothUnderscorePass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testParentFixedFixedPass();
	void testParentFixedFixedFail();
	void testParentSynFixedPass();
	void testParentSynFixedFail();
	void testParentFixedSynPass();
	void testParentFixedSynPassWithWhile();
	void testParentFixedSynFail();
	void testParentFixedSynFailWithWhile();
	void testParentSynSynPass();
	void testParentSynSynPassWithWhile();
	void testParentFirstUnderscorePass();
	void testParentSecondUnderscorePass();
	void testParentBothUnderscorePass();
};
    