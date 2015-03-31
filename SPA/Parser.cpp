#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "InvalidCodeException.h"
#include "Parser.h"
#include "ParsedData.h"
#include "PDR.h"

using namespace std;

//Constructor

Parser::Parser() {
	nestingLevel = 0;
	parsedDataReceiver = PDR::getInstance();
}

//e.g. string content = "procedure test { a = 2; a = 5;} ";
void Parser::parse(string content) {
	content = sanitise(content);
	tokens = explode(content);
	iter = tokens.begin();
	program();
	endParse();
}

string Parser::sanitise(string str) {
	for (size_t i = 0; i < strlen(ParserConstants::SANITISED_CHARS); i++) {
		char sanitisedChar = ParserConstants::SANITISED_CHARS[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

//Split code into a vector of tokens
vector<string> Parser::explode(const string &str) {
	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = str.find_first_of(ParserConstants::DELIM_STRING, prev)) != string::npos) {
		//if there are words in between (not consecutive delimiters)
		if (pos > prev) {
			elems.push_back(str.substr(prev, pos - prev));
		}
		//if it is a delimiter that should be included (aka all delimiters but spaces)
		if (find(begin(ParserConstants::DELIMITERS), end(ParserConstants::DELIMITERS), str[pos]) != end(ParserConstants::DELIMITERS)) {
			elems.push_back(string(1, str[pos]));
		}
		prev = pos + 1;
	}
	return elems;
}

//checks if the next token matches the expected token. if it matches, continue
void Parser::match(string token) {
	if (nextToken == token) {
		//Token is present.
		getNextToken();
	}
	else {
		throw InvalidCodeException();
	}
}

void Parser::getNextToken() {
	if (iter < tokens.end()) {
		nextToken = *(iter++);
	} else {
		nextToken.clear();
	}
}

string Parser::getWord() {
	string result = nextToken;
	getNextToken();
	return result;
}

void Parser::program() {
	getNextToken();
	while (!nextToken.empty()) {
		procedure();
	}
}

void Parser::procedure() {
	match("procedure");
	string procName = getWord();
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, nestingLevel);
	procedure.setProcName(procName);
	parsedDataReceiver->processParsedData(procedure);
	match("{");
	stmtLst();
	match("}");
}

void Parser::stmtLst() {
	nestingLevel++;
	while (nextToken != "}") {
		stmt();		
	}
	nestingLevel--;
}

void Parser::stmt() {
	if (nextToken == "while") {
		parseWhile();
	} else {
		assign(); 
	}
}

void Parser::assign() {
	string var = getWord();
	match("=");
	string expression = getWord();
	ParsedData assignment = ParsedData(ParsedData::ASSIGNMENT, nestingLevel);
	assignment.setAssignVar(var);
	assignment.setAssignExpression(getExpression());
	parsedDataReceiver->processParsedData(assignment);
}


/**
Sample parsing of expression
3*2+a*2 -> 3 2 * a 2 * +
1+a*2+3-5 -> 1 a 2 * + 3 + 5 -
2- 9 +8*0 -> 2 9 - 8 0 * +
1-2*3-4+5*6*7+8 -> 1 2 3 * - 4 - 5 6 * 7 * + 8 +

**/

queue<string> Parser::getExpression() {
	stack<string> operationStack;
	queue<string> expressionQueue;
	//using Shunting-yard algorithm
	string word;
	while ((word = getWord()) != ";") {
		if (word == "+" || word == "-" || word == "*") {
			//if top of stack is *, all other operation (+-*) are lower or equal, so just add top to output queue
			//if top of stack is + or -, only add top to output queue if word is + or -
			while (!operationStack.empty() && !(operationStack.top() != "*" && word == "*")) {
				expressionQueue.push(operationStack.top());
				operationStack.pop();
			}
			operationStack.push(word);
		} else {
			expressionQueue.push(word);
		}	
	}
	while (!operationStack.empty()) {
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	return expressionQueue;
}


void Parser::parseWhile() {
	match("while");
	string conditionVar = getWord();
	ParsedData whileStmt = ParsedData(ParsedData::WHILE, nestingLevel);
	whileStmt.setWhileVar(conditionVar);
	parsedDataReceiver->processParsedData(whileStmt);
	match("{");
	stmtLst();
	match("}");
}

void Parser::endParse() {
	ParsedData endData = ParsedData(ParsedData::END, nestingLevel);
	parsedDataReceiver->processParsedData(endData);
}