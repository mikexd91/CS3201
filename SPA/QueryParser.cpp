#include "QueryParser.h"
#include "Query.h"
#include "Clause.h"
#include "StringPair.h"
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

string removeAll(string source, string remove){
	while (source.find(remove) != string::npos){
		string::size_type index = source.find(remove);
		source.erase(index, remove.length());
	}
}

vector<string> findClauses(string sentence){
	vector<string> selectDeclarations;
}

Query QueryParser::processQuery(string input){
	Query parsedQuery = Query();
	vector<string> declarationTokens = tokeniser(input, ';');
	int numSynonyms = declarationTokens.size() - 1;
	string selectStatement = declarationTokens.at(numSynonyms);
	declarationTokens.pop_back();
	map<string, string> declarations = map<string, string>();
	for (int i=0; i<numSynonyms; i++){
		string currentDeclaration = declarationTokens.at(i);
		vector<string> declarationPair = tokeniser(currentDeclaration, ' ');
		declarations.insert(declarationPair.at(1), declarationPair.at(0));
	}
	string duplicate = selectStatement;

}


