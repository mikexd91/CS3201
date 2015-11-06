/*
 * NextStarBipClauseTest.h
 *
 *  Created on: 4 Nov, 2015
 *      Author: Leon
 */

#pragma once

#include <cppunit/extensions/HelperMacros.h>

class NextStarBipClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( NextStarBipClauseTest );

	//CPPUNIT_TEST( testFixFix );
	//CPPUNIT_TEST( testSynFix );
	CPPUNIT_TEST( testFixSyn );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixFix();
	void testSynFix();
	void testFixSyn();
};
