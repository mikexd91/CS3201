#pragma once
#include "Query.h"
#include "Clause.h"
#include "StringPair.h"
#include <string>
#include <vector>
#include <sstream>

using std::string;

class QueryParser
{

private:
	vector<string> split(string, char, vector<string>);
	vector<string> tokeniser(string, char);
	bool checkValidity();
public:
	QueryParser(void);
	~QueryParser(void);

	Query processQuery(string);
	
};

