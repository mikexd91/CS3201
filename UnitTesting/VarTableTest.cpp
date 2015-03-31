#include <cppunit/config/SourcePrefix.h>
#include "VarTableTest.h"

#include <iostream>
#include <string>

using namespace std;

void VarTableTest::setUp() {

}

void VarTableTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VarTableTest );
	VarTable vt = *VarTable::getInstance();

void VarTableTest::testProperties() { 

	printf("%d\n", &vt);

	Variable x = *(new Variable("x"));
	vt.addVariable(&x);
	Variable* vget = vt.getVariable("x");
	CPPUNIT_ASSERT_EQUAL(&x, vget);
	string expname = "x";
	CPPUNIT_ASSERT_EQUAL(expname, vget->getName());

	return;
}

void VarTableTest::testLinks() {

	VarTable vtt = *VarTable::getInstance();
	printf("%d\n", &vt);
	printf("%d\n", &vtt);

	//Variable x("x");
	//Variable* vget = vt.contains("x");
	CPPUNIT_ASSERT(vt.contains("x"));
	//CPPUNIT_ASSERT_EQUAL(&x, vget);
	string expname = "x";
	//CPPUNIT_ASSERT_EQUAL(expname, vget->getName());

	return;
}