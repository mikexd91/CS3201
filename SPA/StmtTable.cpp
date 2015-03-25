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

// gets all assignment stmts
const set<Statement*>& StmtTable::getAssgStmts() {
	set<Statement*> stmtSet;

	for(setIter=assgStmtSet.begin(); setIter!=assgStmtSet.end(); setIter++) {
		stmtSet.insert(table.at(*setIter));
	}

	return stmtSet;
}

// gets all call stmts
const set<Statement*>& StmtTable::getCallStmts() {
	set<Statement*> stmtSet;

	for(setIter=callStmtSet.begin(); setIter!=callStmtSet.end(); setIter++) {
		stmtSet.insert(table.at(*setIter));
	}

	return stmtSet;
}

// gets all while stmts
const set<Statement*>& StmtTable::getWhileStmts() {
	set<Statement*> stmtSet;

	for(setIter=whileStmtSet.begin(); setIter!=whileStmtSet.end(); setIter++) {
		stmtSet.insert(table.at(*setIter));
	}

	return stmtSet;
}

// gets all if stmts
const set<Statement*>& StmtTable::getIfStmts() {
	set<Statement*> stmtSet;

	for(setIter=ifStmtSet.begin(); setIter!=ifStmtSet.end(); setIter++) {
		stmtSet.insert(table.at(*setIter));
	}

	return stmtSet;
}

// MUTATORS
// adds statement to table
void StmtTable::addStmt(Statement *stmt) {
	int stmtNum = stmt->getStmtNum();

	// add stmt to table
	table.emplace(stmtNum, stmt);

	// record type of stmt
	switch(stmt->getType()) {
		case NodeType::ASSIGN_STMT_:
				assgStmtSet.insert(stmtNum);
				break;
		case NodeType::CALL_STMT_:
				callStmtSet.insert(stmtNum);
				break;
		case NodeType::IF_STMT_:
				ifStmtSet.insert(stmtNum);
				break;
		case NodeType::WHILE_STMT_:
				whileStmtSet.insert(stmtNum);
				break;
		default:
			;
	}
}


// PRIVATE FUNCTIONS
// empty private constructor
StmtTable::StmtTable() {}