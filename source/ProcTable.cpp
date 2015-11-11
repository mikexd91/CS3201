#include <set>
#include "boost/unordered_map.hpp"
#include "boost/foreach.hpp"
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

// gets the proc obj by proc name
Procedure* ProcTable::getProcObj(string procName) {
	ProcTableIterator iter = table.find(procName);

	// if not found
	if(iter == table.end()) {
		return NULL;
	} else {
		return iter->second;
	}
}

// gets all proc obj in the table
const unordered_set<Procedure*>& ProcTable::getAllProcs() {
	unordered_set<Procedure*> *procSet = new unordered_set<Procedure*>();

	BOOST_FOREACH(auto p, table) {
		procSet->emplace(p.second);
	}

	return *procSet;
}

// gets set of procedures called by procName
const unordered_set<string>& ProcTable::getCalls(const string &procName) {
	Procedure* proc = table.find(procName)->second;

	unordered_set<string>* calls = new unordered_set<string>();

	// get set of procedure call references
	Procedure::CallsSet procSet = proc->getCalls();
	BOOST_FOREACH(auto p, procSet) {
		calls->insert(p->getProcName());
	}

	return *calls;
}

// gets set of variables modified by procName
const unordered_set<string>& ProcTable::getModifies(const string &procName) {
	Procedure* proc = table.find(procName)->second;

	unordered_set<string>* modifies = new unordered_set<string>();

	// get set of procedure call references
	Procedure::ModifiesSet procSet = proc->getModifies();
	BOOST_FOREACH(auto p, procSet) {
		modifies->insert(p);
	}

	return *modifies;
}

// gets set of variables used by procName
const unordered_set<string>& ProcTable::getUses(const string &procName) {
	Procedure* proc = table.find(procName)->second;

	unordered_set<string>* uses = new unordered_set<string>();

	// get set of procedure call references
	Procedure::UsesSet procSet = proc->getUses();
	BOOST_FOREACH(auto p, procSet) {
		uses->insert(p);
	}

	return *uses;
}

// MUTATORS
// adds proc to table
void ProcTable::addProc(Procedure *proc) {
	string procName = proc->getProcName();

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
	ProcTableIterator iter = getIterator();

	BOOST_FOREACH(auto p, table) {
		if(p.first.compare(procName) == 0) {
			return true;
		}
	}

	return false;
}


// PRIVATE FUNCTIONS
// empty private constructor
ProcTable::ProcTable() {}