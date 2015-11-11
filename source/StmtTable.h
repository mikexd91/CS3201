#pragma once

#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include "Statement.h"

using boost::unordered_set;

class StmtTable {
	/*

	StmtTable Implementation:
	---------------------------------
	|_[key]______|__________________|
	| stmt no.	 | stmt object ref	|
	|_[key]______|__________________|
	---------------------------------

	*/

public:
	typedef boost::unordered_map<int, Statement*> StatementTable;
	typedef StatementTable::iterator StmtTableIterator;

	// ACCESSORS
	static StmtTable* getInstance();			// gets instance of stmt table

	StatementTable::iterator getIterator();		// gets iterator to stmt table
	StatementTable::iterator getEnd();			// gets iterator to end of stmt table
	Statement* getStmtObj(int stmtNum);			// gets the stmt obj by stmt number

	const unordered_set<Statement*>& getAssgStmts();		// gets all assignment stmts
	const unordered_set<Statement*>& getCallStmts();		// gets all call stmts
	const unordered_set<Statement*>& getWhileStmts();		// gets all while stmts
	const unordered_set<Statement*>& getIfStmts();			// gets all if stmts
	const unordered_set<Statement*>& getAllStmts();			// gets all stmts

	// MUTATORS
	void addStmt(Statement *stmt);	// adds statement to table
	void clearTable();				// removes all statements to the table (for testing)

private:
	StatementTable table;
	unordered_set<Statement*> assgStmtSet;
	unordered_set<Statement*> callStmtSet;
	unordered_set<Statement*> whileStmtSet;
	unordered_set<Statement*> ifStmtSet;
	
	// SINGLETON
	static bool instanceFlag;
	static StmtTable* tableInstance;

	// SINGLETON CONSTRUCTOR
	StmtTable();

};