#include <stdio.h>
#include <iostream>
#include "Parser.h"
#include "InvalidCodeException.h"
#include "SuchThatClauseBuilder.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

int main() {

	/*SuchThatClauseBuilder* b = new SuchThatClauseBuilder(MODIFIES_);
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
	system("pause");*/
	/* rewrite as necessary */

	/*Parser parser = Parser();

	ifstream in("sample.txt");
	stringstream buffer;
    buffer << in.rdbuf();
    string programSource = buffer.str();

	try {
		parser.parse(programSource);
	} catch (InvalidCodeException) {
		cout << "Error";
		exit(EXIT_FAILURE);
	}*/

	// designExtractor.extract();

	string queryString = "pql query string";
	// QueryProcessor queryProcessor = QueryProcessor();
	// result = queryProcessor.evaluate(queryString);
	// queryProjector.project(result);



	return 0;
}