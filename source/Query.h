#pragma once
#include <string>
//Query object to be passed between parser and evaluator
using std::string;

class Query
{
private:
	string selectSynonym;
public:
	Query(void);
	~Query(void);
};

