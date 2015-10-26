#include <stdio.h>
#include <iostream>
#include "Parser.h"
#include "PQLController.h"
#include "InvalidCodeException.h"
#include "SuchThatClauseBuilder.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

#include "boost/foreach.hpp"

using namespace boost;

int main() {

	Parser parser = Parser();

	ifstream in("source_multi.txt");
	stringstream buffer;
    buffer << in.rdbuf();
    string programSource = buffer.str();

	try {
		parser.parse(programSource);
	} catch (InvalidCodeException) {
		cout << "Error";
		exit(EXIT_FAILURE);
	}

	PQLController* pqlController = new PQLController();
	string query = "procedure p1, p2, p3 Select p2 such that Calls(p1, p2) and Calls(p2, p3)";
	try {
		unordered_set<string> resultSet = pqlController->parse(query);
		BOOST_FOREACH(auto r, resultSet) {
			string variable = r;
		}
		delete pqlController;
	} catch (std::exception e) {
		delete pqlController;
	}

	return 0;
}