#include "AffectsClause.h"
#include "AffectsCalculator.h"
#include "CFG.h"
#include "AssgGNode.h"
#include "DummyGNode.h"
#include "IfGNode.h"
#include "WhileGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsClause::AffectsClause(void):SuchThatClause(AFFECTS_) {
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
}

AffectsClause::~AffectsClause(void){
	
}

//to add if statements
bool AffectsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool AffectsClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeFixedFixed(firstArg, secondArg);
}

//e.g. Parent(_,_)
bool AffectsClause::evaluateS1GenericS2Generic() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeS1GenericS2Generic();
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsClause::getAllS2WithS1Fixed(string s1) {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeFixedSyn(s1);
}

//e.g. Parent(_, s2)
//get all children
unordered_set<string> AffectsClause::getAllS2() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeAllS2();
}

//e.g. Parent(2,_)
bool AffectsClause::evaluateS1FixedS2Generic(string s1){
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeS1FixedS2Generic(s1);
}

//e.g. Affects(_, 2)
// nick - going upwards recursively to check if theres 
// something that mods the var used at the stmt
bool AffectsClause::evaluateS1GenericS2Fixed(string s2) {
	// get the statement object and make sure it is an assign stmt
	int stmtNum = lexical_cast<int>(s2);
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	if (stmt->getType() != ASSIGN_STMT_) {
		return false;
	}

	// get the uses set
	unordered_set<string> usesSet = stmt->getUses();
	if (usesSet.size() <= 0) {
		//if the assignment doesnt use anything, then nothing affects it
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

	//// get the containing procedure
	//Procedure* containingProc = stmt1->getProc();
	//// get all the statements in the proc
	//unordered_set<int> procStmts = containingProc->getContainStmts();
	//// check every pair for affects(pg, s2)
	//BOOST_FOREACH(auto pg, procStmts) {
	//	string pgstr = lexical_cast<string>(pg);
	//	////cout << "checking " << pgstr << " " << s2 << endl;
	//	if (evaluateS1FixedS2Fixed(pgstr, s2)) {
	//		return true;
	//	}
	//}
	//return false;
}

//e.g. Affects(s,2)
// nick - going upwards recursively to add all the 
// somethings that mods the var used at the stmt
unordered_set<string> AffectsClause::getAllS1WithS2Fixed(string s2) {
	// prepare result obj
	unordered_set<string> result;
	
	int stmtNum = lexical_cast<int>(s2);

	// first stmt cannot be affected and any stmt num below 1 is wrong.
	if (stmtNum <= 1) {
		return result;
	}
	
	// get the statement object and make sure it is an assign stmt
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	if (stmt->getType() != ASSIGN_STMT_) {
		return result;
	}

	// now look at the previous stmt
	int prevStmtNum = stmtNum - 1;
	Statement* prevStmt = stmtTable->getStmtObj(prevStmtNum);
	
	// get the uses set of the first stmt
	unordered_set<string> usesSet = stmt->getUses();
	if (usesSet.size() <= 0) {
		//if the assignment doesnt use anything, then nothing affects it
		return result;
	}

	//sigh
	// start from pervious node only if the previous stmt is not within the same assg node

	// get the current gnode
	GNode* gn = stmt->getGNodeRef();
	// get the start stmt
	int gnStartStmtNum = gn->getStartStmt();
	
	if (gnStartStmtNum <= prevStmtNum) {
		// if start stmt is less than or equal to the prev stmt then it is in the same assg node, 
		// so do modaddassg from this node and prev stmt num

		//cout << "start from this NODE at prev stmt num" << endl;

		BOOST_FOREACH(string var, usesSet) {
			//cout << "using " << var << endl;
			unordered_set<int> intResults;// = new unordered_set<int>();
			modadd(var, gn, &intResults, new unordered_set<int>(), prevStmtNum);
			//cout << "done with " << var << endl;
			//cout << intResults.size() << endl;
			BOOST_FOREACH(int r, intResults) {
				//cout << r << endl;
				string rs = to_string((long long) r);
				result.insert(rs);
			}
			//cout << "dont" << endl;
		}

	} else {
		// else the prev stmt is not in the same assg node, 
		// so do modaddnonassg from previous node

		//cout << "start from the previous NODE instead" << endl;

		// get the gnode of the prev stmt
		GNode* pgn = gn->getParents().at(0);

		BOOST_FOREACH(string var, usesSet) {
			//cout << "using " << var << endl;
			unordered_set<int> intResults;// = new unordered_set<int>();
			modadd(var, pgn, &intResults, new unordered_set<int>());
			//cout << "done with " << var << endl;
			//cout << intResults.size() << endl;
			BOOST_FOREACH(int r, intResults) {
				//cout << r << endl;
				string rs = to_string((long long) r);
				result.insert(rs);
			}
			//cout << "dont" << endl;
		}
	}

	//cout << "done" << endl;
	//// get the containing procedure
	//Procedure* containingProc = stmt->getProc();
	//// get all the statements in the proc
	//unordered_set<int> procStmts = containingProc->getContainStmts();
	//// check every pair for affects(pg, s2)
	//BOOST_FOREACH(auto pg, procStmts) {
	//	string pgstr = lexical_cast<string>(pg);
	//	////cout << "checking " << pgstr << " " << s2 << endl;
	//	if (evaluateS1FixedS2Fixed(pgstr, s2)) {
	//		// add it to the result
	//		result.insert(pgstr);
	//	}
	//}

	// return whatever we have placed inside the result set.
	return result;
}

//e.g. Parent(s1,_)
//get all parents
unordered_set<string> AffectsClause::getAllS1() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeAllS1();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsClause::getAllS1AndS2() {
	AffectsCalculator calc = AffectsCalculator();
	unordered_set<vector<string>> results = calc.computeAllS1AndS2();
	if (firstArg == secondArg) {
		unordered_set<vector<string>> filteredResult = unordered_set<vector<string>>();
		BOOST_FOREACH(vector<string> result, results) {
			if (result.at(0) == result.at(1)) {
				filteredResult.insert(result);
			}
		}
		return filteredResult;
	} else {
		return results;
	}
}

bool AffectsClause::modcheck(string var, GNode* gn, unordered_set<int>* visitedSet) {
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
		case PROG_ :
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

bool AffectsClause::modcheck(string var, GNode* gn, unordered_set<int>* visitedSet, int stmtNum) {
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

void AffectsClause::modadd(string var, GNode* gn, unordered_set<int>* resultSet, unordered_set<int>* visitedSet) {
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
		case PROG_ :
		case END_ :
			//print(*visitedSet);
			//cout << "end" << endl;
			return;

		case CALL_ :
			//cout << "call" << gn->getStartStmt() << endl;
			//print(*visitedSet);
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return;
			}
			visitedSet->insert(gn->getStartStmt());
			// need to check if it mods the var
			// if it does, then we cannot go up
			if (stmtTable->getStmtObj(gn->getStartStmt())->getModifies().count(var) >= 1) {
				return;
			} else {
				modadd(var, gn->getParents().at(0), resultSet, visitedSet);
				return;
			}

		case IF_ :
			//cout << "if" << gn->getStartStmt() << endl;
			//print(*visitedSet);
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return;
			}
			visitedSet->insert(gn->getStartStmt());
			modadd(var, gn->getParents().at(0), resultSet, visitedSet);
			return;

		case WHILE_ :
			//cout << "while" << gn->getStartStmt() << endl;
			//print(*visitedSet);
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return;
			}
			visitedSet->insert(gn->getStartStmt());
			modadd(var, gn->getParents().at(1), resultSet, visitedSet);
			modadd(var, gn->getParents().at(0), resultSet, visitedSet);
			return;
			
		case DUMMY_ :
			//cout << "dummy" << endl;
			dgn = (DummyGNode*) gn;
			//dgn_id = dgn->getIfParentStmt() * 100000 + dgn->getElseParentStmt() * 100;
			// use gdn.getparents().at(0).getstart() + 900000
			dgn_id = dgn->getEntrance()->getStartStmt() + 900000;
			//print(*visitedSet);
			if (visitedSet->count(dgn_id) >= 1) {
				return;
			}
			visitedSet->insert(dgn_id);
			modadd(var, gn->getParents().at(0), resultSet, visitedSet);
			modadd(var, gn->getParents().at(1), resultSet, visitedSet);
			return;
			
		case ASSIGN_ :
			//cout << "assign" << endl;
			modadd(var, gn, resultSet, visitedSet, gn->getEndStmt());
			return;

		default :
			//cout << "unknown node type" << endl;
			return;
	}
}

void AffectsClause::modadd(string var, GNode* gn, unordered_set<int>* resultSet, unordered_set<int>* visitedSet, int stmtNum) {
	//cout << "modadd with stmtnum = " << stmtNum << endl;

	if (gn->getNodeType() == ASSIGN_) {
		//cout << "end stmt = " << gn->getStartStmt() << endl;
		for (int i = stmtNum; i >= gn->getStartStmt(); i--) {
			//cout << i << endl;
			//print(*visitedSet);
			if (visitedSet->count(i) >= 1) {
				// visited this stmt before
				// above shud be visited before also
				//cout << "visited " << i << " before" << endl;
				return;
			}
			//cout << "never visited " << i << " before" << endl;
			visitedSet->insert(i);
			Statement* stmt = stmtTable->getStmtObj(i);
			unordered_set<string> modSet = stmt->getModifies();
			if (modSet.size() == 1) {
				// it must modify something
				// get that something
				string modVar = *(modSet.begin());
				if (modVar == var) {
					// if it modifies then note that down
					resultSet->insert(i);
					//cout << "inserted " << i << endl;
					// stop going further up this assg block
					return;
				}
			}
		}
		//cout << gn->getParents().size() << endl;
		modadd(var, gn->getParents().at(0), resultSet, visitedSet);
	}
}

void AffectsClause::priynt(unordered_set<int> s) {
	if (s.size() > 0) {
		//cout << "Visited: ";
		BOOST_FOREACH(auto x, s) {
			//cout << x << " ";
		}
		//cout << endl;
	}
}




