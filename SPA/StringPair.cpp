#include "StringPair.h"
#include <string>

StringPair::StringPair(void){
}

StringPair::~StringPair(void){
}

void StringPair::setFirst(string s){
	this->first = s;
}

void StringPair::setSecond(string s){
	this->second = s;
}

string StringPair::getFirst(void){
	return this->first;
}

string StringPair::getSecond(void){
	return this->second;
}