#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ResultsTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultsTest );
	CPPUNIT_TEST( testCluasePass );
	CPPUNIT_TEST( testHasResults );
	CPPUNIT_TEST( testInsertMultiResult );
	CPPUNIT_TEST( testInsertResult );
	CPPUNIT_TEST( testInsertResultWhere );
	CPPUNIT_TEST( testSelectMultiSyn );
	CPPUNIT_TEST( testSelectSyn );
	CPPUNIT_TEST( testSelectSynWhere );
	CPPUNIT_TEST( testPushChanges );
	CPPUNIT_TEST( testDelMultiResult );
	CPPUNIT_TEST( testDelResult );
	CPPUNIT_TEST_SUITE_END();

public:
	void testCluasePass();
	void testHasResults();
	void testInsertMultiResult();
	void testInsertResult();
	void testInsertResultWhere();
	void testSelectMultiSyn();
	void testSelectSyn();
	void testSelectSynWhere();
	void testPushChanges();
	void testDelMultiResult();
	void testDelResult();
};

