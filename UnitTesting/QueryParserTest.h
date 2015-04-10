#pragma once
#include <cppunit/extensions/HelperMacros.h>

class QueryParserTest : public CPPUNIT_NS::TestFixture{

	CPPUNIT_TEST_SUITE ( QueryParserTest );
	CPPUNIT_TEST ( testParser );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

	void testParser(void);
};