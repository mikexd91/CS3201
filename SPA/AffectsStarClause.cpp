#include "AffectsStarClause.h"
#include "AffectsStarCalculator.h"
#include "CFG.h"
#include "AssgGNode.h"
#include "DummyGNode.h"
#include "IfGNode.h"
#include "WhileGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsStarClause::AffectsStarClause(void):SuchThatClause(AFFECTS_) {
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
}

AffectsStarClause::~AffectsStarClause(void){
	
}

//to add if statements
bool AffectsStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool AffectsStarClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	AffectsStarCalculator calc = AffectsStarCalculator();
	return calc.computeFixedFixed(firstArg, secondArg);
}

//e.g. Parent(_,_)
bool AffectsStarClause::evaluateS1GenericS2Generic() {
	return false;
}

//e.g. Parent(_,2)
bool AffectsStarClause::evaluateS1GenericS2Fixed(string s2) {
	return false;
}

//e.g. Parent(2,_)
bool AffectsStarClause::evaluateS1FixedS2Generic(string s1){
	return false;
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsStarClause::getAllS2WithS1Fixed(string s1) {
	return unordered_set<string>();
}

//e.g. Parent(_, s2)
//get all children
unordered_set<string> AffectsStarClause::getAllS2() {
	return unordered_set<string>();
}

//e.g. Parent(s1,2)
//get parent of string
unordered_set<string> AffectsStarClause::getAllS1WithS2Fixed(string s2) {
	return unordered_set<string>();
}

//e.g. Parent(s1,_)
//get all parents
unordered_set<string> AffectsStarClause::getAllS1() {
	return unordered_set<string>();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsStarClause::getAllS1AndS2() {
	return unordered_set<vector<string>>();
}




