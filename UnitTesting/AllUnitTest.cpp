#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
// Get the top level suite from the registry

CppUnit::TestSuite *unitSuite = new CppUnit::TestSuite( "PKB Unit Tests" );

//Front End
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("PDRTest").makeTest());

//AST Components
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ASTTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("TNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProgNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProcNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("StmtLstNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("AssgNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("CallNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("IfNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("WhileNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ConstNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("VarNodeTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("OpNodeTest").makeTest());

// PKB Tables
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("VarTableTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("StmtTableTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProcTableTest").makeTest());

// PQL Clauses
/**
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("FollowsClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("FollowsStarClauseTest").makeTest());
**/

unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("UsesClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("PatternAssgClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ModifiesClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ParentClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("CallsClauseTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("CallsStarClauseTest").makeTest());

// PQL Evaluator
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ResultsTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("QueryEvaluatorTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("QueryParserTest").makeTest());

//Result
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("SingleSynInsertTest").makeTest());
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ResultTest").makeTest());

CppUnit::TestFactoryRegistry::getRegistry().addTestToSuite(unitSuite);
CppUnit::TextUi::TestRunner runner;


runner.addTest(unitSuite);
bool wasSuccessful = runner.run();

//getchar();

return wasSuccessful ? 0 : 1;
}