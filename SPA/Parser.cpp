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
	validateCallStmts();
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
	if (find(existingProcedures.begin(), existingProcedures.end(), procName) != existingProcedures.end()) {
			//same procedure is defined twice
			throwException(stmtCount);
	}
	currentProcName = procName;
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, this->nestingLevel);
	procedure.setProcName(procName);
	parsedDataReceiver->processParsedData(procedure);
	existingProcedures.push_back(procName);
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
	} else if (nextToken == "if") {
		parseIfBlock();
	} else if (nextToken == "call") {
		call();
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

void Parser::call() {
	match("call");
	string procName = getName();
	//check that the procedure does not call itself
	if (currentProcName == procName) {
		throwException(stmtCount);
	}
	ParsedData callStmt = ParsedData(ParsedData::CALL, this->nestingLevel);
	callStmt.setProcName(procName);
	parsedDataReceiver->processParsedData(callStmt);
	calledProcedures.push_back(procName);
	match(";");
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

void Parser::parseIfBlock() {
	parseIf();
	parseThen();
	parseElse();
}

void Parser::parseIf(){
	match("if");
	string conditionVar = getName();
	ParsedData ifStmt = ParsedData(ParsedData::IF, nestingLevel);
	ifStmt.setIfVar(conditionVar);
	parsedDataReceiver->processParsedData(ifStmt);
}

void Parser::parseThen(){
	match ("then");
	match("{");
	stmtLst();
	match("}");
}

void Parser::parseElse() {
	match ("else");
	ParsedData elseStmt = ParsedData(ParsedData::ELSE, nestingLevel);
	parsedDataReceiver->processParsedData(elseStmt);
	match ("{");
	stmtLst();
	match("}");
}

//checked that all the called procedures exist as procedures
void Parser::validateCallStmts() {
	for (vector<string>::iterator iter = calledProcedures.begin(); iter != calledProcedures.end(); iter++) {
		string calledProcedure = *iter;
		if (find(existingProcedures.begin(), existingProcedures.end(), calledProcedure) == existingProcedures.end()) {
			throwException(stmtCount);
		}
	}
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