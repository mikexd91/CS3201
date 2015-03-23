#pragma once
#include <string>
using std::string;
class Clause
{
private:
	string clauseType;
	string firstArg;
	string firstArgType;
	string secondArg;
	string secondArgType;
public:
	Clause(void);
	~Clause(void);

	void setClauseType(string);
	void setFirstArg(string);
	void setSecondArg(string);
	void setFirstArgType(string);
	void setSecondArgType(string);

	string getClauseType();
	string getFirstArg();
	string getSecondArg();
	string getFirstArgType();
	string getSecondArgType();
};

