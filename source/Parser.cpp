#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "Parser.h"

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
		//throw error. token should be present but is not
		
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
	//send procedure object to pdr
	match("{");
	stmtLst();
	match("}");
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
	string name = getWord();
	match("=");
	string expression = getWord();
	//send assign object to pdr
}







