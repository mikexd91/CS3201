#include <stdio.h>
#include <iostream>
#include "PQLController.h"
#include "Parser.h"
#include "InvalidCodeException.h"
#include "SuchThatClauseBuilder.h"
#include "Utils.h"
#include "boost/unordered_set.hpp"
#include <fstream>
#include <sstream>

using namespace std;
using namespace boost;

int main() {
	/*
	SuchThatClauseBuilder* b = new SuchThatClauseBuilder(MODIFIES_);
	b->setArg(1, "a");
	b->setArgFixed(1, false);
	b->setArgType(1, stringconst::ARG_ASSIGN);
	b->setArg(2, "v");
	b->setArgFixed(2, false);
	b->setArgType(2, stringconst::ARG_VARIABLE);

	ModifiesClause* m = (ModifiesClause*) b->build();
	Results* r = new Results();
	r->setClauseFail();
	m->evaluate(r);

	cout << "asd" << m->isValid() << r->isClausePass() << "asd" << endl;
	system("pause");
	*/
	/* rewrite as necessary */

	Parser* parser = new Parser();
	PQLController* pqlController = new PQLController();

	// read the file as given by filename
	string filename = "SystemTests/Sources/System-Source-1.txt";
	ifstream in(filename);
	stringstream buffer;
    buffer << in.rdbuf();
    string programSource = buffer.str();
	// call parser.parse on the string
	try {
		parser->parse(programSource);
		delete parser;
	} catch (InvalidCodeException e) {
		cout << e.what();
		delete parser;
		exit(EXIT_FAILURE);
	}

	// designExtractor.extract();

	string queryString = "variable v; Select v such that Uses(1,v)";
	
	list<string> results = list<string>();
	try {
		unordered_set<string> resultSet = pqlController->parse(queryString);
		// get results
		// iterate through the results and stuff them into the results list
		unordered_set<string>::iterator iter;
		for (iter = resultSet.begin(); iter != resultSet.end(); ++iter) {
			results.push_back(*iter);
		}
		delete pqlController;
	} catch (std::exception e) {
		results.push_back(e.what());
		delete pqlController;
	}

	
	// QueryProcessor queryProcessor = QueryProcessor();
	// result = queryProcessor.evaluate(queryString);
	// queryProjector.project(result);



	return 0;
}