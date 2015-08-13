#include <stdio.h>
#include <iostream>
#include "Parser.h"
#include "InvalidCodeException.h"
#include <fstream>
#include <sstream>

int main() {
	/* rewrite as necessary */

	Parser parser = Parser();

	ifstream in("sample.txt");
	stringstream buffer;
    buffer << in.rdbuf();
    string programSource = buffer.str();

	try {
		parser.parse(programSource);
	} catch (InvalidCodeException) {
		cout << "Error";
		exit(EXIT_FAILURE);
	}

	// designExtractor.extract();

	string queryString = "pql query string";
	// QueryProcessor queryProcessor = QueryProcessor();
	// result = queryProcessor.evaluate(queryString);
	// queryProjector.project(result);

	return 0;
}