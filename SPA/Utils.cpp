#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>
#include "boost/assign.hpp"

#include "InvalidExpressionException.h"
#include "StmtTable.h"

using namespace std;

const boost::unordered_map<std::string, int> OPERATOR_PRIORITIES = boost::assign::map_list_of ("+", 2) ("-", 2) ("*", 3);

//Removes the sanitised characters from str, and return a new string
string Utils::sanitise(string str) {
	for (size_t i = 0; i < strlen(UtilsConstants::SANITISED_CHARS); i++) {
		char sanitisedChar = UtilsConstants::SANITISED_CHARS[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

//Split code into a vector of tokens, depending on delim_string and delimiters.
//delim_string contains all of the delimiters, including those that should not be in the final token list. 
//delimiters consists of characters that should be included in the final vector, but should be delimited and separated accordingly
//this means that delim_string should be a superset of delimiters
vector<string> Utils::explode(const string &str, const string delim_string, const char delimiters[7] ) {
	char delimitersToUse[7]; 
	for (int i = 0; i < 7; i++) {
		delimitersToUse[i] = delimiters[i];
	}

	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = str.find_first_of(delim_string, prev)) != string::npos) {
		//if there are words in between (not consecutive delimiters)
		if (pos > prev) {
			elems.push_back(str.substr(prev, pos - prev));
		}
		//if it is a delimiter that should be included (aka all delimiters but spaces)
		if (find(begin(delimitersToUse), end(delimitersToUse), str[pos]) != end(delimitersToUse)) {
			elems.push_back(string(1, str[pos]));
		}
		prev = pos + 1;
	}
	return elems;
}

//takes in a queue of assignment expression, with every word and symbol separated.
//e.g. a+b+c -> ["a", "+", "b", "+", "c"]
//returns another queue of the assignment expression in its RPN format
//e.g. a+b+c-> ["a",  "b", "+", "c" "+", ]
queue<string> Utils::getRPN(queue<string> originalExpression) {
	stack<string> operationStack;
	queue<string> expressionQueue;
	//using Shunting-yard algorithm
	string word;
	int count = 0;
	word = getWordAndPop(originalExpression);
	parseFactor(word, expressionQueue);
	while (!originalExpression.empty()) {
		word = getWordAndPop(originalExpression);
		parseSymbol(word, expressionQueue, operationStack);
		word = getWordAndPop(originalExpression);
		parseFactor(word, expressionQueue);
	}
	while (!operationStack.empty()) {
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	return expressionQueue;
}

//checks that the queue is not empty, then gets the next element of the queue and pop the head
string Utils::getWordAndPop(queue<string> &originalExpression) {
	if(originalExpression.empty()) {
		throw InvalidExpressionException("Invalid Expression!");
	} else {
		string word = originalExpression.front();
		originalExpression.pop();
		return word;
	}
}

//Needed for generating RPN. Checks the symbol.
//It also adds the past symbols to the expression queue (if needed) and current symbol to the operation stack
void Utils::parseSymbol(string word, queue<string> &expressionQueue, stack<string> &operationStack) {
	//if top of stack is *, all other operation (+-*) are lower or equal, so just add top to output queue
	//if top of stack is + or -, only add top to output queue if word is + or -
	if (isValidSymbol(word)) {
		while (!operationStack.empty() && OPERATOR_PRIORITIES.at(word) < OPERATOR_PRIORITIES.at(operationStack.top())) {
			expressionQueue.push(operationStack.top());
			operationStack.pop();
		}
		operationStack.push(word);
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}
}

//Needed for generating RPN. Checks and adds the factor to the expression queue.
void Utils::parseFactor(string word, queue<string> &expressionQueue) {
	if (isValidFactor(word)) {
		expressionQueue.push(word);
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}
}

//Check if a string is a valid variable name
bool Utils::isValidName(string name) {
	if (!isalpha(name[0])) {
		return false;
	} else {
		for (size_t i = 1; i < name.size(); i++) {
			if(!isalnum(name[i])) {
				return false;
			}
		}
		return true;
	}
}

//Check if a string is a valid constant
bool Utils::isValidConstant(string number) {
	for (size_t i = 0; i < number.size(); i++) {
		if (!isdigit(number[i])) {
			return false;
		}
	}
	return true;
}

//Check if a string is a valid factor (constant or name)
bool Utils::isValidFactor(string factor) {
	return isValidConstant(factor) || isValidName(factor);
}

//Check if a string is a valid symbol
bool Utils::isValidSymbol(string symbol) {
	return symbol == "+" || symbol == "-" || symbol == "*";
}

//Checks if the arg type and statement type are matching
bool Utils::isSameType(string argType, NodeType stmt) {
	//TODO implement all other types
	if(argType==stringconst::ARG_STATEMENT
		|| (argType==stringconst::ARG_WHILE && stmt==WHILE_STMT_)
		|| (argType==stringconst::ARG_ASSIGN && stmt==ASSIGN_STMT_)) {

		return true;
	}

	return false;
}

//Converts statement NodeType to ArgType for clauses
string Utils::convertNodeTypeToArgType(NodeType stmtType) {
	switch(stmtType) {
		case ASSIGN_STMT_:
			return stringconst::ARG_ASSIGN;
		case IF_STMT_:
			//return stringconst::ARG_IF;
		case WHILE_STMT_:
			return stringconst::ARG_WHILE;
	}

	return NULL;
}


//Converts statement NodeType to ArgType for clauses
NodeType Utils::convertArgTypeToNodeType(string stmtType) {
	if (stmtType == stringconst::ARG_ASSIGN) {
		return ASSIGN_STMT_;
	} else if (stmtType == stringconst::ARG_WHILE) {
		return WHILE_STMT_;
	} else {
		return NULL_;
	}
}


//filters the set to retrieve the statement of the specified type
set<int> Utils::filterStatements(set<int> stmtSet, NodeType type) {
	StmtTable * stmtTable = StmtTable::getInstance();
	set<int>::iterator it;
	set<int> finalValue;
	for (it = stmtSet.begin(); it != stmtSet.end(); ++it) {
		Statement* currentStmt = stmtTable->getStmtObj(*it);
		if (currentStmt->getType() == type) {
			finalValue.insert(*it);
		}
	}
	return finalValue;
}

void Utils::removeVectorDupes(vector<string> &vec) {
	// convert to set then back to vector
	set<string> s(vec.begin(), vec.end());
	vec.clear();
	vec.assign(s.begin(), s.end());
}

void Utils::removeVectorDupes(vector<pair<string,string>> &vec) {
	// convert to set then back to vector
	set<pair<string,string>> s(vec.begin(), vec.end());
	vec.clear();
	vec.assign(s.begin(), s.end());
}
