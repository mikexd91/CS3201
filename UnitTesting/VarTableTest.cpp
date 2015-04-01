#include <cppunit/config/SourcePrefix.h>
#include "VarTableTest.h"

#include <iostream>
#include <string>

using namespace std;

VarTable vt = *(VarTable::getInstance());

void VarTableTest::setUp() {
	Variable* x = new Variable("x");
	//Variable xx("x"); Variable* x = &xx;
	printf("\n");
	printf("x: %p\n", x);
	printf("name: %s\n", x->getName().c_str());
	vt.addVariable(x);
	printf("%d\n", vt.contains("x"));
}

void VarTableTest::tearDown() {

}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VarTableTest );

void VarTableTest::testProperties() {
	CPPUNIT_ASSERT(dynamic_cast<VarTable*>(&vt));

	printf("vt addr: %p\n", &vt);
	
	Variable* newx = new Variable("x");
	printf("newx: %p\n", newx);
	printf("newname: %s\n", newx->getName().c_str());

	Variable* vget = vt.getVariable("x");
	CPPUNIT_ASSERT(NULL != vget);
	printf("vget: %p\n", vget);
	printf("name: %s\n", vget->getName().c_str()); 

	string expname = "x";
	CPPUNIT_ASSERT_EQUAL(expname, vget->getName());

	return;
}

void VarTableTest::testLinks() {

	VarTable* vtt = (VarTable::getInstance());
	printf("%p\n", &vt);
	printf("%p\n", vtt->contains("x"));

	//Variable x("x");
	//Variable* vget = vt.contains("x");
	CPPUNIT_ASSERT(vt.contains("x"));
	//CPPUNIT_ASSERT_EQUAL(&x, vget);
	string expname = "x";
	//CPPUNIT_ASSERT_EQUAL(expname, vget->getName());

	return;
}