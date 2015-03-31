#include "VarTable.h"
#include <unordered_map>

// constructors
VarTable::VarTable() {

}

// general getters
VarTable* VarTable::getInstance() {
	if (_instance == NULL) {
		_instance = new VarTable();
	}
	return _instance;
}

Variable* VarTable::getVariable(const string& varName) {
	return this->_table.find(varName)->second;
}

VarTable::VarIter VarTable::getIterator() {
	VarTable::VarIter tableIter = _table.begin();

	return tableIter;
}

bool VarTable::contains(const string& varName) {
	return (this->_table.find(varName) != _table.end());
}