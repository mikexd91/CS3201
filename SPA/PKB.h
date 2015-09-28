#pragma once

#include "StmtTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Statement.h"
#include "Variable.h"
#include "Procedure.h"
#include "PDR.h"
#include "ParsedData.h"
#include "Utils.h"

#include <utility>
#include <boost/unordered_set.hpp>

using boost::unordered_set;
using namespace stringconst;

class PKB {

public:
	static PKB* getInstance();																			// get instance of PKB
	void processParsedData(ParsedData);																	// pass parsed data to PDR

	/*----- USES -----*/
	bool isStmtUsingVar(int, string);																	// evaluates whether stmt uses variable
	bool isProcUsingVar(string, string);																// evaluates whether proc uses variable
	bool isStmtUsing(int);																				// evaluates whether stmt is using anything
	bool isProcUsing(string);																			// evaluates whether proc is using anything
	bool isVarBeingUsed(string);																		// evaluates whether var is used by anything
	bool isAnyVarBeingUsed();																			// evaluates whether any var is being used

	unordered_set<string> getVarsUsedByStmt(int);														// gets all variables used by stmt
	unordered_set<string> getVarsUsedByProc(string);													// gets all variables used by proc
	unordered_set<string> getStmtsUsingVar(string var, string argType=ARG_STATEMENT);					// gets all stmts of argType using the variable				
	unordered_set<string> getProcsUsingVar(string);														// gets all procs using the variable
	unordered_set<string> getVarsBeingUsed();															// gets all variables used by anything
	unordered_set<string> getStmtsUsingAny(string argType=ARG_STATEMENT);								// gets all stmts of argType using anything
	unordered_set<string> getProcsUsingAny();															// gets all procs using anything

	unordered_set<pair<string, string>> getAllStmtVarUsePairs(string argType=ARG_STATEMENT);			// gets all stmt(of argType)-var use pairs
	unordered_set<pair<string, string>> getAllProcVarUsePairs();										// gets all proc-var use pairs


	/*----- MODIFIES -----*/
	bool isStmtModifyingVar(int, string);																// evaluates whether stmt modifies variable
	bool isProcModifyingVar(string, string);															// evaluates whether proc modifies variable
	bool isStmtModifying(int);																			// evaluates whether stmt is modifying anything
	bool isProcModifying(string);																		// evaluates whether proc is modifying anything
	bool isVarBeingModified(string);																	// evaluates whether var is modified by anything
	bool isAnyVarBeingModified();																		// evaluates whether any var is being modified

	unordered_set<string> getVarsModifiedByStmt(int);													// gets all variables modified by stmt
	unordered_set<string> getVarsModifiedByProc(string);												// gets all variables modifiedby proc
	unordered_set<string> getStmtsModifyingVar(string var, string argType=ARG_STATEMENT);				// gets all stmts modifying the variable
	unordered_set<string> getProcsModifyingVar(string);													// gets all procs modifying the variable
	unordered_set<string> getVarsBeingModified();														// gets all variables modifiedby anything
	unordered_set<string> getStmtsModifyingAny(string argType=ARG_STATEMENT);							// gets all stmts of argType modifying anything
	unordered_set<string> getProcsModifyingAny();														// gets all procs modifying anything

	unordered_set<pair<string, string>> getAllStmtVarModifyPairs(string argType=ARG_STATEMENT);			// gets all stmt-var modify pairs
	unordered_set<pair<string, string>> getAllProcVarModifyPairs();										// gets all proc-var modify pairs


	/*----- PARENT -----*/
	bool isStmtParent(int, int);																		// evaluates whether s1 is parent of s2
	bool hasParent(int);																				// evaluates whether stmt has a parent
	bool isParent(int);																					// evaluates whether stmt is parent of another stmt
	bool isParentExist();																				// evaluates whether a parent stmt exists

	unordered_set<string> getChildStmts(int);															// gets all child stmts of this stmt
	string getParentStmt(int);																			// get parent stmt of this stmt
	unordered_set<string> getAllChildStmts(string argType=ARG_STATEMENT);								// get all child stmts of argType
	unordered_set<string> getAllParentStmts(string argType=ARG_STATEMENT);								// get all parent stmts of argType

	unordered_set<pair<string, string>> getAllParentChildPairs(string parentType, string childType);	// gets all parent-child pairs of respective types


	/*----- PARENTSTAR -----*/
	bool isStmtParentStar(int, int);																	// evaluates whether s1 is parent star of s2
	bool hasParentStar(int);																			// evaluates whether stmt has a parent star
	bool isParentStar(int);																				// evaluates whether stmt is parent star of another stmt
	bool isParentStarExist();																			// evaluates whether a parent star stmt exists

	unordered_set<string> getChildStarStmts(int);														// gets all child star stmts of this stmt
	unordered_set<string> getParentStarStmts(int);														// get parent star stmt of this stmt
	unordered_set<string> getAllChildStarStmts(string argType=ARG_STATEMENT);							// get all child star stmts of argType
	unordered_set<string> getAllParentStarStmts(string argType=ARG_STATEMENT);							// get all parent star stmts of argType

	unordered_set<pair<string, string>> getAllParentChildStarPairs(string parentType, string childType);// gets all parent-child star pairs of respective types


	/*----- FOLLOWS -----*/


	/*----- FOLLOWSSTAR -----*/


	/*----- CALLS -----*/
	bool isCalls(string, string);																		// evaluates whether p1 calls p2
	bool hasCaller(string);																				// evaluates whether proc is called by another proc
	bool isCaller(string);																				// evaluates whether proc calls another proc
	bool isCallsExist();																				// evaluates whether any proc calls another proc

	unordered_set<string> getProcsCalled(string);														// gets all procs called by this proc
	unordered_set<string> getProcCallers(string);														// gets all procs that calls this proc
	unordered_set<string> getAllProcsCalled();															// gets all procs that are called by any proc
	unordered_set<string> getAllProcCallers();															// gets all procs that call any proc

	unordered_set<pair<string, string>> getAllCallsPairs();												// gets all proc-proc calls pairs


	/*----- CALLSSTAR -----*/
	bool isCallsStar(string, string);																		// evaluates whether p1 calls star p2
	bool hasCallerStar(string);																				// evaluates whether proc is called star by another proc
	bool isCallerStar(string);																				// evaluates whether proc calls star another proc
	bool isCallsStarExist();																				// evaluates whether any proc calls star another proc

	unordered_set<string> getProcsCalledStar(string);														// gets all procs called star by this proc
	unordered_set<string> getProcCallersStar(string);														// gets all procs that calls star this proc
	unordered_set<string> getAllProcsCalledStar();															// gets all procs that are called star by any proc
	unordered_set<string> getAllProcCallersStar();															// gets all procs that call star any proc

	unordered_set<pair<string, string>> getAllCallsStarPairs();												// gets all proc-proc calls star pairs


private:
	// SINGLETON
	static bool instanceFlag;
	static PKB* pkbInstance;

	// SINGLETON CONSTRUCTOR
	PKB();

};