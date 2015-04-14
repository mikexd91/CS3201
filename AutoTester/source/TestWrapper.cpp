#include <iostream>
#include <fstream>
#include <sstream>
#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
	counter = 1;
	parser = new Parser();
	pqlController = new PQLController();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	// ...rest of your code...

	// read the file as given by filename
	ifstream in(filename);
	stringstream buffer;
    buffer << in.rdbuf();
    string programSource = buffer.str();
	// call parser.parse on the string
	parser->parse(programSource);

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	counter;
	// eval the query
	set<string> resultSet = pqlController->parse(query);
	// get results
	// iterate through the results and stuff them into the results list
	set<string>::iterator iter;
	for (iter = resultSet.begin(); iter != resultSet.end(); ++iter) {
		results.push_back(*iter);
	}
	counter++;
}
