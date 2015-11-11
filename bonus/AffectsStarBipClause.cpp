#include "AffectsStarBipClause.h"
#include "AffectsStarBipCalculator.h"
#include "AffectsBipCalculator.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsStarBipClause::AffectsStarBipClause(void):SuchThatClause(AFFECTS_) {
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
}

AffectsStarBipClause::~AffectsStarBipClause(void){
	
}

//to add if statements
bool AffectsStarBipClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool AffectsStarBipClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	AffectsStarBipCalculator calc = AffectsStarBipCalculator();
	return calc.computeFixedFixed(firstArg, secondArg);
}

//e.g. Parent(_,_)
//Assuming that Affects(_,_) == Affects*(_,_)
bool AffectsStarBipClause::evaluateS1GenericS2Generic() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeS1GenericS2Generic();
}

//e.g. Parent(_,2)
bool AffectsStarBipClause::evaluateS1GenericS2Fixed(string s2) {
	unordered_set<Statement*> assgStmts = stmtTable->getAssgStmts();
	BOOST_FOREACH(Statement* assgStmt, assgStmts) {
		AffectsStarBipCalculator calc = AffectsStarBipCalculator();
		if (calc.computeFixedFixed(boost::lexical_cast<string>(assgStmt->getStmtNum()), s2)) {
			return true;
		}
	}
	return false;
}

//e.g. Parent(2,_)
//Assuming that Affects(1,_) == Affects*(1,_)
bool AffectsStarBipClause::evaluateS1FixedS2Generic(string s1){
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeS1FixedS2Generic(s1);
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsStarBipClause::getAllS2WithS1Fixed(string s1) {
	AffectsStarBipCalculator calc = AffectsStarBipCalculator();
	return calc.computeFixedSyn(s1);
}

//e.g. Parent(_, s2)
//get all children
//Assuming that Affects(_,s2) == Affects*(_,s2)
unordered_set<string> AffectsStarBipClause::getAllS2() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeAllS2();
}

//e.g. Parent(s1,2)
//get parent of string
unordered_set<string> AffectsStarBipClause::getAllS1WithS2Fixed(string s2) {
	AffectsStarBipCalculator calc = AffectsStarBipCalculator();
	unordered_set<vector<string>> s1s2 = calc.computeSynSyn(false);
	unordered_set<string> result = unordered_set<string>();
	BOOST_FOREACH(vector<string> pair, s1s2) {
		if (pair.at(1) == s2) {
			result.insert(pair.at(0));
		}
	}
	return result;
}

//e.g. Parent(s1,_)
//Assuming that Affects(s1,_) == Affects*(s1,_)
unordered_set<string> AffectsStarBipClause::getAllS1() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeAllS1();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsStarBipClause::getAllS1AndS2() {
	AffectsStarBipCalculator calc = AffectsStarBipCalculator();
	return calc.computeSynSyn(firstArg == secondArg);
}




