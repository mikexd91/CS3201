#include "AffectsStarClause.h"
#include "AffectsStarCalculator.h"
#include "AffectsCalculator.h"
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
//Assuming that Affects(_,_) == Affects*(_,_)
bool AffectsStarClause::evaluateS1GenericS2Generic() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeS1GenericS2Generic();
}

//e.g. Parent(2,_)
//Assuming that Affects(1,_) == Affects*(1,_)
bool AffectsStarClause::evaluateS1FixedS2Generic(string s1){
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeS1FixedS2Generic(s1);
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsStarClause::getAllS2WithS1Fixed(string s1) {
	AffectsStarCalculator calc = AffectsStarCalculator();
	return calc.computeFixedSyn(s1);
}

//e.g. Parent(_, s2)
//get all children
//Assuming that Affects(_,s2) == Affects*(_,s2)
unordered_set<string> AffectsStarClause::getAllS2() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeAllS2();
}

//e.g. Parent(s1,_)
//Assuming that Affects(s1,_) == Affects*(s1,_)
unordered_set<string> AffectsStarClause::getAllS1() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeAllS1();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsStarClause::getAllS1AndS2() {
	AffectsStarCalculator calc = AffectsStarCalculator();
	return calc.computeSynSyn(firstArg == secondArg);
}




// nick
//e.g. Parent(_,2)
bool AffectsStarClause::evaluateS1GenericS2Fixed(string s2) {
	// get the statement object and make sure it is an assign stmt
	int stmtNum = lexical_cast<int>(s2);
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	if (stmt->getType() != ASSIGN_STMT_) {
		//cout << "not assg" << endl;
		return false;
	}

	// get the uses set
	unordered_set<string> usesSet = stmt->getUses();
	if (usesSet.size() <= 0) {
		//if the assignment doesnt use anything, then nothing affects it
		//cout << "not using" << endl;
		return false;
	}

	//cout << "using " << usesSet.size() << endl;
	//BOOST_FOREACH(auto u, usesSet) {
	//	cout << u << endl;
	//}

	// get the containing procedure
	Procedure* containingProc = stmt->getProc();
	// get all the statements in the proc
	unordered_set<int> procStmts = containingProc->getContainStmts();
	// check every pair for affects*(pg, s2)
	BOOST_FOREACH(auto pg, procStmts) {
		string pgstr = lexical_cast<string>(pg);
		//cout << "checking " << pgstr << " " << s2 << endl;
		if (evaluateS1FixedS2Fixed(pgstr, s2)) {
			return true;
		}
	}
	return false;
}

// nick
//e.g. Parent(s1,2)
//get parent of string
unordered_set<string> AffectsStarClause::getAllS1WithS2Fixed(string s2) {
	// prepare result obj
	unordered_set<string> result;
	
	// get the statement object and make sure it is an assign stmt
	int stmtNum = lexical_cast<int>(s2);
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	if (stmt->getType() != ASSIGN_STMT_) {
		//cout << "not assg" << endl;
		return result;
	}

	// get the uses set
	unordered_set<string> usesSet = stmt->getUses();
	if (usesSet.size() <= 0) {
		//if the assignment doesnt use anything, then nothing affects it
		//cout << "not using" << endl;
		return result;
	}

	//cout << "using " << usesSet.size() << endl;

	// get the containing procedure
	Procedure* containingProc = stmt->getProc();
	// get all the statements in the proc
	unordered_set<int> procStmts = containingProc->getContainStmts();
	// check every pair for affects(pg, s2)
	BOOST_FOREACH(auto pg, procStmts) {
		string pgstr = lexical_cast<string>(pg);
		//cout << "checking " << pgstr << " " << s2 << endl;
		if (evaluateS1FixedS2Fixed(pgstr, s2)) {
			// add it to the result
			result.insert(pgstr);
		}
	}

	// return whatever we have placed inside the result set.
	return result;
}
