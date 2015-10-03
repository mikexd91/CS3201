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

void StringPair::setAttribute(string s){
	this->attribute = s;
}

string StringPair::getFirst(void){
	return this->first;
}

string StringPair::getSecond(void){
	return this->second;
}

string StringPair::getAttribute(void){
	return this->attribute;
}