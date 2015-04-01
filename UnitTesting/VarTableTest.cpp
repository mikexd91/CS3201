#include <cppunit/config/SourcePrefix.h>
#include "VarTableTest.h"

#include <iostream>
#include <string>

using namespace std;

VarTable vt = (VarTable::getInstance());
Variable* x = new Variable("x");

void VarTableTest::setUp() {
	vt.addVariable(x);
}

void VarTableTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VarTableTest );

void VarTableTest::testProperties() {
	CPPUNIT_ASSERT(dynamic_cast<VarTable*>(&vt));

	VarTable asd = VarTable::getInstance();
	VarTable qwe = VarTable::getInstance();

	//CPPUNIT_ASSERT_EQUAL(&asd, &qwe);
	CPPUNIT_ASSERT(asd.contains("x"));
	CPPUNIT_ASSERT(qwe.contains("x"));

	asd.addVariable(new Variable("y"));

	Variable* vget = vt.getVariable("x");
	CPPUNIT_ASSERT(NULL != vget);

	string expname = "x";
	CPPUNIT_ASSERT_EQUAL(expname, vget->getName());
	CPPUNIT_ASSERT_EQUAL(x, vget);

	return;
}

void VarTableTest::testLinks() {

	VarTable vtt = (VarTable::getInstance());
	CPPUNIT_ASSERT(vtt.contains("y"));

	return;
}