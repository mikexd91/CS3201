#include "QueryParser.h"
#include "Query.h"
#include "Clause.h"
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

QueryParser::QueryParser(void)
{
}

QueryParser::~QueryParser(void)
{
}

vector<string> split(string s, char delim, vector<string> elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> QueryParser::queryTokeniser(string input, char delim){
	vector<string> elems;
    split(input, delim, elems);
    return elems;
}

Query QueryParser::processQuery(string input){
	Query parsedQuery = Query();
	vector<string> tokens = queryTokeniser(input, ' ');
	
}