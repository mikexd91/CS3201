#include "VarTable.h"
#include <unordered_map>

using namespace std;

bool VarTable::_hasInstance = false;
VarTable* VarTable::_instance;
//VarTable::Table VarTable::_table;

// constructors
VarTable::VarTable() {
}

// general getters
VarTable* VarTable::getInstance() {
	if (!VarTable::_hasInstance) {
		VarTable::_instance = (new VarTable());
		VarTable::_hasInstance = true;
	}
	return VarTable::_instance;
}

Variable* VarTable::getVariable(const string& varName) {
	if (this->contains(varName)) {
		return this->_table.find(varName)->second;
	} else {
		return NULL;
	}
}

VarTable::VarIter VarTable::getIterator() {
	VarTable::VarIter tableIter = _table.begin();

	return tableIter;
}

// setters

void VarTable::addVariable(Variable *var) {
	string name = var->getName();
	this->_table.emplace(name, var);
}

bool VarTable::contains(const string& varName) {
	return (this->_table.find(varName) != _table.end());
}

// reset VarTable
void VarTable::reset() {
	VarTable::_hasInstance = false;
	delete _instance;
}
