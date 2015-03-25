#include <stdio.h>
#include <iostream>
#include "Parser.h"

int main() {
	/* rewrite as necessary */

	Parser parser = Parser();

	string programSource = "simple source code";
	parser.parse(programSource);

	// designExtractor.extract();

	string queryString = "pql query string";
	// QueryProcessor queryProcessor = QueryProcessor();
	// result = queryProcessor.evaluate(queryString);
	// queryProjector.project(result);

	return 0;
}