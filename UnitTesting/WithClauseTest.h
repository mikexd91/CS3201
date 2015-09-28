#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class WithClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( WithClauseTest );
	CPPUNIT_TEST( evaluateRightIntLeftInt );
	CPPUNIT_TEST( evaluateRightStringLeftString );
	CPPUNIT_TEST( evaluateRightSynLeftSyn );
	CPPUNIT_TEST( evaluateRightSynLeftInt );
	CPPUNIT_TEST( evaluateRightAttrStmtNumLeftInt );
	CPPUNIT_TEST( evaluateRightAttrValueLeftInt );
	CPPUNIT_TEST( evaluateRightAttrVarNameLeftString );
	CPPUNIT_TEST( evaluateRightAttrProcNameLeftString );
	CPPUNIT_TEST( evaluateRightAttrProcNameLeftAttrProcName );
	CPPUNIT_TEST( evaluateRightAttrVarNameLeftAttrVarName );
	CPPUNIT_TEST( evaluateRightAttrStmtNumLeftAttrStmtNum );
	CPPUNIT_TEST( evaluateRightAttrValueLeftAttrValue );
	CPPUNIT_TEST( evaluateRightAttrProcNameLeftAttrVarName );
	CPPUNIT_TEST( evaluateRightAttrStmtNumLeftAttrValue );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// 1 = 1
	void evaluateRightIntLeftInt();

	// "asd" = "asd"
	void evaluateRightStringLeftString();

	// n1 = n2 where n1, n2 are prog_line
	void evaluateRightSynLeftSyn();

	// n = 1 where n is prog_line
	void evaluateRightSynLeftInt();

	// s.stmt# = 1 where s is stmt
	void evaluateRightAttrStmtNumLeftInt();

	// c.value = 1 where c is constant
	void evaluateRightAttrValueLeftInt();

	// v.varName = "x" where v is var
	void evaluateRightAttrVarNameLeftString();

	// p.procName = "proc" where p is procedure
	void evaluateRightAttrProcNameLeftString();

	// p1.procName = p2.procName
	void evaluateRightAttrProcNameLeftAttrProcName();

	// v1.varName = v2.varName
	void evaluateRightAttrVarNameLeftAttrVarName();

	// s1.stmt# = s2.stmt#
	void evaluateRightAttrStmtNumLeftAttrStmtNum();

	// c1.value = c2.value
	void evaluateRightAttrValueLeftAttrValue();

	// p.procName = v.varName
	void evaluateRightAttrProcNameLeftAttrVarName();

	// s.stmt# = c.value
	void evaluateRightAttrStmtNumLeftAttrValue();

};