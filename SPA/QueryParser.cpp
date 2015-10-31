#include "QueryParser.h"
#include "Query.h"
#include "StringPair.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "VarTable.h"
#include "ProcTable.h"
#include "Clause.h"
#include "FollowsClause.h"
#include "FollowsStarClause.h"
#include "ModifiesClause.h"
#include "ParentClause.h"
#include "ParentStarClause.h"
#include "PatternClause.h"
#include "UsesClause.h"
#include "CallsClause.h"
#include "PatternIfClause.h"
#include "PatternAssgClause.h"
#include "PatternWhileClause.h"
#include "PatternClauseBuilder.h"
#include "ExpressionParser.h"
#include "WithClauseRef.h"
#include "WithClauseBuilder.h"
#include "WithClause.h"
#include "boost/unordered_map.hpp"
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <ios>

using namespace std;
using boost::unordered_map;
using namespace boost;

bool QueryParser::instanceFlag = false;		// instance flag
QueryParser* QueryParser::parser = NULL;	// parser instance

// SINGLETON ACCESSOR
QueryParser* QueryParser::getInstance() {
	// if instance does not exist, create and return instance
	if(!instanceFlag) {
		parser = new QueryParser();
		instanceFlag = true;
	}

	return parser;
}

// empty private constructor
QueryParser::QueryParser() {}


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

//queueBuilder must take in empty queue
void QueryParser::queueBuilder(string in, queue<string>* out){
	queue<string>* queryQueue = &queue<string>();
	vector<string> temp = vector<string>();
	QueryParser::tokeniser(in, ' ', &temp);
	QueryParser::splitByDelims(&temp, "(", temp);
	QueryParser::splitByDelims(&temp, ")", temp);
	QueryParser::splitByDelims(&temp, ",", temp);
	QueryParser::splitByDelims(&temp, "\"", temp);
	QueryParser::splitByDelims(&temp, "+", temp);
	QueryParser::splitByDelims(&temp, "-", temp);
	QueryParser::splitByDelims(&temp, "*", temp);
	QueryParser::splitByDelims(&temp, "-", temp);
	QueryParser::splitByDelims(&temp, ".", temp);
	QueryParser::splitByDelims(&temp, "=", temp);
	QueryParser::splitByDelims(&temp, "<", temp);
	QueryParser::splitByDelims(&temp, ">", temp);
	for (size_t i=0; i<temp.size(); i++){
		out->push(temp.at(i));
	}
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

void QueryParser::tokeniser(string input, char delim, vector<string>* elems){
	size_t pos = input.find_first_of(delim);
	while (pos != string::npos){
		if (pos == 0){
			string temp = input.substr(1);
			input = temp;
			pos = temp.find_first_of(delim);
		} else if (pos == input.length()){
			elems->push_back(input.substr(0, pos - 1));
			pos = string::npos;
		} else {
			string s = input.substr(0, pos);
			elems->push_back(s);
			input = input.substr(pos + 1);
			pos = input.find_first_of(delim);
		}
	}
	if (pos == string::npos && !input.empty()){
		elems->push_back(input);
	}
}

bool QueryParser::containsOperator(string s){
	vector<string> opVector;
	opVector.push_back("+");
	opVector.push_back("/");
	opVector.push_back("-");
	opVector.push_back("*");
	return containsAny(s, &opVector);
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

bool QueryParser::containsAny(string s, vector<string>* list){
	for (size_t i=0; i<list->size(); i++){
		string current = list->at(i);
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
	decVector.push_back(stringconst::ARG_CALL);
	return containsAny(s, &decVector);
}

bool QueryParser::containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_CALLS);
	clauseVector.push_back(stringconst::TYPE_NEXT);
	clauseVector.push_back(stringconst::TYPE_AFFECTS);
	return containsAny(s, &clauseVector);
}

bool QueryParser::containsKeyword(string s){
	vector<string> wordVector;
	wordVector.push_back(stringconst::STRING_SUCH);
	wordVector.push_back(stringconst::STRING_THAT);
	return containsAny(s, &wordVector);
}

string QueryParser::getClauseString(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_CALLS);
	clauseVector.push_back(stringconst::TYPE_NEXT);
	clauseVector.push_back(stringconst::TYPE_AFFECTS);
	for (size_t i=0; i<clauseVector.size(); i++){
		string current = clauseVector.at(i);
		if (contains(s, current)){
			return current;
		}
	}
	return stringconst::STRING_EMPTY;
}

SuchThatClauseBuilder* QueryParser::createCorrectClause(string type, queue<string>* line){
	string isStar = line->front();
	if (type == stringconst::TYPE_FOLLOWS){
		if (isStar == "*"){
			Utils::getWordAndPop(*line);
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(FOLLOWSSTAR_);
			return clause;
		} else {
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(FOLLOWS_);
			return clause;
		}
	} else if (type == stringconst::TYPE_PARENT){
		if (isStar == "*"){
			Utils::getWordAndPop(*line);
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(PARENTSTAR_);
			return clause;
		} else {
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(PARENT_);
			return clause;
		}
	} else if (type == stringconst::TYPE_MODIFIES){
		SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(MODIFIES_);
		return clause;		
	} else if (type == stringconst::TYPE_USES){
		SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(USES_);
		return clause;
	} else if (type == stringconst::TYPE_CALLS){
		if (isStar == "*"){
			Utils::getWordAndPop(*line);
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(CALLSSTAR_);
			return clause;
		} else {
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(CALLS_);
			return clause;
		}
	} else if (type == stringconst::TYPE_NEXT){
		if (isStar == "*"){
			Utils::getWordAndPop(*line);
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(NEXTSTAR_);
			return clause;
		} else {
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(NEXT_);
			return clause;
		}
	} else if (type == stringconst::TYPE_AFFECTS){
		if (isStar == "*"){
			Utils::getWordAndPop(*line);
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(AFFECTSSTAR_);
			return clause;
		} else {
			SuchThatClauseBuilder* clause = new SuchThatClauseBuilder(AFFECTS_);
			return clause;
		}
	} else {
		throw UnexpectedClauseException();
	}
}

void QueryParser::parseDeclarations(Query* query, vector<string>* list){
	for (size_t i=0; i<list->size(); i++){
		unordered_map<string, string> decList = query->getDeclarationList();
		string current = list->at(i);
		boost::algorithm::trim(current);
		vector<string> tokens = vector<string>();
		tokeniser(current, ',', &tokens);
		string first = tokens.at(0);
		vector<string> split = vector<string>();
		tokeniser(first, ' ', &split);
		string decType = split.at(0); 
		boost::trim(decType);
		if (!containsDeclarationType(decType)){
			cout << decType;
			throw InvalidDeclarationException();
		}
		StringPair* newPair = new StringPair();
		newPair->setFirst(split.at(1));
		newPair->setSecond(decType);
		query->addDeclaration(*newPair);

		if (tokens.size() > 1){
			for (size_t i=1; i<tokens.size(); i++){
				string here = tokens.at(i);
				here = removeSpace(here);
				if (decList.find(here) != decList.end()){
					throw DuplicateDeclarationException();
				}
				StringPair* newPair = new StringPair();
				newPair->setFirst(here);
				newPair->setSecond(decType);
				query->addDeclaration(*newPair);
			}
		}
	}
}

void QueryParser::unexpectedEndCheck(queue<string>* in){
	if (in->empty()){
		throw UnexpectedEndException();
	}
}

//TODO add attribute keyword list
void QueryParser::parseSelectSynonyms(Query* query, queue<string>* line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string first = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (first != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	} else {
		string current = Utils::getWordAndPop(*line);
		if (current == "<"){
			bool expectSelect = true;
			while (expectSelect){
				string syn = Utils::getWordAndPop(*line);
				if (decList.find(syn) == decList.end()){
					cout << "missing dec: " << syn;
					throw MissingDeclarationException();
				}
				string type = decList.at(syn);
				StringPair* newPair = new StringPair();
				newPair->setFirst(syn);
				newPair->setSecond(type);
				query->addSelectSynonym(*newPair);
				string next = line->front();
				if (next == ">"){
					expectSelect = false;
				} else if (next != ","){
					throw InvalidSyntaxException();
				}
				Utils::getWordAndPop(*line);
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
					cout << "missing dec: " << current;
					throw MissingDeclarationException();
				}
				string type = decList.at(current);
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				string next;
				if (!line->empty()){
					next = line->front();
				}
				if (next == "."){
					Utils::getWordAndPop(*line);
					unexpectedEndCheck(line);
					string attr = Utils::getWordAndPop(*line);
					if (attr == stringconst::ATTR_COND_PROCNAME){
						if (type != stringconst::ARG_PROCEDURE && type != stringconst::ARG_CALL){
							cout << type << " & " << attr << "mismatch";
							throw InvalidAttributeException();
						} else {
							newPair->setAttribute(attr);
						}
					} else if (attr == stringconst::ATTR_COND_STMTNUM){
						if (type != stringconst::ARG_STATEMENT && type != stringconst::ARG_ASSIGN && type != stringconst::ARG_CALL && type != stringconst::ARG_IF && type!= stringconst::ARG_WHILE){
							cout << type << " & " << attr << "mismatch";
							throw InvalidAttributeException();
						} else {
							newPair->setAttribute(attr);
						}
					} else if (attr == stringconst::ATTR_COND_VALUE){
						if (type != stringconst::ARG_CONSTANT){
							cout << type << " & " << attr << "mismatch";
							throw InvalidAttributeException();
						} else {
							newPair->setAttribute(attr);
						}
					} else if (attr == stringconst::ATTR_COND_VARNAME){
						if (type != stringconst::ARG_VARIABLE){
							cout << type << " & " << attr << "mismatch";
							throw InvalidAttributeException();
						} else {
							newPair->setAttribute(attr);
						}
					}
				}
				query->addSelectSynonym(*newPair);
			}
		}
	}
}

void QueryParser::parseClause(Query* query, queue<string>* line){
	unordered_map<string, string> decList = query->getDeclarationList();
	bool expectFirstFixedSynonym = false;
	bool expectSecondFixedSynonym = false;
	VarTable* vtable = VarTable::getInstance();
	ProcTable* ptable = ProcTable::getInstance();

	string clauseType = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	SuchThatClauseBuilder* newClause = createCorrectClause(clauseType, line);

	string openParen = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}

	string firstVar = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (firstVar == "\""){
		expectFirstFixedSynonym = true;
		firstVar = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (firstVar == "\""){
			throw InvalidSyntaxException();
		}
	} /*else if (firstVar == "-"){
		string firstVal = Utils::getWordAndPop(line);
		unexpectedEndCheck(line);
		if (!firstVal){

		}
	}*/
	newClause->setArgFixed(1, expectFirstFixedSynonym);
	if (decList.find(firstVar) == decList.end()){
		if (!Utils::isValidConstant(firstVar)){
			if (!expectFirstFixedSynonym){
				if (firstVar != stringconst::STRING_EMPTY){
					cout << "missing dec: " << firstVar;
					throw MissingDeclarationException();
				} else {
					newClause->setArg(1, stringconst::STRING_EMPTY);
					newClause->setArgType(1, stringconst::ARG_GENERIC);
				}
			} else {
				newClause->setArg(1, firstVar);
				if (vtable->contains(firstVar)){
					newClause->setArgType(1, stringconst::ARG_VARIABLE);
				} else if (ptable->contains(firstVar)){
					newClause->setArgType(1, stringconst::ARG_PROCEDURE);
				} else {
					cout << "no such variable or procedure name" << endl;
					throw InvalidArgumentException();
				}
			}
		} else {
			newClause->setArgFixed(1, true);
			newClause->setArg(1, firstVar);
			newClause->setArgType(1, stringconst::ARG_STATEMENT);
		}
	} else {
		string firstArgType = decList.at(firstVar);
		newClause->setArg(1, firstVar);
		newClause->setArgType(1, firstArgType);
	}
	if (expectFirstFixedSynonym){
		string closeFixed = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (closeFixed != "\""){
			throw InvalidSyntaxException();
		}
	}

	string comma = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}

	string secondVar = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (secondVar == "\""){
		expectSecondFixedSynonym = true;
		secondVar = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (secondVar == "\""){
			throw InvalidSyntaxException();
		}
	}
	newClause->setArgFixed(2, expectSecondFixedSynonym);
	if (decList.find(secondVar) == decList.end()){
		if (!Utils::isValidConstant(secondVar)){
			if (!expectSecondFixedSynonym){
				if (secondVar != stringconst::STRING_EMPTY){
					throw MissingDeclarationException();
				} else {
					newClause->setArg(2, stringconst::STRING_EMPTY);
					newClause->setArgType(2, stringconst::ARG_GENERIC);
				}
			} else {
				newClause->setArg(2, secondVar);
				if (vtable->contains(secondVar)){
					newClause->setArgType(2, stringconst::ARG_VARIABLE);
				} else if (ptable->contains(secondVar)){
					newClause->setArgType(2, stringconst::ARG_PROCEDURE);
				} else {
					cout << "no such variable or procedure name" << endl;
					throw InvalidArgumentException();
				}
			}
		} else {
			newClause->setArgFixed(2, true);
			newClause->setArg(2, secondVar);
			newClause->setArgType(2, stringconst::ARG_STATEMENT);
		}
	} else {
		string secondArgType = decList.at(secondVar);
		newClause->setArg(2, secondVar);
		newClause->setArgType(2, secondArgType);
	}
	if (expectSecondFixedSynonym){
		string closeFixed = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (closeFixed != "\""){
			throw InvalidSyntaxException();
		}
	}

	string closeParen = Utils::getWordAndPop(*line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}
	Clause* clause = (Clause*)newClause->build();
	query->addClause(clause);
}

void QueryParser::parsePattern(Query* query, queue<string>* line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string patternType;

	string synonym = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (decList.find(synonym) == decList.end()){
		cout << "missing dec: " << synonym;
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

void QueryParser::parsePatternOther(Query* query, queue<string>* line, string synonym){
	unordered_map<string, string> decList = query->getDeclarationList();
	string patternType = decList.at(synonym);
	string var;
	bool varFixed = false;
	string varType;
	string expr;

	string openParen = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}
	
	var = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (var == "\""){
		varFixed = true;
		varType = stringconst::ARG_VARIABLE;
		var = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (var == "\""){
			throw InvalidSyntaxException();
		}
	} else if (var == stringconst::STRING_EMPTY){
		varType = stringconst::ARG_GENERIC;
	} else {
		if (decList.find(var) == decList.end()){
			cout << "missing dec: " << var;
			throw MissingDeclarationException();
		} else {
			varType = decList.at(var);
		}
	}

	if (varFixed){
		string close = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (close != "\""){
			throw InvalidSyntaxException();
		}
	}

	string comma = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}

	string condition = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (condition == stringconst::STRING_EMPTY){
		if (line->front() == ")"){
			expr = condition;
		} else if (line->front() == "\""){
			Utils::getWordAndPop(*line);
			unexpectedEndCheck(line);
			string next = Utils::getWordAndPop(*line);
			unexpectedEndCheck(line);
			if (next == "\""){
				throw InvalidSyntaxException();
			}
			queue<string> exprHolder;
			while (next != "\""){
				exprHolder.push(next);
				next = Utils::getWordAndPop(*line);
				unexpectedEndCheck(line);
			}
			string endBound = Utils::getWordAndPop(*line);
			unexpectedEndCheck(line);
			if (endBound != "_"){
				throw InvalidSyntaxException();
			}
			ExpressionParser expP;
			expr = "_\"" + queueToString(expP.getRPN(exprHolder)) + "\"_";
		}
	} else if (condition == "\""){
		queue<string> exprHolder;
		string next = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (next == "\""){
			throw InvalidSyntaxException();
		}
		while (next != "\""){
			exprHolder.push(next);
			next = Utils::getWordAndPop(*line);
			unexpectedEndCheck(line);
		}
		ExpressionParser expP;
		expr = "\"" + queueToString(expP.getRPN(exprHolder)) + "\"";
	} else {
		throw InvalidSyntaxException();
	}

	string closeParen = Utils::getWordAndPop(*line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	if (patternType == stringconst::ARG_ASSIGN){
		PatternClauseBuilder* assgBuilder = new PatternClauseBuilder(PATTERNASSG_);
		assgBuilder->setSynonym(synonym);
		assgBuilder->setVar(var);
		assgBuilder->setVarType(varType);
		assgBuilder->setVarFixed(varFixed);
		assgBuilder->setExpr(1, expr);
		Clause* newClause = (Clause*)assgBuilder->build();
		query->addClause(newClause);
	} else if (patternType == stringconst::ARG_WHILE){
		PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
		whileBuilder->setSynonym(synonym);
		whileBuilder->setVar(var);
		whileBuilder->setVarType(varType);
		whileBuilder->setVarFixed(varFixed);
		whileBuilder->setExpr(1, expr);
		Clause* newClause = (Clause*)whileBuilder->build();
		query->addClause(newClause);
	} else {
		throw ParseTimeException();
	}
}

void QueryParser::parsePatternIf(Query* query, queue<string>* line, string synonym){
	unordered_map<string, string> decList = query->getDeclarationList();
	string var;
	bool varFixed = false;
	string varType;
	string expr1;
	string expr2;

	string openParen = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}

	var = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (var == "\""){
		varFixed = true;
		var = Utils::getWordAndPop(*line);
		varType = stringconst::ARG_VARIABLE;
		string close = Utils::getWordAndPop(*line);
		unexpectedEndCheck(line);
		if (close != "\""){
			throw InvalidSyntaxException();
		}
	} else {
		if (decList.find(var) == decList.end()){
			if (var != stringconst::STRING_EMPTY){
				cout << "missing dec: " << var;
				throw MissingDeclarationException();
			} else {
				varType = stringconst::ARG_GENERIC;
			}
		} else {
			varType = decList.at(var);
		}
	}

	string comma1 = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (comma1 != ","){
		throw InvalidSyntaxException();
	}

	expr1 = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (expr1 != "_"){
		throw InvalidSyntaxException();
	}

	string comma2 = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (comma2 != ","){
		throw InvalidSyntaxException();
	}

	expr2 = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);
	if (expr2 != "_"){
		throw InvalidSyntaxException();
	}

	string closeParen = Utils::getWordAndPop(*line);
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	PatternClauseBuilder* ifBuilder = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder->setSynonym(synonym);
	ifBuilder->setVar(var);
	ifBuilder->setVarType(varType);
	ifBuilder->setVarFixed(varFixed);
	ifBuilder->setExpr(1, expr1);
	ifBuilder->setExpr(2, expr2);
	Clause* newClause = (Clause*)ifBuilder->build();
	query->addClause(newClause);
}

void QueryParser::parseWith(Query* query, queue<string>* line){

	unordered_map<string, string> decList = query->getDeclarationList();
	WithClauseBuilder* withBuilder = new WithClauseBuilder(WITH_);

	string leftEntityValue = "";
	string leftEntityCond = "";
	string rightEntityValue = "";
	string rightEntityCond = "";
	string nextToken = "";

	leftEntityValue = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);

	if (Utils::isValidConstant(leftEntityValue)){
		withBuilder->setEntity(1, leftEntityValue);
		withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_INTEGER);
		withBuilder->setRefType(1, INTEGER_);
		withBuilder->setAttrType(1, NULLATTR_);
	} else if (leftEntityValue == "\""){
		leftEntityValue = Utils::getWordAndPop(*line);
		nextToken = line->front();
		if (nextToken != "\""){
			cout << "expected \", got " << nextToken;
			throw InvalidSyntaxException();
		} else {
			Utils::getWordAndPop(*line);
		}
		withBuilder->setEntity(1, leftEntityValue);
		withBuilder->setEntityType(1, stringconst::ENTITY_TYPE_IDENT);
		withBuilder->setRefType(1, IDENT_);
		withBuilder->setAttrType(1, NULLATTR_);
	} else if (decList.find(leftEntityValue) == decList.end()){
		cout << "missing declaration " << leftEntityValue;
		throw MissingDeclarationException();
	} else {
		string leftDeclarationType = decList.at(leftEntityValue);
		nextToken = line->front();
		withBuilder->setEntity(1, leftEntityValue);
		withBuilder->setEntityType(1, leftDeclarationType);
		if (nextToken == "="){
			withBuilder->setRefType(1, SYNONYM_);
			withBuilder->setAttrType(1, NULLATTR_);
		} else if (nextToken == "."){
			Utils::getWordAndPop(*line);
			unexpectedEndCheck(line);
			leftEntityCond = Utils::getWordAndPop(*line);
			if (leftEntityCond == stringconst::ATTR_COND_PROCNAME){
				if (leftDeclarationType != stringconst::ARG_PROCEDURE && leftDeclarationType != stringconst::ARG_CALL){
					cout << leftDeclarationType << " & " << leftEntityCond << "mismatch";
					throw InvalidAttributeException();
				}
				withBuilder->setRefType(1, ATTRREF_);
				withBuilder->setAttrType(1, PROCNAME_);
			} else if (leftEntityCond == stringconst::ATTR_COND_STMTNUM){
				if (leftDeclarationType != stringconst::ARG_STATEMENT
					&& leftDeclarationType != stringconst::ARG_ASSIGN
					&& leftDeclarationType != stringconst::ARG_IF
					&& leftDeclarationType != stringconst::ARG_WHILE
					&& leftDeclarationType != stringconst::ARG_CALL){
						cout << leftDeclarationType << " & " << leftEntityCond << "mismatch";
						throw InvalidAttributeException();
				}
				withBuilder->setRefType(1, ATTRREF_);
				withBuilder->setAttrType(1, STMTNUM_);
			} else if (leftEntityCond == stringconst::ATTR_COND_VALUE){
				if (leftDeclarationType != stringconst::ARG_CONSTANT){
					cout << leftDeclarationType << " & " << leftEntityCond << "mismatch";
					throw InvalidAttributeException();
				}
				withBuilder->setEntity(1, leftEntityValue);
				withBuilder->setRefType(1, ATTRREF_);
				withBuilder->setAttrType(1, CONSTVALUE_);
			} else if (leftEntityCond == stringconst::ATTR_COND_VARNAME){
				if (leftDeclarationType != stringconst::ARG_VARIABLE){
					cout << leftDeclarationType << " & " << leftEntityCond << "mismatch";
					throw InvalidAttributeException();
				}
				withBuilder->setRefType(1, ATTRREF_);
				withBuilder->setAttrType(1, VARNAME_);
			} else {
				cout << "unknown attr cond";
				throw InvalidSyntaxException();
			}
		}
	}

	nextToken = Utils::getWordAndPop(*line);
	unexpectedEndCheck(line);

	if (nextToken != "="){
		throw InvalidSyntaxException();
	}

	rightEntityValue = Utils::getWordAndPop(*line);

	if (Utils::isValidConstant(rightEntityValue)){
		withBuilder->setEntity(2, rightEntityValue);
		withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_INTEGER);
		withBuilder->setRefType(2, INTEGER_);
		withBuilder->setAttrType(2, NULLATTR_);
	} else if (rightEntityValue == "\""){
		unexpectedEndCheck(line);
		rightEntityValue = Utils::getWordAndPop(*line);
		nextToken = line->front();
		if (nextToken != "\""){
			cout << "expected \", got " << nextToken;
			throw InvalidSyntaxException();
		} else {
			Utils::getWordAndPop(*line);
		}
		withBuilder->setEntity(2, rightEntityValue);
		withBuilder->setEntityType(2, stringconst::ENTITY_TYPE_IDENT);
		withBuilder->setRefType(2, IDENT_);
		withBuilder->setAttrType(2, NULLATTR_);
	} else if (decList.find(rightEntityValue) == decList.end()){
		cout << "missing declaration " << rightEntityValue;
		throw MissingDeclarationException();
	} else {
		string rightDeclarationType = decList.at(rightEntityValue);
		if (line->empty()){
			withBuilder->setEntity(2, rightEntityValue);
			withBuilder->setEntityType(2, rightDeclarationType);
			withBuilder->setRefType(2, SYNONYM_);
			withBuilder->setAttrType(2, NULLATTR_);
		} else {
			nextToken = line->front();
			withBuilder->setEntity(2, rightEntityValue);
			withBuilder->setEntityType(2, rightDeclarationType);
			if (nextToken == "="){
				withBuilder->setRefType(2, SYNONYM_);
				withBuilder->setAttrType(2, NULLATTR_);
			} else if (nextToken == "."){
				Utils::getWordAndPop(*line);
				unexpectedEndCheck(line);
				rightEntityCond = Utils::getWordAndPop(*line);
				if (rightEntityCond == stringconst::ATTR_COND_PROCNAME){
					if (rightDeclarationType != stringconst::ARG_PROCEDURE && rightDeclarationType != stringconst::ARG_CALL){
						cout << rightDeclarationType << " & " << rightEntityCond << "mismatch";
						throw InvalidAttributeException();
					}
					withBuilder->setRefType(2, ATTRREF_);
					withBuilder->setAttrType(2, PROCNAME_);
				} else if (rightEntityCond == stringconst::ATTR_COND_STMTNUM){
					if (rightDeclarationType != stringconst::ARG_STATEMENT
						&& rightDeclarationType != stringconst::ARG_IF
						&& rightDeclarationType != stringconst::ARG_WHILE
						&& rightDeclarationType != stringconst::ARG_CALL
						&& rightDeclarationType != stringconst::ARG_ASSIGN){
							cout << rightDeclarationType << " & " << rightEntityCond << "`";
							throw InvalidAttributeException();
					}
					withBuilder->setRefType(2, ATTRREF_);
					withBuilder->setAttrType(2, STMTNUM_);
				} else if (rightEntityCond == stringconst::ATTR_COND_VALUE){
					if (rightDeclarationType != stringconst::ARG_CONSTANT){
						cout << rightDeclarationType << " & " << rightEntityCond << "mismatch";
						throw InvalidAttributeException();
					}
					withBuilder->setRefType(2, ATTRREF_);
					withBuilder->setAttrType(2, CONSTVALUE_);
				} else if (rightEntityCond == stringconst::ATTR_COND_VARNAME){
					if (rightDeclarationType != stringconst::ARG_VARIABLE){
						cout << rightDeclarationType << " & " << rightEntityCond << "mismatch";
						throw InvalidAttributeException();
					}
					withBuilder->setRefType(2, ATTRREF_);
					withBuilder->setAttrType(2, VARNAME_);
				} else {
					cout << "unknown attr cond";
					throw InvalidSyntaxException();
				}
			}
		}
	}
	WithClause* wClause = withBuilder->build();
	query->addClause(wClause);
}

Query* QueryParser::parseQuery(string input){

	Query* outputQuery = new Query();
	vector<string>* decVector = new vector<string>();
	tokeniser(input, ';', decVector);
	int numDecTypes = decVector->size() - 1;

	string queryLine = decVector->at(decVector->size() - 1);
	queue<string>* selectLine = new queue<string>();
	queueBuilder(queryLine, selectLine);
	decVector->pop_back();

	parseDeclarations(outputQuery, decVector);
	bool expectPattern = false;
	bool expectWith = false;

	while (!selectLine->empty()){

		string currentWord = selectLine->front();
		
		if (currentWord == stringconst::STRING_SELECT){
			
			parseSelectSynonyms(outputQuery, selectLine);
		
		} else if (containsClauseType(currentWord)){
			
			expectPattern = false;
			expectWith = false;
			parseClause(outputQuery, selectLine);
		
		} else if (currentWord == stringconst::TYPE_PATTERN){
			
			expectPattern = true;
			expectWith = false;
			string wordPattern = Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);
			parsePattern(outputQuery, selectLine);
		
		} else if (currentWord == stringconst::TYPE_WITH){
		
			expectPattern = false;
			expectWith = true;
			string wordWith = Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);
			parseWith(outputQuery, selectLine);
		
		} else if (currentWord == stringconst::STRING_AND && expectPattern){
		
			string wordAnd = Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);
			parsePattern(outputQuery, selectLine);

		} else if (currentWord == stringconst::STRING_AND && expectWith){
		
			string wordAnd = Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);
			parseWith(outputQuery, selectLine);

		} else if (currentWord == stringconst::STRING_AND && !expectWith && !expectPattern){

			Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);

		} else if (currentWord == stringconst::STRING_SUCH || currentWord == stringconst::STRING_THAT){
			
			expectPattern = false;
			expectWith = false;
			Utils::getWordAndPop(*selectLine);
			unexpectedEndCheck(selectLine);

		} else {
			
			cout << currentWord << " not in dictionary";
			throw InvalidSyntaxException();

		}

		if (selectLine->empty()){
			return outputQuery;
		}
	}

	return outputQuery;
}

void QueryParser::splitByDelims(vector<string>* out, string delim, vector<string> in){
	out->clear();
	for (size_t i = 0; i < in.size(); i++){
		string current = in.at(i);
		size_t pos = current.find_first_of(delim);
		while (pos != std::string::npos){
			string before = current.substr(0, pos);
			out->push_back(before);
			out->push_back(delim);
			string after = current.substr(pos+1);
			current = after;
			pos = current.find_first_of(delim);			
		}
		if (!current.empty()){
			out->push_back(current);
		}
	}
}