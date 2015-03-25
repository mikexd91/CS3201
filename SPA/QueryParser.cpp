#include "QueryParser.h"
#include "Query.h"
#include "Clause.h"
#include "Pair.h"
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

QueryParser::QueryParser(void){
}

QueryParser::~QueryParser(void){
}

vector<string> split(string s, char delim, vector<string> elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> QueryParser::tokeniser(string input, char delim){
	vector<string> elems;
    split(input, delim, elems);
    return elems;
}

/*
template <typename string>
Query QueryParser::processQuery(string input){
	Query parsedQuery = Query();
	vector<string> declarationTokens = tokeniser(input, ';');
	int numSynonyms = declarationTokens.size() - 1;
	string selectStatement = declarationTokens.at(numSynonyms);
	declarationTokens.pop_back();
	vector<Pair> declarations = vector<Pair>();
	for (int i=0; i<numSynonyms; i++){
		string currentDeclaration = declarationTokens.at(i);
		vector<string> declarationPair = tokenister(currentDeclaration, ' ');
		Pair currentPair = Pair();
		currentPair.setFirst(declarationPair.at(0));
		currentPair.setSecond(declarationPair.at(1));
	}
}*/