#include "QueryParser.h"
#include "Query.h"
#include "StringPair.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "Clause.h"
#include "FollowsClause.h"
#include "FollowsStarClause.h"
#include "ModifiesClause.h"
#include "ParentClause.h"
#include "ParentStarClause.h"
#include "PatternClause.h"
#include "UsesClause.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
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
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		if (contains(s, current)){
			return true;
		}
	}
	return false;
}

bool containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_PATTERN);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	return containsAny(s, clauseVector);
}

bool containsKeyword(string s){
	vector<string> wordVector;
	wordVector.push_back(stringconst::STRING_SUCH);
	wordVector.push_back(stringconst::STRING_THAT);
	wordVector.push_back(stringconst::STRING_WITH);
	wordVector.push_back(stringconst::STRING_AND);
	return containsAny(s, wordVector);
}

string getClauseString(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_PATTERN);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	for (size_t i=0; i<clauseVector.size(); i++){
		string current = clauseVector.at(i);
		if (contains(s, current)){
			return current;
		}
	}
}

Clause createCorrectClause(string type){
	Clause c;
	if (type.compare(stringconst::TYPE_PATTERN)){
		PatternClause clause;
		c = clause;
	} else if (type.compare(stringconst::TYPE_FOLLOWS)){
		FollowsClause clause;
		c = clause;		
	} else if (type.compare(stringconst::TYPE_PARENT)){
		ParentClause clause;
		c = clause;		
	} else if (type.compare(stringconst::TYPE_MODIFIES)){
		ModifiesClause clause;
		c = clause;		
	} else if (type.compare(stringconst::TYPE_USES)){
		UsesClause clause;
		c = clause;		
	} else if (type.compare(stringconst::TYPE_FOLLOWS_STAR)){
		FollowsStarClause clause;
		c = clause;		
	} else if (type.compare(stringconst::TYPE_PARENT_STAR)){
		ParentStarClause clause;
		c = clause;		
	}
	return c;
}

void parseSelect(vector<string> tokens, Query query){
	bool selectSynonyms = true;
	bool insideClause = false;
	bool insidePattern = false;
	vector<Clause*> currentClause;
	if (tokens.size() == 1 || !tokens.at(0).compare(stringconst::STRING_SELECT)){
		throw InvalidSelectException();
	}
	for (size_t i=1; i<tokens.size(); i++){
		string current = tokens.at(i);
		if (selectSynonyms){
			if (!current.compare(stringconst::STRING_SUCH)){
				StringPair newSelect;
				newSelect.setFirst(current);
				query.addSelectSynonym(newSelect);
			} else {
				selectSynonyms = false;
			}
		} else {
			if (insidePattern){
				if (contains(current, "(")){
					std::size_t index = current.find_first_of("(");
					std::size_t endIndex = current.find_first_of(",");
					string firstArg = current.substr(0, index);
					string secondArg = current.substr(index, endIndex - index);
					currentClause.at(0)->setFirstArg(firstArg);
					currentClause.at(0)->setSecondArg(secondArg);
				} else if (contains(current, ")")){
					insidePattern = false;
					std::size_t index = current.find_first_of(")");
					string patternArg = current.substr(0, index);
					//RPN method
					query.addClause(*currentClause.at(0));
					currentClause.pop_back();
					insidePattern = false;
				}
			} else if(insideClause){
				std::size_t index = current.find_first_of(")");
				string synonym = current.substr(0, index);
				currentClause.at(0)->setSecondArg(synonym);
				query.addClause(*currentClause.at(0));
				currentClause.pop_back();
				insideClause = false;
			} else {
				if (containsClauseType(current)){
					string clauseType = getClauseString(current);
					Clause newClause = createCorrectClause(current);
					if (clauseType.compare(stringconst::TYPE_PATTERN)){
						insidePattern = true;
						currentClause.push_back(&newClause);
					} else {
						insideClause = true;
						std::size_t index = current.find_first_of("(");
						std::size_t endIndex = current.find_first_of(",");
						string synonym = current.substr(index, endIndex);
						newClause.setFirstArg(synonym);
						currentClause.push_back(&newClause);
					}
				} else if (!containsKeyword(current)){
					throw InvalidSelectException();
				}
			}
		}
	}
	if (query.getSelectList().size() == 0){
		throw InvalidSelectException();
	}
}

void vetQuery(Query query){
	map<string, string> decList = query.getDeclarationList();
	vector<Clause> clauseList = query.getClauseList();
	vector<StringPair> selectList = query.getSelectList();
	for (size_t i=0; i<selectList.size(); i++){
		StringPair current = selectList.at(i);
		string currentSelect = current.getFirst();
		if (decList.find(currentSelect) == decList.end() ) {
			throw MissingDeclarationException();
		} else {
			string currentType = decList.at(currentSelect);
			selectList.at(i).setSecond(currentType);
		}
	}
	for (size_t i=0; i<clauseList.size(); i++){
		Clause current = clauseList.at(i);
		//DO CHECK FOR PATTERN
		string currentSynonymOne = current.getFirstArg();
		if (decList.find(currentSynonymOne) == decList.end() ) {
			throw MissingDeclarationException();
		} else {
			string currentTypeOne = decList.at(currentSynonymOne);
			current.setFirstArgType(currentTypeOne);
			//check for fixed statement number and variable call

		}
	}
}

Query QueryParser::processQuery(string input){
	Utils util;
	Query parsedQuery = Query();
	vector<string> declarationTokens = tokeniser(input, ';');
	int numSynonyms = declarationTokens.size() - 1;
	string selectStatement = declarationTokens.at(numSynonyms);
	declarationTokens.pop_back();
	unordered_map<string, string> declarations = unordered_map<string, string>();
	for (int i=0; i<numSynonyms; i++){
		string currentDeclaration = declarationTokens.at(i);
		vector<string> declarationPair = tokeniser(currentDeclaration, ' ');
		string variable = declarationPair.at(1);
		string type = declarationPair.at(0);
		declarations.emplace(variable, type);
	}
	parsedQuery.setDeclarationList(declarations);
	selectStatement = util.sanitise(selectStatement);
	vector<string> tokens = util.explode(selectStatement);
	parseSelect(tokens, parsedQuery);

	return parsedQuery;
}


