#pragma once
#include <string>

using std::string;
class QueryParser
{
private:
	string INVALID_QUERY;
	string VALID_QUERY;
public:
	QueryParser(void);
	~QueryParser(void);
};

