#pragma once

#include <set>
#include <boost/unordered_map.hpp>
#include "Statement.h"

using namespace boost;

class StmtTable {
	/*

	StmtTable Implementation:
	---------------------------------
	|_[key]______|__________________|
	| stmt no.	 | stmt object ref	|
	|_[key]______|__________________|
	---------------------------------

	*/

	typedef boost::unordered_map<int, Statement*> StatementTable;

public:
	// ACCESSORS
	static StmtTable* getInstance();			// gets instance of stmt table

	StatementTable::iterator getIterator();		// gets iterator to stmt table

	const set<Statement>& getAssgStmts();		// gets all assignment stmts
	const set<Statement>& getCallStmts();		// gets all call stmts
	const set<Statement>& getWhileStmts();		// gets all while stmts
	const set<Statement>& getIfStmts();			// gets all if stmts

	// MUTATORS
	void addStmt(Statement *stmt);	// adds statement to table

private:
	StatementTable table;
	set<int> assgStmtSet;
	set<int> callStmtSet;
	set<int> whileStmtSet;
	set<int> ifStmtSet;
	
	// SINGLETON
	static bool instanceFlag;
	static StmtTable* tableInstance;

	// SINGLETON CONSTRUCTOR
	StmtTable();

};