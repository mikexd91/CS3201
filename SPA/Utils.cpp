#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>

#include "InvalidExpressionException.h"

using namespace std;

string Utils::sanitise(string str) {
	for (size_t i = 0; i < strlen(UtilsConstants::SANITISED_CHARS); i++) {
		char sanitisedChar = UtilsConstants::SANITISED_CHARS[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

//Split code into a vector of tokens
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

void Utils::parseSymbol(string word, queue<string> &expressionQueue, stack<string> &operationStack) {
	//if top of stack is *, all other operation (+-*) are lower or equal, so just add top to output queue
	//if top of stack is + or -, only add top to output queue if word is + or -
	if (isValidSymbol(word)) {
		while (!operationStack.empty() && !(operationStack.top() != "*" && word == "*")) {
			expressionQueue.push(operationStack.top());
			operationStack.pop();
		}
		operationStack.push(word);
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}
}

bool Utils::isValidName(string name) {
	if (!isalpha(name[0])) {
		return false;
	} else {
		for (int i = 1; i < name.size(); i++) {
			if(!isalnum(name[i])) {
				return false;
			}
		}
		return true;
	}
}

bool Utils::isValidConstant(string number) {
	for (int i = 0; i < number.size(); i++) {
		if (!isdigit(number[i])) {
			return false;
		}
	}
	return true;
}

bool Utils::isValidFactor(string factor) {
	return isValidConstant(factor) || isValidName(factor);
}

bool Utils::isValidSymbol(string symbol) {
	return symbol == "+" || symbol == "-" || symbol == "*";
}
