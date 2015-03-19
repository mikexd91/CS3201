#include "ParsedData.h"
using namespace std;

ParsedData::ParsedData(Type dataType, int levelNum)
{
	type = dataType;
	nestingLevel = levelNum;

	//Assignment
	string assignVar = NULL;
	int assignExpression = NULL; //an integer for now

	//Procedure
	string procName = NULL;
}


void ParsedData::setAssignVar(string name) {
	assignVar = name;
};

void ParsedData::setAssignExpression(int expression) {
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

int ParsedData::getAssignExpression() {
	return assignExpression;
};

string ParsedData::getProcName() {
	return procName;
};

ParsedData::Type ParsedData::getType() {
	return type;
};


