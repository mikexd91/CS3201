#include "Utils.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

Utils::Utils(void){
}

Utils::~Utils(void){
}

string Utils::sanitise(string str) {
	for (size_t i = 0; i < strlen(UtilsConstants::SANITISED_CHARS); i++) {
		char sanitisedChar = UtilsConstants::SANITISED_CHARS[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

//Split code into a vector of tokens
vector<string> Utils::explode(const string &str) {
	vector<string> elems;
	int pos;
	int prev = 0;
	while ((pos = str.find_first_of(UtilsConstants::DELIM_STRING, prev)) != string::npos) {
		//if there are words in between (not consecutive delimiters)
		if (pos > prev) {
			elems.push_back(str.substr(prev, pos - prev));
		}
		//if it is a delimiter that should be included (aka all delimiters but spaces)
		if (find(begin(UtilsConstants::DELIMITERS), end(UtilsConstants::DELIMITERS), str[pos]) != end(UtilsConstants::DELIMITERS)) {
			elems.push_back(string(1, str[pos]));
		}
		prev = pos + 1;
	}
	return elems;
}