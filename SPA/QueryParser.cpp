#include "QueryParser.h"
#include "Query.h"
#include "StringPair.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "Clause.h"
#include "FollowsClause.h"
//#include "FollowsStarClause.h"
#include "ModifiesClause.h"
#include "ParentClause.h"
//#include "ParentStarClause.h"
#include "PatternClause.h"
#include "UsesClause.h"
#include "PatternIfClause.h"
#include "PatternAssgClause.h"
#include "PatternWhileClause.h"
#include "boost/unordered_map.hpp"
#include "ExpressionParser.h"
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

//ADD OPERATORS TO Q: splitByDelims( operator )
queue<string> QueryParser::queueBuilder(string in){
	queue<string> out;
	vector<string> temp;
	vector<string> outHolder;
	temp = QueryParser::tokeniser(in, ' ');
	outHolder = QueryParser::splitByDelims(temp, "(");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ")");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ",");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "\"");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "+");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "-");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "*");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "-");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ".");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "=");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "<");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ">");
	for (size_t i=0; i<outHolder.size(); i++){
		out.push(outHolder.at(i));
	}
	return out;
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
	vector<string> elems;
	size_t pos = input.find_first_of(delim);
	while (pos != string::npos){
		if (pos == 0){
			string temp = input.substr(1);
			input = temp;
			pos = temp.find_first_of(delim);
		} else if (pos == input.length()){
			elems.push_back(input.substr(0, pos - 1));
			pos = string::npos;
		} else {
			string s = input.substr(0, pos);
			elems.push_back(s);
			input = input.substr(pos + 1);
			pos = input.find_first_of(delim);
		}
	}
	if (pos == string::npos && !input.empty()){
		elems.push_back(input);
	}
	return elems;
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

bool QueryParser::containsDeclarationType(string s){
	vector<string> decVector;
	decVector.push_back(stringconst::ARG_ASSIGN);
	decVector.push_back(stringconst::ARG_STATEMENT);
	decVector.push_back(stringconst::ARG_WHILE);
	decVector.push_back(stringconst::ARG_IF);
	decVector.push_back(stringconst::ARG_VARIABLE);
	decVector.push_back(stringconst::ARG_PROGLINE);
	decVector.push_back(stringconst::ARG_CONSTANT);
	decVector.push_back(stringconst::ARG_PROCEDURE);
	return containsAny(s, decVector);
}

bool QueryParser::containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	//clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	//clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
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
	//clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	//clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	for (size_t i=0; i<clauseVector.size(); i++){
		string current = clauseVector.at(i);
		if (contains(s, current)){
			return current;
		}
	}
	return stringconst::STRING_EMPTY;
}

Clause* QueryParser::createCorrectClause(string type){
	//if (type == stringconst::TYPE_FOLLOWS_STAR){
		//FollowsStarClause* clause = new FollowsStarClause();
		//return clause;		
	/*} else if (type == stringconst::TYPE_PARENT_STAR){
		ParentStarClause* clause = new ParentStarClause();
		return clause;
	} else*/ if (type == stringconst::TYPE_FOLLOWS){
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
	/*INSERT WHEN CLAUSES ARE DONE
	} else if (type == stringconst::TYPE_CALLS){
		CallsClause* clause = new CallsClause();
		return clause;
	} else if (type == stringconst::TYPE_NEXT){
		NextClause* clause = new NextClause();
		return clause;
	} else if (type == stringconst::TYPE_AFFECTS){
		AffectsClause* clause = new AffectsClause();
		return clause;
	} else if (type == stringconst::TYPE_NEXT_STAR){
		NextStarClause* clause = new NextStarClause();
		return clause;
	} else if (type == stringconst::TYPE_AFFECTS_STAR){
		AffectsStarClause* clause = new AffectsStarClause();
		return clause;
	}
	*/
	} else {
		throw UnexpectedClauseException();
	}
}

void QueryParser::parseDeclarations(Query* query, vector<string> list){
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		boost::algorithm::trim(current);
		vector<string> tokens = tokeniser(current, ',');
		string first = tokens.at(0);
		vector<string> split = tokeniser(first, ' ');
		string decType = split.at(0); 
		boost::trim(decType);
		if (!containsDeclarationType(decType)){
			throw InvalidDeclarationException();
		}
		if (decType == stringconst::ARG_PROGLINE){
			decType = stringconst::ARG_STATEMENT;
		}
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

void QueryParser::unexpectedEndCheck(queue<string> in){
	if (in.empty()){
		throw UnexpectedEndException();
	}
}

//TODO add attribute keyword list
void QueryParser::parseSelectSynonyms(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string first = Utils::getWordAndPop(line);
	if (first != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	} else {
		string current = Utils::getWordAndPop(line);
		if (current == "<"){
			bool expectSelect = true;
			while (expectSelect){
				string syn = Utils::getWordAndPop(line);
				if (decList.find(syn) == decList.end()){
					throw MissingDeclarationException();
				}
				string type = decList.at(syn);
				StringPair* newPair = new StringPair();
				newPair->setFirst(syn);
				newPair->setSecond(type);
				query->addSelectSynonym(*newPair);
				string next = line.front();
				if (next == ">"){
					expectSelect = false;
				} else if (next != ","){
					throw InvalidSyntaxException();
				}
				Utils::getWordAndPop(line);
			}
		} else {
			if (current == "BOOLEAN"){
				string type = stringconst::ARG_BOOLEAN;
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				query->addSelectSynonym(*newPair);		
			} else {
				if (decList.find(current) == decList.end()){
					throw MissingDeclarationException();
				}
				string type = decList.at(current);
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				string next;
				if (!line.empty()){
					next = line.front();
				}
				if (next == "."){
					Utils::getWordAndPop(line);
					unexpectedEndCheck(line);
					string attr = Utils::getWordAndPop(line);
					newPair->setAttribute(attr);
				}
				query->addSelectSynonym(*newPair);
			}
		}
	}
}

//TODO: UPDATE PARSE CLAUSE WITH NEW QUEUE (DONE, UNIT TESTING)
void QueryParser::parseClause(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	bool expectFirstFixed = false;
	bool expectSecondFixed = false;

	string clauseType = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	Clause* newClause = createCorrectClause(clauseType);

	string openParen = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}

	string firstVar = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (firstVar == "\""){
		expectFirstFixed = true;
		firstVar = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (firstVar == "\""){
			throw InvalidSyntaxException();
		}
	}
	newClause->setFirstArgFixed(expectFirstFixed);
	if (decList.find(firstVar) == decList.end()){
		if (!Utils::isValidConstant(firstVar)){
			if (!expectFirstFixed){
				if (firstVar != stringconst::STRING_EMPTY){
					throw MissingDeclarationException();
				} else {
					newClause->setFirstArg(stringconst::STRING_EMPTY);
					newClause->setFirstArgType(stringconst::ARG_GENERIC);
				}
			} else {
				newClause->setFirstArg(firstVar);
				newClause->setFirstArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setFirstArg(firstVar);
			newClause->setFirstArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string firstArgType = decList.at(firstVar);
		newClause->setFirstArg(firstVar);
		newClause->setFirstArgType(firstArgType);
	}
	if (expectFirstFixed){
		string closeFixed = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (closeFixed != "\""){
			throw InvalidSyntaxException();
		}
	}

	string comma = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}

	string secondVar = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (secondVar == "\""){
		expectSecondFixed = true;
		secondVar = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (secondVar == "\""){
			throw InvalidSyntaxException();
		}
	}
	newClause->setSecondArgFixed(expectSecondFixed);
	if (decList.find(secondVar) == decList.end()){
		if (!Utils::isValidConstant(secondVar)){
			if (!expectSecondFixed){
				if (secondVar != stringconst::STRING_EMPTY){
					throw MissingDeclarationException();
				} else {
					newClause->setSecondArg(stringconst::STRING_EMPTY);
					newClause->setSecondArgType(stringconst::ARG_GENERIC);
				}
			} else {
				newClause->setSecondArg(secondVar);
				newClause->setSecondArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setSecondArg(secondVar);
			newClause->setSecondArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string secondArgType = decList.at(secondVar);
		newClause->setSecondArg(secondVar);
		newClause->setSecondArgType(secondArgType);
	}
	if (expectSecondFixed){
		string closeFixed = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (closeFixed != "\""){
			throw InvalidSyntaxException();
		}
	}

	string closeParen = Utils::getWordAndPop(line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	query->addClause(newClause);
}

void QueryParser::parsePattern(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string patternType;

	string synonym = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (decList.find(synonym) == decList.end()){
		throw InvalidDeclarationException();
	} else {
		patternType = decList.at(synonym);
	}
	if (patternType == stringconst::ARG_ASSIGN || patternType == stringconst::ARG_WHILE){
		parsePatternOther(query, line, synonym);
	} else if (patternType == stringconst::ARG_IF){
		parsePatternIf(query, line, synonym);
	}
}

void QueryParser::parsePatternOther(Query* query, queue<string> line, string synonym){
	unordered_map<string, string> decList = query->getDeclarationList();
	string patternType = decList.at(synonym);
	string var;
	bool varFixed = false;
	string varType;
	string expr;

	string openParen = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}

	var = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (var == "\""){
		varFixed = true;
		varType = stringconst::ARG_VARIABLE;
		var = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (var == "\""){
			throw InvalidSyntaxException();
		}
	} else if (var == stringconst::STRING_EMPTY){
		varType = stringconst::ARG_GENERIC;
	} else {
		if (decList.find(var) == decList.end()){
			throw MissingDeclarationException();
		} else {
			varType = decList.at(var);
		}
	}

	if (varFixed){
		string close = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (close != "\""){
			throw InvalidSyntaxException();
		}
	}

	string comma = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}

	string condition = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (condition == stringconst::STRING_EMPTY){
		if (line.front() == ")"){
			expr = condition;
		} else if (line.front() == "\""){
			Utils::getWordAndPop(line);
			unexpectedEndCheck(line);
			string next = Utils::getWordAndPop(line);
			unexpectedEndCheck(line);
			if (next == "\""){
				throw InvalidSyntaxException();
			}
			queue<string> exprHolder;
			while (next != "\""){
				exprHolder.push(next);
				next = Utils::getWordAndPop(line);
				unexpectedEndCheck(line);
			}
			string endBound = Utils::getWordAndPop(line);
			unexpectedEndCheck(line);
			if (endBound != "_"){
				throw InvalidSyntaxException();
			}
			ExpressionParser expP;
			expr = "_\"" + queueToString(expP.getRPN(exprHolder)) + "\"_";
		}
	} else if (condition == "\""){
		queue<string> exprHolder;
		string next = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (next == "\""){
			throw InvalidSyntaxException();
		}
		while (next != "\""){
			exprHolder.push(next);
			next = Utils::getWordAndPop(line);
			unexpectedEndCheck(line);
		}
		ExpressionParser expP;
		expr = "\"" + queueToString(expP.getRPN(exprHolder)) + "\"";
	} else {
		throw InvalidSyntaxException();
	}

	string closeParen = Utils::getWordAndPop(line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	if (patternType == stringconst::ARG_ASSIGN){
		PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
		newClause->setVarFixed(varFixed);
		newClause->setVarType(varType);
		query->addClause(newClause);
	} else if (patternType == stringconst::ARG_WHILE){
		PatternWhileClause* newClause = new PatternWhileClause(synonym, var, expr);
		newClause->setVarFixed(varFixed);
		newClause->setVarType(varType);
		query->addClause(newClause);
	}
}

void QueryParser::parsePatternIf(Query* query, queue<string> line, string synonym){
	unordered_map<string, string> decList = query->getDeclarationList();
	string var;
	bool varFixed = false;
	string varType;
	string expr1;
	string expr2;

	string openParen = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}

	var = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (var == "\""){
		varFixed = true;
		var = Utils::getWordAndPop(line);
		varType = stringconst::ARG_VARIABLE;
		string close = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (close != "\""){
			throw InvalidSyntaxException();
		}
	} else {
		if (decList.find(var) == decList.end()){
			if (var != stringconst::STRING_EMPTY){
				throw MissingDeclarationException();
			} else {
				varType = stringconst::ARG_GENERIC;
			}
		} else {
			varType = decList.at(var);
		}
	}

	string comma1 = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (comma1 != ","){
		throw InvalidSyntaxException();
	}

	expr1 = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (expr1 != "_"){
		throw InvalidSyntaxException();
	}

	string comma2 = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (comma2 != ","){
		throw InvalidSyntaxException();
	}

	expr2 = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	if (expr2 != "_"){
		throw InvalidSyntaxException();
	}

	string closeParen = Utils::getWordAndPop(line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	PatternIfClause* newClause = new PatternIfClause(synonym, var, expr1, expr2);
	newClause->setVarFixed(varFixed);
	newClause->setVarType(varType);
	query->addClause(newClause);
}

Query QueryParser::parseQuery(string input){
	Query* output = new Query();
	vector<string> splitBySC = tokeniser(input, ';');
	int numDeclarations = splitBySC.size() - 1;
	string selectStatement = splitBySC.at(splitBySC.size()-1);
	queue<string> selectQueue = queueBuilder(selectStatement);
	splitBySC.pop_back();
	parseDeclarations(output, splitBySC);
	bool expectPattern = false;
	while(!selectQueue.empty()){
		string current = selectQueue.front();
		if (current == stringconst::STRING_SELECT){
			expectPattern = false;
			parseSelectSynonyms(output, selectQueue);
		} else if (containsClauseType(current)){
			expectPattern = false;
			parseClause(output, selectQueue);
		} else if (contains(current, stringconst::TYPE_PATTERN)){
			string wordPattern = Utils::getWordAndPop(selectQueue);
			unexpectedEndCheck(selectQueue);
			parsePattern(output, selectQueue);
			expectPattern = true;
		} else if (current == stringconst::STRING_AND && expectPattern){
			string wordAnd = Utils::getWordAndPop(selectQueue);
			unexpectedEndCheck(selectQueue);
			parsePattern(output, selectQueue);
		} else if (containsKeyword(current)){
			expectPattern = false;
		}
		selectQueue.pop();
	}
	vector<Clause*> clauseList = output->getClauseList();
	for (size_t i=0; i<clauseList.size(); i++){
		Clause* current = clauseList.at(i);
		//if (!current->isValid()){
			//throw InvalidClauseException();
		//}
	}
	return *output;
}

vector<string> QueryParser::splitByDelims(vector<string> in, string delim){
	vector<string> out;
	for (size_t i = 0; i < in.size(); i++){
		string current = in.at(i);
		size_t pos = current.find_first_of(delim);
		while (pos != std::string::npos){
			string before = current.substr(0, pos);
			out.push_back(before);
			out.push_back(delim);
			string after = current.substr(pos+1);
			current = after;
			pos = current.find_first_of(delim);			
		}
		if (!current.empty()){
			out.push_back(current);
		}
	}
	return out;
}