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
	//check modifies/uses aspects of firstArg and secondArg
	int stmtNum1 = boost::lexical_cast<int>(firstArg);
	int stmtNum2 = boost::lexical_cast<int>(secondArg);
	Statement* stmt1 = stmtTable->getStmtObj(stmtNum1);
	Statement* stmt2 = stmtTable->getStmtObj(stmtNum2);
	Procedure* proc1 = stmt1->getProc();
	Procedure* proc2 = stmt2->getProc();
	if (stmt1->getType() != ASSIGN_STMT_ || stmt2->getType() != ASSIGN_STMT_ || proc1 != proc2) {
		return false;
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	unordered_set<string> uses2 = stmt2->getUses();
	string modifyingVar;
	if (modifies1.size() != 1 || uses2.empty()) {
		//error
		if (modifies1.size() != 1) {
			cout << "Assignment statements should only have 1 modify variable";	
		}
		return false;
	} else {
		modifyingVar = *modifies1.begin();
		if (uses2.find(modifyingVar) == uses2.end()) {
			//stmt2 does not use a variable that stmt1 modifies.
			return false;
		}
	}
	//if both are in same procedure
	//check if stmt2 next* stmt1
	CFGIterator iterator = CFGIterator(stmt1->getGNodeRef());
	GNode* currentNode = iterator.getNextNode();
	while (!currentNode->isNodeType(END_)){
		if (currentNode->isNodeType(ASSIGN_)) {
			AssgGNode* assgNode = static_cast<AssgGNode*>(currentNode);
			int startNum;
			if (iterator.isStart()) {
				startNum = stmtNum1+1;
			} else {
				startNum = assgNode->getStartStmt();
			}
			for (int i =startNum; i <= assgNode->getEndStmt(); i++) {
				if (i == stmtNum2) {
					return true;
				} else {
					Statement* assgStmt = stmtTable->getStmtObj(i);
					//if there is a statement that modifies the variable
					if (assgStmt->getModifies().find(modifyingVar) != assgStmt->getModifies().end()) {
						if(!toContinueForFixed(iterator)) {
							//break
							return false;
						}
					}
				}
			}
		} else if (currentNode->isNodeType(WHILE_)) {
			WhileGNode* whileNode = static_cast<WhileGNode*>(currentNode);
			Statement* whileStmt = stmtTable->getStmtObj(whileNode->getStartStmt());
			bool isWithinWhile = whileStmt->getChildrenStar().find(stmtNum2) != whileStmt->getChildrenStar().end();
			if (!isWithinWhile) {
				iterator.skipWhileLoop(whileNode);
			}
		} else if (currentNode->isNodeType(CALL_)) {
			//check if called procedure modifies var
			CallGNode* callNode = static_cast<CallGNode*>(currentNode);
			Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
			if (callStmt->getModifies().find(modifyingVar) != callStmt->getModifies().end()) {
				if (!toContinueForFixed(iterator)) {
					return false;
				}
			}
		}
		currentNode = iterator.getNextNode();
	}
	return false;
}

//e.g. Parent(_,_)
bool AffectsClause::evaluateS1GenericS2Generic() {
	AffectsCalculator calc = AffectsCalculator();
	return calc.computeS1GenericS2Generic();
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsClause::getAllS2WithS1Fixed(string s1) {
	//check modifies/uses aspects of firstArg and secondArg
	unordered_set<string> results = unordered_set<string>();
	int stmtNum1 = boost::lexical_cast<int>(s1);
	Statement* stmt1 = stmtTable->getStmtObj(stmtNum1);
	if (stmt1->getType() != ASSIGN_STMT_) {
		return unordered_set<string>();
	}
	unordered_set<string> modifies1 = stmt1->getModifies();
	string modifyingVar;
	if (modifies1.size() != 1) {
		//error
		cout << "Assignment statements should only have 1 modify variable";	
		return results;
	} else {
		modifyingVar = *modifies1.begin();
	}

	//if both are in same procedure
	//check if stmt2 next* stmt1
	CFGIterator iterator = CFGIterator(stmt1->getGNodeRef());
	GNode* currentNode = iterator.getNextNode();
	while (!currentNode->isNodeType(END_)){
		if (currentNode->isNodeType(ASSIGN_)) {
			AssgGNode* assgNode = static_cast<AssgGNode*>(currentNode);
			int startNum;
			if (iterator.isStart()) {
				startNum = stmtNum1+1;
			} else {
				startNum = assgNode->getStartStmt();
			}
			for (int i =startNum; i <= assgNode->getEndStmt(); i++) {
				Statement* assgStmt = stmtTable->getStmtObj(i);
				if (assgStmt->getUses().find(modifyingVar) != assgStmt->getUses().end()) {
					results.insert(lexical_cast<string>(assgStmt->getStmtNum()));
				}
				if (assgStmt->getModifies().find(modifyingVar) != assgStmt->getModifies().end()) {
					if(!toContinue(iterator)) {
						return results;
					}
				}
			}
		} else if (currentNode->isNodeType(CALL_)) {
			//check if called procedure modifies var
			CallGNode* callNode = static_cast<CallGNode*>(currentNode);
			Statement* callStmt = stmtTable->getStmtObj(callNode->getStartStmt());
			if (callStmt->getModifies().find(modifyingVar) != callStmt->getModifies().end()) {
				if (!toContinue(iterator)) {
					return results;
				}
			}
		}
		currentNode = iterator.getNextNode();
	}
	return results;
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
	
	// get the statement object and make sure it is an assign stmt
	int stmtNum = lexical_cast<int>(s2);
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	if (stmt->getType() != ASSIGN_STMT_) {
		return result;
	}

	// get the uses set
	unordered_set<string> usesSet = stmt->getUses();
	if (usesSet.size() <= 0) {
		//if the assignment doesnt use anything, then nothing affects it
		return result;
	}

	// get the gnode of this stmt
	GNode* gn = stmt->getGNodeRef();

	BOOST_FOREACH(string var, usesSet) {
		//cout << "using " << var << endl;
		unordered_set<int> intResults;// = new unordered_set<int>();
		modadd(var, gn, &intResults, new unordered_set<int>(), stmtNum);
		//cout << "done with " << var << endl;
		//cout << intResults.size() << endl;
		BOOST_FOREACH(int r, intResults) {
			//cout << r << endl;
			string rs = to_string((long long) r);
			result.insert(rs);
		}
		//cout << "dont" << endl;
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

bool AffectsClause::toContinue(CFGIterator& iterator) {
	if (iterator.isInIfContainer()) {
		IfGNode* ifNode = iterator.getCurrentIfNode();
		if (iterator.toConsiderElseStmt()) {
			//if we should consider else stmt (consider both branches) -> consider else branch
			iterator.skipThenStmt(ifNode);
		} else {
			//we have considered both then and else stmt, let's see whether we should proceed with evaluation
			iterator.skipElseStmt(ifNode);
		}
		return true;
	} else if (iterator.isInWhileLoop()) {
		//if the bad assg stmt is in a while loop, skip the while loop
		//as there may be a path outside it that does not affect
		WhileGNode* whileNode = iterator.getCurrentWhileNode();
		iterator.skipWhileLoop(whileNode);
		return true;
	} else {
		//there is no else branch, and we are not in a while loop
		return false;
	}
}

bool AffectsClause::toContinueForFixed(CFGIterator& iterator) {
	if (iterator.isInIfContainer()) {
		IfGNode* ifNode = iterator.getCurrentIfNode();
		if (iterator.toConsiderElseStmt()) {
			//if we should consider else stmt (consider both branches) -> consider else branch
			iterator.skipThenStmt(ifNode);
		} else {
			//we have considered both then and else stmt, let's see whether we should proceed with evaluation
			iterator.skipElseStmt(ifNode);
		}
		return true;
	} else {
		//2 possibilities:
		//1. in an ordinary set of assg stmt -> no other path
		//2. while loop will only occur if the start stmt is in a while loop, or if our end stmt is inside the while loop,
		//it will fail either way
		return false;
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
			dgn_id = dgn->getIfParentStmt() * 1000 + dgn->getElseParentStmt() * 100;
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
			print(*visitedSet);
			//cout << "end" << endl;
			return;

		case CALL_ :
			//cout << "call" << endl;
			print(*visitedSet);
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
			//cout << "if" << endl;
			print(*visitedSet);
			if (visitedSet->count(gn->getStartStmt()) >= 1) {
				return;
			}
			visitedSet->insert(gn->getStartStmt());
			modadd(var, gn->getParents().at(0), resultSet, visitedSet);
			return;

		case WHILE_ :
			//cout << "while" << gn->getStartStmt() << endl;
			print(*visitedSet);
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
			dgn_id = dgn->getIfParentStmt() * 100000 + dgn->getElseParentStmt() * 100;
			print(*visitedSet);
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
			print(*visitedSet);
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
		////cout << gn->getParents().size() << endl;
		modadd(var, gn->getParents().at(0), resultSet, visitedSet);
	}
}

void AffectsClause::print(unordered_set<int> s) {
	if (s.size() > 0) {
		//cout << "Visited: ";
		BOOST_FOREACH(auto x, s) {
			//cout << x << " ";
		}
		//cout << endl;
	}
}




