#include "VarTable.h"
#include <unordered_map>

using namespace std;

VarTable* VarTable::_instance = NULL;

// constructors
VarTable::VarTable() {

}

// general getters
VarTable* VarTable::getInstance() {
	if (VarTable::_instance == NULL) {
		VarTable::_instance = new VarTable();
	}
	return VarTable::_instance;
}

Variable* VarTable::getVariable(const string& varName) {
	return this->_table.find(varName)->second;
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