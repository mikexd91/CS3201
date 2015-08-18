#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "InvalidCodeException.h"
#include "Parser.h"
#include "ParsedData.h"
#include "Utils.h"
#include "InvalidExpressionException.h"
#include "ExpressionParser.h"

using namespace std;

//Constructor

Parser::Parser() {
	nestingLevel = 0;
	parsedDataReceiver = PDR::getInstance();
	stmtCount = 0;
}

//e.g. string content = "procedure test { a = 2; a = 5;} ";
void Parser::parse(string content) {
	content = Utils::sanitise(content);
	tokens = Utils::explode(content, ParserConstants::DELIM_STRING, ParserConstants::DELIMITERS);
	iter = tokens.begin();
	program();
	endParse();
}

//checks if the next token matches the expected token. if it matches, continue
void Parser::match(string token) {
	if (nextToken == token) {
		//Token is present.
		getNextToken();
	}
	else {
		throwException(stmtCount);
		//throw InvalidCodeException(stmtCount);
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

string Parser::getName() {
	string result = getWord();
	if (Utils::isValidName(result)) {
		return result;
	} else {
		throwException(stmtCount);
		return "";
	}
}

/**
string Parser::getFactor() {
	string result = getWord();
	if (isValidName(result) || isValidConstant(result)) {
		return result;
	} else {
		throw InvalidCodeException(stmtCount);
	}
}
**/

void Parser::program() {
	getNextToken();
	while (!nextToken.empty()) {
		procedure();
	}
}

void Parser::procedure() {
	match("procedure");
	string procName = getName();
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, this->nestingLevel);
	procedure.setProcName(procName);
	parsedDataReceiver->processParsedData(procedure);
	match("{");
	stmtLst();
	match("}");
}

void Parser::stmtLst() {
	nestingLevel++;
	while (nextToken != "}") {
		if (nextToken.empty()) {
			throwException(stmtCount);
		} else {
			stmt();
		}
	}
	nestingLevel--;
}

void Parser::stmt() {
	stmtCount++;
	if (nextToken == "while") {
		parseWhile();
	} else {
		assign(); 
	}
}

void Parser::assign() {
	string var = getName();
	match("=");
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
	queue<string> originalExpression;
	string word;
	while ((word = getWord()) != ";") {
		if (Utils::isValidFactor(word) || Utils::isValidOperator(word) || word == "(" || word == ")") {
			originalExpression.push(word);
		} else {
			throwException(stmtCount);
		}
	}
	try {
		ExpressionParser expressionParser;
		return expressionParser.getRPN(originalExpression);
	} catch (InvalidExpressionException) {
		throwException(stmtCount);
	}
}

void Parser::parseWhile() {
	match("while");
	string conditionVar = getName();
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

void Parser::throwException(int lineNumber) {
	throw InvalidCodeException(generateErrorMessage(lineNumber));

}

string Parser::generateErrorMessage(int lineNumber) {
	ostringstream output;
	output.str("");
	output << ParserConstants::ERROR_MESSAGE << lineNumber;
	return output.str();
}