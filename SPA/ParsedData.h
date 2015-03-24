#pragma once
#include <string>
using namespace std;
class ParsedData	
{
public:
	enum Type {ASSIGNMENT, PROCEDURE};

	//Constructor
	ParsedData(enum Type, int);
	
	//Setter
	void setAssignVar(string);
	void setAssignExpression(string);
	void setProcName(string);

	//Getter
	int getNestingLevel();
	string getAssignVar();
	string getAssignExpression();
	string getProcName();
	Type getType();


private:
	Type type;
	int nestingLevel;
	//Assignment
	string assignVar;
	string assignExpression; //an integer for now
	//Procedure
	string procName;
	
	

};

