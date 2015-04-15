#pragma once

#include "boost/unordered_map.hpp"
#include "Procedure.h"

class ProcTable {
	/*

	ProcTable Implementation:
	---------------------------------
	|_[key]______|__________________|
	| stmt no.	 | proc object ref	|
	|_[key]______|__________________|
	---------------------------------

	*/

	typedef boost::unordered_map<string*, Procedure*> ProcedureTable;

public:
	// ACCESSORS
	static ProcTable* getInstance();			// gets instance of proc table

	ProcedureTable::iterator getIterator();		// gets iterator to proc table
	ProcedureTable::iterator getEnd();			// gets iterator to end of proc table

	// MUTATORS
	void addProc(Procedure *proc);				// adds proc to table
	void clearTable();							// removes all procedures to the table (for testing)

	// GENERAL METHODS
	bool contains(const string &procName);		// checks if proc exists in table

private:
	ProcedureTable table;

	// SINGLETON
	static bool instanceFlag;
	static ProcTable* tableInstance;

	// SINGLETON CONSTRUCTOR
	ProcTable();

};