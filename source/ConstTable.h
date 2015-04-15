#pragma once

#include <set>
#include "boost/unordered_map.hpp"
#include "Constant.h"

class ConstTable {
	/*

	ConstTable Implementation:
	---------------------------------
	|_[key]______|__________________|
	| const name | const object ref	|
	|_[key]______|__________________|
	---------------------------------

	*/

	typedef boost::unordered_map<string, Constant*> ConstantTable;

public:
	// ACCESSORS
	static ConstTable* getInstance();			// gets instance of const table

	ConstantTable::iterator getIterator();		// gets iterator to const table
	ConstantTable::iterator getEnd();			// gets iterator to end of const table

	Constant* getConst(const string &name);		// gets const by name
	Constant* getConst(int value);				// gets const by value
	vector<Constant*> getAllConst();			// gets all const objects
	vector<string> getAllConstNames();			// gets all const names
	vector<int> getAllConstValues();			// gets all const values

	// MUTATORS
	void addConst(Constant *constant);			// adds const to table
	void clearTable();							// removes all constants to the table (for testing)

	// GENERAL METHODS
	bool contains(const string &constName);		// checks if const exists in table

private:
	ConstantTable table;

	// SINGLETON
	static bool instanceFlag;
	static ConstTable* tableInstance;

	// SINGLETON CONSTRUCTOR
	ConstTable();

};