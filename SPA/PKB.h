#pragma once

#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Statement.h"
#include "Variable.h"
#include "Procedure.h"
#include <utility>
#include <boost/unordered_set.hpp>

using boost::unordered_set;

class PKB {

public:
	static PKB* getInstance();											// get instance of PKB

	/*----- USES -----*/
	bool isStmtUsingVar(int, string);									// evaluates whether stmt uses variable
	bool isProcUsingVar(string, string);								// evaluates whether proc uses variable
	bool isStmtUsing(int);												// evaluates whether stmt is using anything
	bool isProcUsing(string);											// evaluates whether proc is using anything
	bool isVarBeingUsed(string);										// evaluates whether var is used by anything
	bool isAnyVarBeingUsed();											// evaluates whether any var is being used

	unordered_set<string> getVarsUsedByStmt(int);						// gets all variables used by stmt
	unordered_set<string> getVarsUsedByProc(string);					// gets all variables used by proc
	unordered_set<string> getStmtsUsingVar(string);						// gets all stmts using the variable
	unordered_set<string> getAssignStmtsUsingVar(string);				// gets all assign stmts using the variable
	unordered_set<string> getWhileStmtsUsingVar(string);				// gets all while stmts using the variable
	unordered_set<string> getIfStmtsUsingVar(string);					// gets all if stmts using the variable
	unordered_set<string> getProcsUsingVar(string);						// gets all procs using the variable
	unordered_set<string> getVarsBeingUsed();							// gets all variables used by anything
	unordered_set<string> getStmtsUsingAny();							// gets all stmts using anything
	unordered_set<string> getAssignStmtsUsingAny();						// gets all assign stmts using anything
	unordered_set<string> getWhileStmtsUsingAny();						// gets all while stmts using anything
	unordered_set<string> getIfStmtsUsingAny();							// gets all if stmts using anything
	unordered_set<string> getProcsUsingAny();							// gets all procs using anything

	unordered_set<pair<string, string>> getAllStmtVarUsePairs();		// gets all stmt-var use pairs
	unordered_set<pair<string, string>> getAllAssignStmtVarUsePairs();	// gets all assign stmt-var use pairs
	unordered_set<pair<string, string>> getAllWhileStmtVarUsePairs();	// gets all while stmt-var use pairs
	unordered_set<pair<string, string>> getAllIfStmtVarUsePairs();		// gets all if stmt-var use pairs
	unordered_set<pair<string, string>> getAllProcVarUsePairs();		// gets all proc-var use pairs


	/*----- MODIFIES ------*/







};