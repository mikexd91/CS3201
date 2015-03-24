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
}

//e.g. string content = "procedure test { a = 2; a = 5;} ";
void Parser::parse(string content) {
	content = sanitise(content);
	tokens = explode(content);
	iter = tokens.begin();
	program();
}

string Parser::sanitise(string str) {
	for (int i = 0; i < strlen(constants::SANITISED_CHARS); i++) {
		char sanitisedChar = constants::SANITISED_CHARS[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

//Split code into a vector of tokens
vector<string> Parser::explode(const string &str) {
	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = str.find_first_of(constants::DELIM_STRING, prev)) != string::npos) {
		//if there are words in between (not consecutive delimiters)
		if (pos > prev) {
			elems.push_back(str.substr(prev, pos - prev));
		}
		//if it is a delimiter that should be included (aka all delimiters but spaces)
		if (find(begin(constants::DELIMITERS), end(constants::DELIMITERS), str[pos]) != end(constants::DELIMITERS)) {
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
		//end of file
		return;
	}
}

string Parser::getWord() {
	string result = nextToken;
	getNextToken();
	return result;
}

void Parser::program() {
	getNextToken();
	procedure();
}

void Parser::procedure() {
	match("procedure");
	string procName = getWord();
	ParsedData procedure = ParsedData(ParsedData::PROCEDURE, nestingLevel);
	procedure.setProcName(procName);
	parsedDataReceiver.processParsedData(procedure);
	match("{");
	nestingLevel++;
	stmtLst();
	match("}");
	nestingLevel--;
}

void Parser::stmtLst() {
	while (nextToken != "}") {
		stmt();		
	}
}

void Parser::stmt() {
	//only assign statements now
	assign();
	match(";");
}

void Parser::assign() {
	string var = getWord();
	match("=");
	string expression = getWord();
	ParsedData assignment = ParsedData(ParsedData::ASSIGNMENT, nestingLevel);
	assignment.setAssignVar(var);
	assignment.setAssignExpression(expression.c_str());
	parsedDataReceiver.processParsedData(assignment);
}