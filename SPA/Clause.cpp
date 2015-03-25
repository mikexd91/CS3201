#include "Clause.h"
#include <string>
using std::string;
Clause::Clause(void){
}

Clause::~Clause(void){
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

void Clause::setPatternArg(string pattern){
	this->patternArg = pattern;
}

string Clause::getClauseType(void){
	return this->clauseType;
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

string Clause::getPatternArg(void){
	return this->patternArg;
}

bool Clause::checkClauseValidity(void){
	string type = this->getClauseType();
	string argTypeOne = this->getFirstArgType();
	string argTypeTwo = this->getSecondArgType();
	if (type.compare(stringconst::TYPE_FOLLOWS)){
		//INSERT RULES HERE
		return true;
	} else if (type.compare(stringconst::TYPE_PARENT)){
		//INSERT RULES HERE
		return true;
	} else if (type.compare(stringconst::TYPE_MODIFIES)){
		//INSERT RULES HERE
		return true;
	} else if (type.compare(stringconst::TYPE_PATTERN)){
		//INSERT RULES HERE
		return true;
	} else if (type.compare(stringconst::TYPE_USES)){
		//INSERT RULES HERE
		return true;
	}
	return false;
}
