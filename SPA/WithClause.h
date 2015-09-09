#pragma once

#include "Clause.h"
#include "Results.h"
#include "boost\unordered_set.hpp"

using std::string;
using boost::unordered_set;

class WithClause : public Clause
{
public:
	WithClause(void);
	~WithClause(void);

	bool isValid(void);

	//access
	string getLeftValue(void);
	string getLeftType(void);
	bool getLeftFixed(void);
	
	string getRightValue(void);
	string getRightType(void);
	bool getRightFixed(void);

	//set
	void setLeftValue(string);
	void setLeftType(string);
	void setLeftFixed(bool);

	void setRightValue(string);
	void setRightType(string);
	void setRightFixed(bool);

protected:
	//redundant for with
	bool evaluateS1FixedS2Fixed(string, string);
	bool evaluateS1GenericS2Generic(void);
	bool evaluateS1GenericS2Fixed(string);
	bool evaluateS1FixedS2Generic(string);
	unordered_set<string> getAllS2WithS1Fixed(string);
	unordered_set<string> getAllS2(void);
	unordered_set<string> getAllS1WithS2Fixed(string);
	unordered_set<string> getAllS1(void);
	Results::ResultsTable* getAllS1AndS2(void);
};

