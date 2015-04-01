#pragma once

#include "../SPA/AST.h"

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ASTTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ASTTest );
	CPPUNIT_TEST( testInstances );
	CPPUNIT_TEST( testAcrossMethods );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void testInstances();

	void testAcrossMethods();

};
    