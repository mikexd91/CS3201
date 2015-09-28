#pragma once

#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Statement.h"
#include "Variable.h"
#include "Procedure.h"
#include "PDR.h"
#include "ParsedData.h"

#include <utility>
#include <boost/unordered_set.hpp>

using boost::unordered_set;

class PKB {

public:
	static PKB* getInstance();												// get instance of PKB
	void processParsedData(ParsedData);										// pass parsed data to PDR

	/*----- USES -----*/
	bool isStmtUsingVar(int, string);										// evaluates whether stmt uses variable
	bool isProcUsingVar(string, string);									// evaluates whether proc uses variable
	bool isStmtUsing(int);													// evaluates whether stmt is using anything
	bool isProcUsing(string);												// evaluates whether proc is using anything
	bool isVarBeingUsed(string);											// evaluates whether var is used by anything
	bool isAnyVarBeingUsed();												// evaluates whether any var is being used

	unordered_set<string> getVarsUsedByStmt(int);							// gets all variables used by stmt
	unordered_set<string> getVarsUsedByProc(string);						// gets all variables used by proc
	unordered_set<string> getStmtsUsingVar(string);							// gets all stmts using the variable
	unordered_set<string> getAssignStmtsUsingVar(string);					// gets all assign stmts using the variable
	unordered_set<string> getWhileStmtsUsingVar(string);					// gets all while stmts using the variable
	unordered_set<string> getIfStmtsUsingVar(string);						// gets all if stmts using the variable
	unordered_set<string> getProcsUsingVar(string);							// gets all procs using the variable
	unordered_set<string> getVarsBeingUsed();								// gets all variables used by anything
	unordered_set<string> getStmtsUsingAny();								// gets all stmts using anything
	unordered_set<string> getAssignStmtsUsingAny();							// gets all assign stmts using anything
	unordered_set<string> getWhileStmtsUsingAny();							// gets all while stmts using anything
	unordered_set<string> getIfStmtsUsingAny();								// gets all if stmts using anything
	unordered_set<string> getProcsUsingAny();								// gets all procs using anything

	unordered_set<pair<string, string>> getAllStmtVarUsePairs();			// gets all stmt-var use pairs
	unordered_set<pair<string, string>> getAllAssignStmtVarUsePairs();		// gets all assign stmt-var use pairs
	unordered_set<pair<string, string>> getAllWhileStmtVarUsePairs();		// gets all while stmt-var use pairs
	unordered_set<pair<string, string>> getAllIfStmtVarUsePairs();			// gets all if stmt-var use pairs
	unordered_set<pair<string, string>> getAllProcVarUsePairs();			// gets all proc-var use pairs


	/*----- MODIFIES -----*/
	bool isStmtModifyingVar(int, string);									// evaluates whether stmt modifies variable
	bool isProcModifyingVar(string, string);								// evaluates whether proc modifies variable
	bool isStmtModifying(int);												// evaluates whether stmt is modifying anything
	bool isProcModifying(string);											// evaluates whether proc is modifying anything
	bool isVarBeingModified(string);										// evaluates whether var is modified by anything
	bool isAnyVarBeingModified();											// evaluates whether any var is being modified

	unordered_set<string> getVarsModifiedByStmt(int);						// gets all variables modified by stmt
	unordered_set<string> getVarsModifiedByProc(string);					// gets all variables modifiedby proc
	unordered_set<string> getStmtsModifyingVar(string);						// gets all stmts modifying the variable
	unordered_set<string> getAssignStmtsModifyingVar(string);				// gets all assign stmts modifying the variable
	unordered_set<string> getWhileStmtsModifyingVar(string);				// gets all while stmts modifying the variable
	unordered_set<string> getIfStmtsModifyingVar(string);					// gets all if stmts modifying the variable
	unordered_set<string> getProcsModifyingVar(string);						// gets all procs modifying the variable
	unordered_set<string> getVarsBeingModified();							// gets all variables modifiedby anything
	unordered_set<string> getStmtsModifyingAny();							// gets all stmts modifying anything
	unordered_set<string> getAssignStmtsModifyingAny();						// gets all assign stmts modifying anything
	unordered_set<string> getWhileStmtsModifyingAny();						// gets all while stmts modifying anything
	unordered_set<string> getIfStmtsModifyingAny();							// gets all if stmts modifying anything
	unordered_set<string> getProcsModifyingAny();							// gets all procs modifying anything

	unordered_set<pair<string, string>> getAllStmtVarModifyPairs();			// gets all stmt-var modify pairs
	unordered_set<pair<string, string>> getAllAssignStmtVarModifyPairs();	// gets all assign stmt-var modify pairs
	unordered_set<pair<string, string>> getAllWhileStmtVarModifyPairs();	// gets all while stmt-var modify pairs
	unordered_set<pair<string, string>> getAllIfStmtVarModifyPairs();		// gets all if stmt-var modify pairs
	unordered_set<pair<string, string>> getAllProcVarModifyPairs();			// gets all proc-var modify pairs


	/*----- PARENT -----*/
	bool isStmtParent(int, int);											// evaluates whether s1 is parent of s2
	bool hasParent(int);													// evaluates whether stmt has a parent
	bool isParent(int);														// evaluates whether stmt is parent of another stmt
	bool isParentExist();													// evaluates whether a parent stmt exists

	unordered_set<string> getChildStmts(int);								// gets all child stmts of this stmt
	unordered_set<string> getParentStmt(int);								// get parent stmt of this stmt
	unordered_set<string> getAllChildStmts();								// get all child stmts
	unordered_set<string> getAllWhileChildStmts();							// get all child stmts of while stmts
	unordered_set<string> getAllIfChildStmts();								// get all child stmts of if stmts
	unordered_set<string> getAllParentStmts();								// get all parent stmts
	unordered_set<string> getAllWhileParentStmts();							// get all parent stmts of while type
	unordered_set<string> getAllIfParentStmts();							// get all parent stmts of if type

	unordered_set<pair<string, string>> getAllParentChildPairs();			// gets all parent-child pairs
	unordered_set<pair<string, string>> getAllWhileParentChildPairs();		// gets all while parent-child pairs
	unordered_set<pair<string, string>> getAllIfParentChildPairs();			// gets all if parent-child pairs


	/*----- PARENTSTAR -----*/
	bool isStmtParentStar(int, int);										// evaluates whether s1 is parent star of s2
	bool hasParentStar(int);												// evaluates whether stmt has a parent star
	bool isParentStar(int);													// evaluates whether stmt is parent star of another stmt
	bool isParentStarExist();												// evaluates whether a parent star stmt exists

	unordered_set<string> getChildStarStmts(int);							// gets all child star stmts of this stmt
	unordered_set<string> getParentStarStmt(int);							// get parent star stmt of this stmt
	unordered_set<string> getAllChildStarStmts();							// get all child star stmts
	unordered_set<string> getAllWhileChildStarStmts();						// get all child star stmts of while stmts
	unordered_set<string> getAllIfChildStarStmts();							// get all child star stmts of if stmts
	unordered_set<string> getAllParentStarStmts();							// get all parent star stmts
	unordered_set<string> getAllWhileParentStarStmts();						// get all parent star stmts of while type
	unordered_set<string> getAllIfParentStarStmts();						// get all parent star stmts of if type

	unordered_set<pair<string, string>> getAllParentChildStarPairs();		// gets all parent-child star pairs
	unordered_set<pair<string, string>> getAllWhileParentChildStarPairs();	// gets all while parent-child star pairs
	unordered_set<pair<string, string>> getAllIfParentChildStarPairs();		// gets all if parent-child star pairs

	/*----- FOLLOWS -----*/


	/*----- FOLLOWSSTAR -----*/


private:
	// SINGLETON
	static bool instanceFlag;
	static PKB* pkbInstance;

	// SINGLETON CONSTRUCTOR
	PKB();

};