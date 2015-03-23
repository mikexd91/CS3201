#include "Clause.h"
#include <string>

using std::string;
Clause::Clause(void)
{
}

Clause::~Clause(void)
{
}

void Clause::setClauseType(string newType){
	this->clauseType = newType;
}

void Clause::setFirstArg(string first){
	this->firstArg = first;
}

void Clause::setFirstArgType(string type){
	this->firstArgType = type;
}

void Clause::setSecondArg(string second){
	this->secondArg = second;
}

void Clause::setSecondArgType(string type){
	this->secondArgType = type;
}

string Clause::getClauseType(){
	return this->clauseType;
}

string Clause::getFirstArg(){
	return this->firstArg;
}

string Clause::getFirstArgType(){
	return this->firstArgType;
}

string Clause::getSecondArg(){
	return this->secondArg;
}

string Clause::getSecondArgType(){
	return this->secondArgType;
}