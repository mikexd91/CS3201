#pragma once
#include <cppunit/extensions/HelperMacros.h>

class QueryParserTest : public CPPUNIT_NS::TestFixture{
private:
	void testRemoveSpace(void);
public:
	void setUp();
	void tearDown();
	void testProperties();
	void testLinks();
};

