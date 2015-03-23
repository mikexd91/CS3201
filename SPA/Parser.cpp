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

//e.g. string content = "Procedure test { a = 2; a = 5;} ";
void Parser::parse(string content) {
	tokens = explode(content);
	iter = tokens.begin();
	program();
}

//Split code into a vector of tokens
vector<string> Parser::explode(const string &s) {
	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = s.find_first_of(" ;", prev)) != string::npos) {
		if (pos > prev) {
			elems.push_back(s.substr(prev, pos- prev));
			if (s[pos] == ';') {
				elems.push_back(";");
			}
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
	match("Procedure");
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
	assignment.setAssignExpression(atoi(expression.c_str()));
	parsedDataReceiver.processParsedData(assignment);
}







