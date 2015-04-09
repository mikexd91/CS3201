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
#include "PatternAssgClause.h"
#include "boost/unordered_map.hpp"
#include <queue>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using boost::unordered_map;
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
	return stringconst::STRING_EMPTY;
}

Clause* createCorrectClause(string type){
	Clause* c;
	if (type == stringconst::TYPE_FOLLOWS){
		FollowsClause* clause = new FollowsClause();
		c = clause;		
	} else if (type == stringconst::TYPE_PARENT){
		ParentClause* clause = new ParentClause();
		c = clause;		
	} else if (type == stringconst::TYPE_MODIFIES){
		ModifiesClause* clause = new ModifiesClause();
		c = clause;		
	} else if (type == stringconst::TYPE_USES){
		UsesClause* clause = new UsesClause();
		c = clause;		
	} else if (type == stringconst::TYPE_FOLLOWS_STAR){
		FollowsStarClause* clause = new FollowsStarClause();
		c = clause;		
	} else if (type == stringconst::TYPE_PARENT_STAR){
		ParentStarClause* clause = new ParentStarClause();
		c = clause;		
	}
	return c;
}

void parseSelect(vector<string> tokens, Query query){
	//get the var table
	unordered_map<string, string> decList = query.getDeclarationList();
	bool expectSelect = true;
	bool expectClause = false;
	bool expectPattern = false;
	bool expectExpression = false;
	bool expressionBegin = false;
	vector<Clause*> currentClause;
	string patternExpression = "";
	if (tokens.size() == 1 || tokens.at(0) != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	}
	//Select
	for (size_t i=1; i<tokens.size(); i++){
		string current = tokens.at(i);
		if (expectSelect){
			if (!containsKeyword(current)){
				StringPair newSelect = StringPair();
				newSelect.setFirst(current);
				if (decList.find(current) == decList.end()){
					throw MissingDeclarationException();
				} else {
					string currentType = decList.at(current);
					newSelect.setSecond(currentType);
					query.addSelectSynonym(newSelect);
				}
			} else {
				expectSelect = false;
			}
		} else if (expectClause){
			size_t endIndex = current.find_first_of(")");
			string secondArg = current.substr(0, endIndex);
			currentClause.at(0)->setSecondArg(secondArg);
			if (Utils.isNumber(secondArg)){
				currentClause.at(0)->setSecondArgType(stringconst::ARG_STATEMENT);
				currentClause.at(0)->setSecondArgFixed(true);
			} else if (contains(secondArg, "\"")){
				size_t first = secondArg.find_first_of("\"");
				size_t end = secondArg.find_last_of("\"");
				if (first==end){
					throw InvalidArgumentException();
				} else {
					secondArg = secondArg.substr(first, end-first);
					currentClause.at(0)->setSecondArg(secondArg);
					currentClause.at(0)->setSecondArgFixed(true);
					currentClause.at(0)->setSecondArgType(stringconst::ARG_VARIABLE);
				}
			} else {
				if (decList.find(secondArg) == decList.end()){
					throw MissingDeclarationException();
				} else {
					string currentType = decList.at(secondArg);
					currentClause.at(0)->setSecondArgType(currentType);
				}
			}
			query.addClause(currentClause.at(0));
			currentClause.pop_back();
			expectClause = false;
		} else if (expectPattern){
			if (expectExpression){
				if (!expressionBegin){
					size_t size = current.size();
					if (size == 2){
						if (current.at(0) == '_' && current.at(1) == '"'){
							expressionBegin = true;
						}
					} else if (size > 2){
						if (current.at(0) == '_' && current.at(1) == '"'){
							expressionBegin = true;

						}
					} else {
						if (current != stringconst::STRING_EMPTY){
							throw InvalidArgumentException();
						} else {
							currentClause.at(0)->setExpression(current);
							expectPattern = false;
							expectExpression = false;
						}
					}
				} else {
					
				}
			} else {
				size_t startIndex = current.find_first_of("(");
				size_t endIndex = current.find_first_of(",");
				string firstArg = current.substr(0, startIndex);
				string secondArg = current.substr(startIndex, endIndex-startIndex);
				if (decList.find(firstArg) == decList.end()){
					throw MissingDeclarationException();
				} else if (decList.at(firstArg) != stringconst::ARG_ASSIGN){
					throw InvalidDeclarationException();
				} else {
					PatternAssgClause* newClause = new PatternAssgClause(firstArg);
					if (decList.find(secondArg) == decList.end()){
						size_t first = secondArg.find_first_of("\"");
						size_t end = secondArg.find_last_of("\"");
						if (first==end){
							throw InvalidArgumentException();
						} else {
							secondArg = secondArg.substr(first, end-first);
							newClause->setVar(secondArg);
							newClause->setVarType(true);
							newClause->setSecondArgType(stringconst::ARG_VARIABLE);
						}
					} else {
						string secondType = decList.find(secondArg);
						if (secondType != stringconst::ARG_VARIABLE){
							throw InvalidDeclarationException();
						} else {
							newClause->setVar(secondArg);
							newClause->setVarFixed(false);
						}
					}
				}
				currentClause.push_back(newClause);
				expectExpression = true;
			}
		} else {
			if (containsClauseType(current)){
				expectClause = true;
				Clause* newClause;
				newClause = createCorrectClause(getClauseString(current));
				size_t startIndex = current.find_first_of("(");
				size_t endIndex = current.find_first_of(",");
				string firstArg = current.substr(startIndex, endIndex - startIndex);
				newClause->setFirstArg(firstArg);
				if (Utils::isNumber(firstArg)){
					newClause->setFirstArgType(stringconst::ARG_STATEMENT);
					newClause->setFirstArgFixed(true);
				} else if (contains(firstArg, "\"")){
					size_t first = firstArg.find_first_of("\"");
					size_t end = firstArg.find_last_of("\"");
					if (first==end){
						throw InvalidArgumentException();
					} else {
						firstArg = firstArg.substr(first, end-first);
						newClause->setFirstArg(firstArg);
						newClause->setFirstArgFixed(true);
						newClause->setFirstArgType(stringconst::ARG_VARIABLE);
					}
				} else {
					if (decList.find(firstArg) == decList.end()){
						throw MissingDeclarationException();
					} else {
						string currentType = decList.at(firstArg);
						newClause->setFirstArgType(currentType);
					}
				}
				currentClause.push_back(newClause);
			} else if (contains(current, stringconst::TYPE_PATTERN){
				expectPattern = true;
			} else {
				if (!containsKeywword(current)){
					throw InvalidSelectException();
				}
			}
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
	
	unordered_map<string, string> *declarations = new unordered_map<string, string>();
	for (int i=0; i<numSynonyms; i++){
		string currentDeclaration = declarationTokens.at(i);
		vector<string> declarationPair = tokeniser(currentDeclaration, ' ');
		string variable = declarationPair.at(1);
		string type = declarationPair.at(0);
		declarations->emplace(variable, type);
	}
	parsedQuery.setDeclarationList(*declarations);
	selectStatement = util.sanitise(selectStatement);
	vector<string> tokens = util.explode(selectStatement);
	parseSelect(tokens, parsedQuery);
	
	return parsedQuery;
}

void createClause(Query query, string argument){
	if (decList.find(argument) == decList.end()){
		if (!isNumber(argument)){
			if (!contains(argument, "\"")){
				throw MissingDeclarationException();
			} else {
				size_t start = argument.find_first_of("\"");
				size_t end = argument.find_last_of("\"");
				string firstArg = secondArg.substr(start+1, end-start-1);
				newClause->setSecondArg(secondArg);
				newClause->setSecondArgFixed(true);
				newClause->setSecondArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setSecondArg(secondArg);
			newClause->setSecondArgFixed(true);
			newClause->setSecondArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string secondArgType = decList.at(secondArg);
		newClause->setSecondArg(secondArg);
		newClause->setSecondArgFixed(false);
		newClause->setSecondArgType(secondArgType);
	}
}

void parseSelectSynonyms(Query query, queue<string> line){
	unordered_map<string, string> decList = query.getDeclarationList();
	string first = getWordAndPop(line);
	if (first != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	} else {
		bool expectSelect = true;
		while (expectSelect){
			string current = getWordAndPop(line);
			if (decList.find(current) == decList.end()){
				throw MissingDeclarationException();
			} else {
				string type = decList.at(current);
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				query.addSelectSynonym(*newPair);
			}
			string next = line.front();
			if (containsKeyword(next)){
				expectSelect = false;
			}
		}
	}
}

void parseClause(Query query, queue<string> line){
	unordered_map<string, string> decList = query.getDeclarationList();
	string current = getWordAndPop(line);
	string next = line.front();
	if (!contains(next, ")")){
		throw InvalidArgumentException();
	}
	Clause* newClause;
	newClause = createCorrectClause(current);
	size_t startIndex = current.find_first_of("(");
	size_t endIndex = current.find_first_of(",");
	string firstArg = current.substr(startIndex+1, endIndex-startIndex-1);
	size_t last_index = next.find_first_of(")");
	string secondArg = next.substr(0,  last_index);
	if (decList.find(firstArg) == decList.end()){
		if (!isNumber(firstArg)){
			if (!contains(firstArg, "\"")){
				throw MissingDeclarationException();
			} else {
				size_t start = current.find_first_of("\"");
				size_t end = current.find_last_of("\"");
				string firstArg = firstArg.substr(start+1, end-start-1);
				newClause->setFirstArg(firstArg);
				newClause->setFirstArgFixed(true);
				newClause->setFirstArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setFirstArg(firstArg);
			newClause->setFirstArgFixed(true);
			newClause->setFirstArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string firstArgType = decList.at(firstArg);
		newClause->setFirstArg(firstArg);
		newClause->setFirstArgFixed(false);
		newClause->setFirstArgType(firstArgType);
	}

	if (decList.find(secondArg) == decList.end()){
		if (!isNumber(secondArg)){
			if (!contains(secondArg, "\"")){
				throw MissingDeclarationException();
			} else {
				size_t start = current.find_first_of("\"");
				size_t end = current.find_last_of("\"");
				string firstArg = secondArg.substr(start+1, end-start-1);
				newClause->setSecondArg(secondArg);
				newClause->setSecondArgFixed(true);
				newClause->setSecondArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setSecondArg(secondArg);
			newClause->setSecondArgFixed(true);
			newClause->setSecondArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string secondArgType = decList.at(secondArg);
		newClause->setSecondArg(secondArg);
		newClause->setSecondArgFixed(false);
		newClause->setSecondArgType(secondArgType);
	}
	query.addClause(newClause);
	line.pop();
}

void parsePattern(Query query, queue<string> line){
	unordered_map<string, string> decList = query.getDeclarationList();
	string current = getWordAndPop(line);
	string next = line.front();
	
}