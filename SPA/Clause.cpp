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

void Clause::setClauseType(string newType){
	this->clauseType = newType;
}

void Clause::setStar(bool star){
	this->isStar = star;
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

void Clause::setPatternArg(string pattern){
	this->patternArg = pattern;
}

string Clause::getClauseType(void){
	return this->clauseType;
}

bool Clause::getStar(void){
	return this->isStar;
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

bool Clause::getFirstArgFixed(void){
	return this->firstArgFixed;
}

bool Clause::getSecondArgFixed(void){
	return this->secondArgFixed;
}

bool Clause::checkClauseValidity(map<string, string> declarations){
	string type = this->getClauseType();
	string argOne = this->getFirstArg();
	string argTwo = this->getSecondArg();
	if (declarations.count(argOne) != 0){
		//if (!argOneLiteral){
			throw MissingDeclarationException();
			return false;
		//}
	}
	if (declarations.count(argTwo) != 0){
		throw MissingDeclarationException();
		return false;
	}
	string argOneType = declarations.at(argOne);
	string argTwoType = declarations.at(argTwo);
	if (type.compare(stringconst::TYPE_FOLLOWS)){
		//INSERT RULES HERE
		
	} else if (type.compare(stringconst::TYPE_PARENT)){
		//INSERT RULES HERE
		
	} else if (type.compare(stringconst::TYPE_MODIFIES)){
		//INSERT RULES HERE
		
	} else if (type.compare(stringconst::TYPE_PATTERN)){
		//INSERT RULES HERE
		
	} else if (type.compare(stringconst::TYPE_USES)){
		//INSERT RULES HERE
		
	}
	return true;
}
