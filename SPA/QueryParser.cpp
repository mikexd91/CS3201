#include "QueryParser.h"
#include "Query.h"
#include "Clause.h"
#include "StringPair.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;
using namespace boost;

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

bool containsAny(string s, vector<string> list){
	for (int i=0; i<list.size(); i++){
		string current = list.at(i);
		if (contains(s, current)){
			return true;
		}
	}
	return false;
}

bool containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_CALLS);
	clauseVector.push_back(stringconst::TYPE_PATTERN);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	return containsAny(s, clauseVector);
}

string getClauseString(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_CALLS);
	clauseVector.push_back(stringconst::TYPE_PATTERN);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	for (int i=0; i<clauseVector.size(); i++){
		string current = clauseVector.at(i);
		if (contains(s, current)){
			return current;
		}
	}
}

void parseSelect(vector<string> tokens, Query query){
	bool expectingClause, insideClause, noClauses= false;
	for (int i=0; i<tokens.size(); i++){
		string current = tokens.at(i);
		if (i==0){
			if (!current.compare(stringconst::STRING_SELECT)){
				throw InvalidSelectException();
			}
		} else if (i==1){
			query.addSelectSynonym(current);
		} else {
			if (insideClause){

			} else if (expectingClause){
				if (!containsClauseType(current)){
					throw MissingClauseException();
				} else {
					insideClause = true;

				}
			} else {
				if (current.compare(stringconst::STRING_AND) || current.compare(stringconst::STRING_WITH)){
					if (i == tokens.size() -1){
						throw UnexpectedEndException();
					} else {
						expectingClause = true;
					}
				} else if (current.compare(stringconst::STRING_SUCH)){
					if (i == tokens.size() -1){
						throw UnexpectedEndException();
					} else {
						string next = tokens.at(i+1);
						if (!next.compare(stringconst::STRING_THAT)){
							throw InvalidSelectException();
						}
					}
				}
			}
		}
	}
	if (noClauses){
		throw UnexpectedEndException();
	}
}

Query QueryParser::processQuery(string input){
	Utils util;
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
	parsedQuery.setDeclarationList(declarations);
	selectStatement = util.sanitise(selectStatement);
	vector<string> tokens = util.explode(selectStatement);
}


