#include "ParsedData.h"
using namespace std;

ParsedData::ParsedData(Type dataType, int levelNum)
{
	type = dataType;
	nestingLevel = levelNum;

	//Assignment
	string assignVar;
	queue<string> assignExpression; 

	//Procedure
	string procName;

	//While
	string whileVar;
}


void ParsedData::setAssignVar(string name) {
	assignVar = name;
};

void ParsedData::setAssignExpression(queue<string> expression) {
	assignExpression = expression;
}

void ParsedData::setProcName(string name) {
	procName = name;
}

void ParsedData::setWhileVar(string name) {
	whileVar = name;
};

int ParsedData::getNestingLevel() {
	return nestingLevel;
};

string ParsedData::getAssignVar() {
	return assignVar;
};

queue<string> ParsedData::getAssignExpression() {
	return assignExpression;
};

string ParsedData::getProcName() {
	return procName;
};

ParsedData::Type ParsedData::getType() {
	return type;
};

string ParsedData::getWhileVar() {
	return whileVar;
};


