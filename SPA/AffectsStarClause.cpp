#include "AffectsStarClause.h"
#include "AffectsStarCalculator.h"
#include "AffectsCalculator.h"
#include "../SPA/DummyGNode.h"
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
	
	// get the gnode of this stmt
	GNode* gn = stmt->getGNodeRef();

	BOOST_FOREACH(string var, usesSet) {
		if (modcheck(var, gn, new unordered_set<int>(), stmtNum)) {
			return true;
		}
	}

	return false;

	////cout << "using " << usesSet.size() << endl;
	////BOOST_FOREACH(auto u, usesSet) {
	////	cout << u << endl;
	////}

	//// get the containing procedure
	//Procedure* containingProc = stmt->getProc();
	//// get all the statements in the proc
	//unordered_set<int> procStmts = containingProc->getContainStmts();
	//// check every pair for affects*(pg, s2)
	//BOOST_FOREACH(auto pg, procStmts) {
	//	string pgstr = lexical_cast<string>(pg);
	//	//cout << "checking " << pgstr << " " << s2 << endl;
	//	if (evaluateS1FixedS2Fixed(pgstr, s2)) {
	//		return true;
	//	}
	//}
	//return false;
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

bool AffectsStarClause::modcheck(string var, GNode* gn, unordered_set<int>* visitedSet) {
	//modcheck(v, gn) {
	//	if gn.type == proc or prog or end
	//		return false
	//	
	//	elif gn.type == assg
	//		for each stmt# in gn, 
	//			if gn.mod(v) == true
	//				return true
	//	
	//	elif gn.type == calls or if
	//		return modcheck(v, gn.parent)
	//	
	//	elif gn.type == while
	//		return modcheck(v, gn.parent1, gn.parent2)
	//	
	//	elif gn.type == dummy
	//		return modcheck(v, gn.parent1, gn.parent2)

	int dgn_id;
	DummyGNode* dgn;
	switch (gn->getNodeType()) {
		case PROC_ :
		case END_ :
			//cout << "end" << endl;
			return false;

		case CALL_ :
			//cout << "call" << endl;
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return false;
			}
			visitedSet->insert(gn->getStartStmt());
			// need to check if it mods the var
			// if it does, then we cannot go up
			if (stmtTable->getStmtObj(gn->getStartStmt())->getModifies().count(var) >= 1) {
				return false;
			} else {
				return modcheck(var, gn->getParents().at(0), visitedSet);
			}

		case IF_ :
			//cout << "if" << endl;
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return false;
			}
			visitedSet->insert(gn->getStartStmt());
			return modcheck(var, gn->getParents().at(0), visitedSet);

		case WHILE_ :
			//cout << "while" << endl;
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return false;
			}
			visitedSet->insert(gn->getStartStmt());
			return modcheck(var, gn->getParents().at(0), visitedSet) 
				|| modcheck(var, gn->getParents().at(1), visitedSet);
			
		case DUMMY_ :
			//cout << "dummy" << endl;
			dgn = (DummyGNode*)gn;
			dgn_id = dgn->getEntrance()->getStartStmt() + 900000;
			if (visitedSet->count(dgn_id) >= 1) {
				return false;
			}
			visitedSet->insert(dgn_id);
			return modcheck(var, gn->getParents().at(0), visitedSet) 
				|| modcheck(var, gn->getParents().at(1), visitedSet);
			
		case ASSIGN_ :
			//cout << "assign" << endl;
			return modcheck(var, gn, visitedSet, gn->getEndStmt());

		default :
			//cout << "unknown node type" << endl;
			return false;
	}
}

bool AffectsStarClause::modcheck(string var, GNode* gn, unordered_set<int>* visitedSet, int stmtNum) {
	//cout << "modcheck with stmtnum = " << stmtNum << endl;

	if (gn->getNodeType() == ASSIGN_) {
		//cout << "end stmt = " << gn->getStartStmt() << endl;
		for (int i = stmtNum; i >= gn->getStartStmt(); i--) {
			if (visitedSet->count(i) >= 1) {
				// visited this stmt before
				// above shud be visited before also
				return false;
			}
			visitedSet->insert(i);
			//cout << i << endl;
			Statement* stmt = stmtTable->getStmtObj(i);
			unordered_set<string> modSet = stmt->getModifies();
			if (modSet.size() == 1) {
				// it must modify something
				// get that something
				string modVar = *(modSet.begin());
				if (modVar == var) {
					// if it modifies then ok
					return true;
				}
			}
		}
		return modcheck(var, gn->getParents().at(0), visitedSet);
	}
	return false;
}
