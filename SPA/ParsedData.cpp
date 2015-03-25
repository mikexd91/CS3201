#include "ParsedData.h"
using namespace std;

ParsedData::ParsedData(Type dataType, int levelNum)
{
	type = dataType;
	nestingLevel = levelNum;

	//Assignment
	string assignVar = NULL;
	string assignExpression = NULL; 

	//Procedure
	string procName = NULL;
}


void ParsedData::setAssignVar(string name) {
	assignVar = name;
};

void ParsedData::setAssignExpression(string expression) {
	assignExpression = expression;
}

void ParsedData::setProcName(string name) {
	procName = name;
}

int ParsedData::getNestingLevel() {
	return nestingLevel;
};

string ParsedData::getAssignVar() {
	return assignVar;
};

string ParsedData::getAssignExpression() {
	return assignExpression;
};

string ParsedData::getProcName() {
	return procName;
};

ParsedData::Type ParsedData::getType() {
	return type;
};


