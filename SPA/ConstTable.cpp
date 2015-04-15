#pragma once

#include "boost/unordered_map.hpp"
#include "Constant.h"
#include "ConstTable.h"

using namespace std;
using namespace boost;

bool ConstTable::instanceFlag = false;			// instance flag
ConstTable* ConstTable::tableInstance = NULL;	// table instance

// ACCESSORS
// gets instance of const table
ConstTable* ConstTable::getInstance() {
	// if instance does not exist, create and return instance
	if (!instanceFlag) {
		tableInstance = new ConstTable();
		instanceFlag = true;
	}

	return tableInstance;
}

// gets iterator to const table
ConstTable::ConstantTable::iterator ConstTable::getIterator() {
	ConstTable::ConstantTable::iterator tableIter = table.begin();

	return tableIter;
}

// gets iterator to end of table
ConstTable::ConstantTable::iterator ConstTable::getEnd() {
	ConstTable::ConstantTable::iterator tableIter = table.end();

	return tableIter;
}

// gets const by name
Constant* ConstTable::getConst(const string &name) {
	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		string currentName = tableIter->first;

		if(currentName == name) {
			return tableIter->second;
		}
	}

	return NULL;
}

// gets const by value
Constant* ConstTable::getConst(int value) {
	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		Constant* currentConst = tableIter->second;

		if(currentConst->getValue() == value) {
			return currentConst;
		}
	}

	return NULL;
}

// gets all const objects
vector<Constant*> ConstTable::getAllConst() {
	vector<Constant*> allConst = vector<Constant*>();

	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		allConst.push_back(tableIter->second);
	}

	return allConst;
}

// gets all const names
vector<string> ConstTable::getAllConstNames() {
	vector<string> allConstNames = vector<string>();

	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		allConstNames.push_back(tableIter->second->getConstName());
	}

	return allConstNames;
}

// gets all const values
vector<int> ConstTable::getAllConstValues() {
	vector<int> allConstValues = vector<int>();

	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		allConstValues.push_back(tableIter->second->getValue());
	}

	return allConstValues;
}


// MUTATORS
// adds const to table
void ConstTable::addConst(Constant *constant) {
	table.emplace(constant->getConstName(), constant);
}

// removes all constants to the table (for testing)
void ConstTable::clearTable() {
	table.clear();
}


// GENERAL METHODS
// checks if const exists in table
bool ConstTable::contains(const string &constName) {
	ConstTable::ConstantTable::iterator tableIter;
	for(tableIter=table.begin(); tableIter!=table.end(); tableIter++) {
		if(tableIter->first == constName) {
			return true;
		}
	}

	return false;
}