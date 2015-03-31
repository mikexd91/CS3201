#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
// Get the top level suite from the registry
CppUnit::TestSuite *unitSuite = new CppUnit::TestSuite( "All unit test" );
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("TNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProgNodeTest").makeTest());
CppUnit::TestFactoryRegistry::getRegistry().addTestToSuite(unitSuite);
CppUnit::TextUi::TestRunner runner;


runner.addTest(unitSuite);
bool wasSucessful = runner.run();

//getchar();

return wasSucessful ? 0 : 1;
}