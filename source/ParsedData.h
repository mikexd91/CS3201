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
	void setAssignExpression(int);
	void setProcName(string);

	//Getter
	int getNestingLevel();
	string getAssignVar();
	int getAssignExpression();
	string getProcName();
	Type getType();


private:
	Type type;
	int nestingLevel;
	//Assignment
	string assignVar;
	int assignExpression; //an integer for now
	//Procedure
	string procName;
	
	

};

