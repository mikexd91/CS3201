#pragma once
#include <string>

using std::string;

class StringPair
{
private:
	string first;
	string second;
	string attribute;
public:
	StringPair(void);
	~StringPair(void);
	void setFirst(string);
	void setSecond(string);
	void setAttribute(string);
	string getFirst(void);
	string getSecond(void);
	string getAttribute(void);
};

