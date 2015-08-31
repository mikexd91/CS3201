#pragma once

#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include "Procedure.h"

using boost::unordered_set;

class ProcTable {
	/*

	ProcTable Implementation:
	---------------------------------
	|_[key]______|__________________|
	| proc name	 | proc object ref	|
	|_[key]______|__________________|
	---------------------------------

	*/

	typedef boost::unordered_map<string, Procedure*> ProcedureTable;
	typedef ProcedureTable::iterator ProcTableIterator;

public:
	// ACCESSORS
	static ProcTable* getInstance();						// gets instance of proc table

	ProcedureTable::iterator getIterator();					// gets iterator to proc table
	ProcedureTable::iterator getEnd();						// gets iterator to end of proc table
	Procedure* getProcObj(string procName);					// gets the proc obj by proc name

	const unordered_set<string>& getCalls(const string &procName);	// gets set of procedures called by procName
	const unordered_set<string>& getModifies(const string &procName);// gets set of variables modified by procName
	const unordered_set<string>& getUses(const string &procName);	// gets set of variables used by procName

	// MUTATORS
	void addProc(Procedure *proc);							// adds proc to table
	void clearTable();										// removes all procedures to the table (for testing)

	// GENERAL METHODS
	bool contains(const string &procName);					// checks if proc exists in table

private:
	ProcedureTable table;

	// SINGLETON
	static bool instanceFlag;
	static ProcTable* tableInstance;

	// SINGLETON CONSTRUCTOR
	ProcTable();

};