#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>


#include "InvalidExpressionException.h"
#include "StmtTable.h"

using namespace std;

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
vector<string> Utils::explode(const string &str, const string delim_string, const vector<char> delimiters) {

	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = str.find_first_of(delim_string, prev)) != string::npos) {
		//if there are words in between (not consecutive delimiters)
		if (pos > prev) {
			elems.push_back(str.substr(prev, pos - prev));
		}
		//if it is a delimiter that should be included (aka all delimiters but spaces)
		if (find(begin(delimiters), end(delimiters), str[pos]) != end(delimiters)) {
			elems.push_back(string(1, str[pos]));
		}
		prev = pos + 1;
	}
	return elems;
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

//Check if a string is a valid operator
bool Utils::isValidOperator(string symbol) {
	return UtilsConstants::OPERATOR_PRIORITIES.find(symbol) != UtilsConstants::OPERATOR_PRIORITIES.end();
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
