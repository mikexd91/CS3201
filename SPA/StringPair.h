#pragma once
#include <string>

using std::string;

class StringPair
{
private:
	string first;
	string second;
public:
	StringPair(void);
	~StringPair(void);
	void setFirst(string);
	void setSecond(string);
	string getFirst(void);
	string getSecond(void);
};

