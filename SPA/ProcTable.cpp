#pragma once

#include "boost/unordered_map.hpp"
#include "Procedure.h"
#include "ProcTable.h"

using namespace boost;

bool ProcTable::instanceFlag = false;			// instance flag
ProcTable* ProcTable::tableInstance = NULL;		// table instance

// ACCESSORS
// gets instance of proc table
ProcTable* ProcTable::getInstance() {
	// if instance does not exist, create and return instance
	if (!instanceFlag) {
		tableInstance = new ProcTable();
		instanceFlag = true;
	}

	return tableInstance;
}

// gets iterator to proc table
ProcTable::ProcedureTable::iterator ProcTable::getIterator() {
	ProcTable::ProcedureTable::iterator tableIter = table.begin();

	return tableIter;
}

// gets iterator to end of proc table
ProcTable::ProcedureTable::iterator ProcTable::getEnd() {
	ProcTable::ProcedureTable::iterator tableIter = table.end();

	return tableIter;
}

// MUTATORS
// adds proc to table
void ProcTable::addProc(Procedure *proc) {
	string *procName = proc->getProcName();

	// add proc to table
	table.emplace(procName, proc);
}

// removes all procedures to the table (for testing)
void ProcTable::clearTable() {
	table.clear();
}

// GENERAL METHODS
// checks if proc exists in table
bool ProcTable::contains(const string &procName) {
	ProcedureTable::iterator iter = getIterator();

	for( ; iter!=table.end(); iter++) {
		if(*iter->first == procName) {
			return true;
		}
	}

	return false;
}


// PRIVATE FUNCTIONS
// empty private constructor
ProcTable::ProcTable() {}