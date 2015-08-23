#pragma once
#include <string>
#include <queue>
using namespace std;
class ParsedData	
{
public:
	enum Type {ASSIGNMENT, PROCEDURE, WHILE, IF, CALL, END, ELSE};

	//Constructor
	ParsedData(enum Type, int);
	
	//Setter
	void setAssignVar(string);
	void setAssignExpression(queue<string>);
	void setProcName(string);
	void setWhileVar(string name);
	void setIfVar(string name);

	//Getter
	int getNestingLevel();
	string getAssignVar();
	queue<string> getAssignExpression();
	string getProcName();
	Type getType();
	string getWhileVar();
	string getIfVar();


private:
	Type type;
	int nestingLevel;
	//Assignment
	string assignVar;
	queue<string> assignExpression; //in Reverse Polish notation
	//Procedure/calls
	string procName;
	//While
	string whileVar;
	//If
	string ifVar;
};
