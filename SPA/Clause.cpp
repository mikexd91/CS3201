#include "Clause.h"
#include "PQLExceptions.h"
#include <string>
#include <map>

using std::map;
using std::string;
Clause::Clause(void){
}

Clause::~Clause(void){
}

void Clause::setFirstArg(string first){
	this->firstArg = first;
}

void Clause::setFirstArgType(string type){
	this->firstArgType = type;
}

void Clause::setFirstArgFixed(bool fixed){
	this->firstArgFixed = fixed;
}

void Clause::setSecondArg(string second){
	this->secondArg = second;
}

void Clause::setSecondArgType(string type){
	this->secondArgType = type;
}

void Clause::setSecondArgFixed(bool fixed){
	this->secondArgFixed = fixed;
}

string Clause::getFirstArg(void){
	return this->firstArg;
}

string Clause::getFirstArgType(void){
	return this->firstArgType;
}

string Clause::getSecondArg(void){
	return this->secondArg;
}

string Clause::getSecondArgType(void){
	return this->secondArgType;
}

bool Clause::getFirstArgFixed(void){
	return this->firstArgFixed;
}

bool Clause::getSecondArgFixed(void){
	return this->secondArgFixed;
}
