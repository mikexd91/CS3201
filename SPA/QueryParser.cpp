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
#include <exception>

using namespace std;
using boost::unordered_map;
using namespace boost;	

string QueryParser::removeSpace(string s){
	std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
	s.erase(end_pos, s.end());
	return s;
}

vector<string> QueryParser::split(string s, char delim, vector<string>* elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems->push_back(item);
    }
    return *elems;
}

queue<string> QueryParser::queueBuilder(string input, char delim){
	queue<string> elems;
	stringstream ss(input);
	string item;
	while (getline(ss, item, delim)){
		elems.push(item);
	}
	return elems;
}

string QueryParser::queueToString(queue<string> input){
	string item = Utils::getWordAndPop(input);
	stringstream ss("");
	ss << item;
	while (!input.empty()){
		item = Utils::getWordAndPop(input);
		ss << " " << item;
	}
	return ss.str();
}

vector<string> QueryParser::tokeniser(string input, char delim){
	vector<string>* elems = new vector<string>();
    split(input, delim, elems);
    return *elems;
}

bool QueryParser::containsAny(string s, vector<string> list){
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		if (contains(s, current)){
			return true;
		}
	}
	return false;
}

bool QueryParser::containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	return containsAny(s, clauseVector);
}

bool QueryParser::containsKeyword(string s){
	vector<string> wordVector;
	wordVector.push_back(stringconst::STRING_SUCH);
	wordVector.push_back(stringconst::STRING_THAT);
	wordVector.push_back(stringconst::STRING_WITH);
	wordVector.push_back(stringconst::STRING_AND);
	return containsAny(s, wordVector);
}

string QueryParser::getClauseString(string s){
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

Clause* QueryParser::createCorrectClause(string type){
	Clause* c;
	if (type == stringconst::TYPE_FOLLOWS_STAR){
		FollowsStarClause* clause = new FollowsStarClause();
		return clause;		
	} else if (type == stringconst::TYPE_PARENT_STAR){
		ParentStarClause* clause = new ParentStarClause();
		return clause;			
	} else if (type == stringconst::TYPE_FOLLOWS){
		FollowsClause* clause = new FollowsClause();
		return clause;		
	} else if (type == stringconst::TYPE_PARENT){
		ParentClause* clause = new ParentClause();
		return clause;		
	} else if (type == stringconst::TYPE_MODIFIES){
		ModifiesClause* clause = new ModifiesClause();
		return clause;		
	} else if (type == stringconst::TYPE_USES){
		UsesClause* clause = new UsesClause();
		return clause;		
	} else {
		throw UnexpectedClauseException();
	}
}

void QueryParser::parseDeclarations(Query* query, vector<string> list){
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		vector<string> tokens = tokeniser(current, ',');
		string first = tokens.at(0);
		vector<string> split = tokeniser(first, ' ');
		string decType = split.at(0);
		StringPair* newPair = new StringPair();
		newPair->setFirst(split.at(1));
		newPair->setSecond(decType);
		query->addDeclaration(*newPair);
		if (tokens.size() > 1){
			for (size_t i=1; i<tokens.size(); i++){
				string here = tokens.at(i);
				here = removeSpace(here);
				StringPair* newPair = new StringPair();
				newPair->setFirst(here);
				newPair->setSecond(decType);
				query->addDeclaration(*newPair);
			}
		}
	}
}

void QueryParser::parseSelectSynonyms(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string first = Utils::getWordAndPop(line);
	if (first != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	} else {
		bool expectSelect = true;
		while (expectSelect){
			string current = Utils::getWordAndPop(line);
			if (decList.find(current) == decList.end()){
				throw MissingDeclarationException();
			} else {
				string type = decList.at(current);
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				query->addSelectSynonym(*newPair);
			}
			if (line.empty()){
				expectSelect = false;
			} else {
				string next = line.front();
				if (containsKeyword(next)){
					expectSelect = false;
				}
			}
		}
	}
}

void QueryParser::parseClause(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string current = Utils::getWordAndPop(line);
	if (line.empty()){
		throw UnexpectedEndException();
	}
	string next = line.front();
	if (!contains(next, ")")){
		throw InvalidArgumentException();
	}
	Clause* newClause;
	newClause = createCorrectClause(clauseType);
	size_t startIndex = current.find_first_of("(");
	size_t endIndex = current.find_first_of(",");
	string firstArg = current.substr(startIndex+1, endIndex-startIndex-1);
	size_t last_index = next.find_first_of(")");
	string secondArg = next.substr(0,  last_index);
	if (decList.find(firstArg) == decList.end()){
		if (!Utils::isValidConstant(firstArg)){
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
		if (!Utils::isValidConstant(secondArg)){
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
	query->addClause(newClause);
	line.pop();
}

void QueryParser::parsePattern(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string current = Utils::getWordAndPop(line);
	if (line.empty()){
		throw UnexpectedEndException();
	}
	string next = line.front();
	size_t pos = next.find_first_of("(");
	string synonym = next.substr(0, pos);
	if (decList.find(synonym) == decList.end() || decList.at(synonym) != stringconst::ARG_ASSIGN){
		throw InvalidDeclarationException();
	}
	size_t npos = next.find_first_of(",");
	string var = next.substr(pos+1, npos-pos-1);
	if (decList.find(var) == decList.end() || decList.at(var) != stringconst::ARG_VARIABLE){
		throw InvalidDeclarationException();
	}
	std::stringstream ss = stringstream();
	string subsequent = line.front();
	bool begin = false;
	bool exprStart = false;
	while(!contains(subsequent, ")")){
		if (exprStart){
			ss << subsequent << " ";
		} else if (begin && !exprStart){
			if (!contains(subsequent, "\"")){
				throw InvalidArgumentException();
			} else {
				size_t len = subsequent.size();
				exprStart = true;
				if (len > 1){
					string s = subsequent.substr(1, len);
					ss << s << " ";
				}
			}
		} else if (!begin){
			if (contains(subsequent, "_")){
				begin = true;
				size_t len = subsequent.size();
				if (len == 2){
					if (!contains(subsequent, "\"")){
						throw InvalidArgumentException();
					} else {
						exprStart = true;
					}
				} else if (len > 2){
					if (!contains(subsequent, "\"")){
						throw InvalidArgumentException();
					} else {
						exprStart = true;
						string toAdd = subsequent.substr(2, len);
						ss << toAdd << " ";
					}
				}
			}
		}
		Utils::getWordAndPop(line);
		subsequent = line.front();
	}
	if (contains(subsequent, ")")){
		size_t pos = subsequent.find_first_of(")");
		string s = subsequent.substr(0, pos - 1);
		ss << s;
	}
	string expression = ss.str();
	if (expression != ""){
		queue<string> exprQ = queueBuilder(expression, ' ');
		queue<string> exprRPN = Utils::getRPN(exprQ);
		string expr = queueToString(exprRPN);
		PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
		query->addClause(newClause);
	} else {
		PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expression);
		query->addClause(newClause);
	}
} 

Query QueryParser::queryProcessor(string input){
	Query* output = new Query();
	vector<string> splitBySC = tokeniser(input, ';');
	int numDeclarations = splitBySC.size() - 1;
	string selectStatement = splitBySC.at(splitBySC.size()-1);
	queue<string> selectQueue = queueBuilder(selectStatement, ' ');
	splitBySC.pop_back();
	parseDeclarations(output, splitBySC);
	while(!selectQueue.empty()){
		string current = selectQueue.front();
		if (current == stringconst::STRING_SELECT){
			parseSelectSynonyms(output, selectQueue);
		} else if (containsClauseType(current)){
			parseClause(output, selectQueue);
		} else if (contains(current, stringconst::TYPE_PATTERN)){
			parsePattern(output, selectQueue);
		} else if (containsKeyword(current)){
			selectQueue.pop();
		}
	}
	return *output;
}

