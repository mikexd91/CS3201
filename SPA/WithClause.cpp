#include "WithClause.h"
#include "Utils.h"

WithClause::WithClause(void)
{
}

WithClause::~WithClause(void)
{
}

//set
void WithClause::setLeftValue(string val){
	this->firstArg = val;
}

void WithClause::setLeftType(string type){
	this->firstArgType = type;
}

void WithClause::setLeftFixed(bool cond){
	this->firstArgFixed = cond;
}

void WithClause::setRightValue(string val){
	this->secondArg = val;
}

void WithClause::setRightType(string type){
	this->secondArgType = type;
}

void WithClause::setRightFixed(bool cond){
	this->secondArgFixed = cond;
}

//access
string WithClause::getLeftValue(){
	return this->firstArg;
}

string WithClause::getLeftType(){
	return this->firstArgType;
}

bool WithClause::getLeftFixed(){
	return this->firstArgFixed;
}

string WithClause::getRightValue(){
	return this->secondArg;
}

string WithClause::getRightType(){
	return this->secondArgType;
}

bool WithClause::getRightFixed(){
	return this->secondArgFixed;
}

bool WithClause::isValid(){
	//kiv
	return true;
}

//redundant methods, implement to match virtual
bool evaluateS1FixedS2Fixed(string s1, string s2){return false;}
bool evaluateS1GenericS2Generic(){return false;}
bool evaluateS1GenericS2Fixed(string s1){return false;}
bool evaluateS1FixedS2Generic(string s1){return false;}
unordered_set<string> getAllS2WithS1Fixed(string s1){return unordered_set<string>();}
unordered_set<string> getAllS2(){return unordered_set<string>();}
unordered_set<string> getAllS1WithS2Fixed(string s1){return unordered_set<string>();}
unordered_set<string> getAllS1(){return unordered_set<string>();}
Results::ResultsTable* getAllS1AndS2(){return new Results::ResultsTable;}