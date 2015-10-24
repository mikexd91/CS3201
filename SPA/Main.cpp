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

	ifstream in("source.txt");
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
	string query = "Select BOOLEAN with 1 = 1";
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