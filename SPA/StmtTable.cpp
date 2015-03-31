#pragma once

#include <set>
#include <boost/unordered_map.hpp>
#include "StmtTable.h"
#include "Statement.h"
#include "TNode.h"

using namespace boost;

bool StmtTable::instanceFlag = false;			// instance flag
StmtTable* StmtTable::tableInstance = NULL;		// table instance

set<int>::iterator setIter;						// stmt set iterator

// ACCESSORS
// gets instance of stmt table
StmtTable* StmtTable::getInstance() {
	// if instance does not exist, create and return instance
	if (!instanceFlag) {
		tableInstance = new StmtTable();
		instanceFlag = true;
	}

	return tableInstance;
}

// gets iterator to stmt table
StmtTable::StatementTable::iterator StmtTable::getIterator() {
	StmtTable::StatementTable::iterator tableIter = table.begin();

	return tableIter;
}

// gets the stmt obj by stmt number
Statement* StmtTable::getStmtObj(int stmtNum) {
	StmtTableIterator iter = table.find(stmtNum);

	// if not found
	if(iter == table.end()) {
		return NULL;
	} else {
		return iter->second;
	}
}

// gets all assignment stmts
const set<Statement*>& StmtTable::getAssgStmts() {
	return assgStmtSet;
}

// gets all call stmts
const set<Statement*>& StmtTable::getCallStmts() {
	return callStmtSet;
}

// gets all while stmts
const set<Statement*>& StmtTable::getWhileStmts() {
	return whileStmtSet;
}

// gets all if stmts
const set<Statement*>& StmtTable::getIfStmts() {
	return ifStmtSet;
}

// MUTATORS
// adds statement to table
void StmtTable::addStmt(Statement *stmt) {
	int stmtNum = stmt->getStmtNum();

	// add stmt to table
	table.emplace(stmtNum, stmt);

	// record type of stmt
	switch(stmt->getType()) {
		case ASSIGN_STMT_:
				assgStmtSet.insert(stmt);
				break;
		case CALL_STMT_:
				callStmtSet.insert(stmt);
				break;
		case IF_STMT_:
				ifStmtSet.insert(stmt);
				break;
		case WHILE_STMT_:
				whileStmtSet.insert(stmt);
				break;
		default:
			;
	}
}

// removes all statements to the table (for testing)
void StmtTable::clearTable() {
	// clear statements from table
	table.clear();

	// reset statement type sets
	assgStmtSet.clear();
	callStmtSet.clear();
	whileStmtSet.clear();
	ifStmtSet.clear();
}


// PRIVATE FUNCTIONS
// empty private constructor
StmtTable::StmtTable() {}