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

string QueryParser::removeFrontSpaces(string s){
	while (s.at(0) == ' '){
		s.erase(0);
	} 
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

bool QueryParser::containsOperator(string s){
	vector<string> opVector;
	opVector.push_back("+");
	opVector.push_back("/");
	opVector.push_back("-");
	opVector.push_back("*");
	return containsAny(s, opVector);
}

string QueryParser::getFirstOperator(string s){
	for (size_t i=0; i<s.size(); i++){
		char current = s.at(i);
		if (current == '+'){
			return "+";
		} else if (current == '-'){
			return "-";
		} else if (current == '/'){
			return "/";
		} else if (current == '*'){
			return "*";
		}
	}
	return stringconst::STRING_EMPTY;
}

int QueryParser::getOperatorIndex(string s){
	for (size_t i=0; i<s.size(); i++){
		char current = s.at(i);
		if ((current == '+') || (current == '-') || (current == '*') || (current == '/')){
			return i;
		}
	}
	return -1;
}

queue<string> QueryParser::exprBuilder(string input){
	vector<string> elems;
	string duplicate = input;
	while (containsOperator(duplicate)){
		int index = getOperatorIndex(duplicate);
		string op = getFirstOperator(duplicate);
		string left = duplicate.substr(0, index);
		string right = duplicate.substr(index+1, duplicate.size() - index);
		elems.push_back(left);
		elems.push_back(op);
		duplicate = right;
	}
	elems.push_back(duplicate);
	queue<string> toReturn;
	for (size_t i = 0; i<elems.size(); i++){
		string current = elems.at(i);
		string toReplace = removeSpace(current);
		toReturn.push(toReplace);
	}
	return toReturn;
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
		string here = list.at(i);
		string current = removeFrontSpaces(here);
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
	size_t startIndex = current.find_first_of("(");
	size_t endIndex = current.find_first_of(",");
	string clauseType = current.substr(0, startIndex);
	newClause = createCorrectClause(clauseType);
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
				string first = firstArg.substr(start+1, end-start-1);
				newClause->setFirstArg(first);
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
				string second = secondArg.substr(start+1, end-start-1);
				newClause->setSecondArg(second);
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
	bool varFixed = false;
	if (decList.find(var) == decList.end()){
		if (contains(var, "\"")){
			size_t spos = var.find_first_of("\"");
			size_t fpos = var.find_last_of("\"");
			if (spos == fpos){
				throw InvalidArgumentException();
			} else {
				string arg = var.substr(spos+1, fpos-spos-1);
				var = arg;
				varFixed = true;
			}
		}
	}
	std::stringstream ss = stringstream();
	bool endToken = false;
	bool readBegin = false;
	bool readEnd = false;
	Utils::getWordAndPop(line);
	string subsequent = line.front();
	endToken = contains(subsequent, ")");
	while (!endToken){
		if (!readBegin){
			//TO READ : _"
			if (!(contains(subsequent, "_") || contains(subsequent, "\""))){
				throw InvalidArgumentException();
			}
			readBegin = true;
			size_t bpos = subsequent.find_first_of("_");
			size_t epos = subsequent.find_first_of("\"");
			if (bpos != epos - 1 || bpos != 0){
				throw InvalidArgumentException();
			}
			size_t len = subsequent.size();
			if (len > 2){
				string exprPart = subsequent.substr(2);
				ss << exprPart;
			}
		} else {
			//TO READ (EXPR || EXPR"_)
			if (!contains(subsequent, "_") && !contains(subsequent, "\"")){
				ss << " " << subsequent;
			} else {
				if (contains(subsequent, "_") && contains(subsequent, "\"")){
					size_t epos = subsequent.find_first_of("\"");
					size_t fpos = subsequent.find_first_of("_");
					size_t len = subsequent.size();
					if ((epos != fpos -1) && fpos != len - 1){
						throw InvalidArgumentException();
					}
					string exprPart = subsequent.substr(0, epos);
					ss << " " << exprPart;
					readEnd = true;
				} else {
					throw InvalidArgumentException();
				}
			}
		}
		Utils::getWordAndPop(line);
		subsequent = line.front();
		endToken = contains(subsequent, ")");
	}
	if (!readBegin){
		//ONLY ONE TOKEN FOR WHOLE EXPRESSION
		size_t bpos = subsequent.find_first_of("_");
		size_t fpos = subsequent.find_last_of("_");
		if (bpos == fpos){
			if (bpos == 0 && !contains(subsequent, "\"")){
				PatternAssgClause* newClause = new PatternAssgClause(synonym, var, stringconst::STRING_EMPTY);
				newClause->setVarFixed(varFixed);
				query->addClause(newClause);
			} else {
				throw InvalidArgumentException();
			}
		} else {
			size_t spos = subsequent.find_first_of("\"");
			size_t epos = subsequent.find_last_of("\"");
			if (spos == epos){
				throw InvalidArgumentException();
			} else {
				string exprPart = subsequent.substr(spos + 1, epos - spos - 1);
				queue<string> expression = exprBuilder(exprPart);
				queue<string> exprRPN = Utils::getRPN(expression);
				string expr = queueToString(exprRPN);
				PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
				newClause->setVarFixed(varFixed);
				query->addClause(newClause);
			}
		}
	} else if (!readEnd){
		//TOKEN CONTAINS END OF EXPRESSION
		size_t epos = subsequent.find_first_of("\"");
		size_t fpos = subsequent.find_first_of("_");
		if (fpos != epos +1){
			throw InvalidArgumentException();
		} else {
			string exprPart = subsequent.substr(0, epos);
			ss << " " << exprPart;
			string expressionS = ss.str();
			cout << expressionS;
			queue<string> expressionQ = exprBuilder(expressionS);
			queue<string> exprRPN = Utils::getRPN(expressionQ);
			string expr = queueToString(exprRPN);
			PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
			newClause->setVarFixed(varFixed);
			query->addClause(newClause);
		}
	} else {
		//TOKEN CONTAINS ONLY )
		Utils::getWordAndPop(line);
	}
} 

Query QueryParser::parseQuery(string input){
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
			//selectQueue.pop();
		}
		selectQueue.pop();
	}
	vector<Clause*> clauseList = output->getClauseList();
	for (size_t i=0; i<clauseList.size(); i++){
		Clause* current = clauseList.at(i);
		if (!current->isValid()){
			throw InvalidClauseException();
		}
	}
	return *output;
}

