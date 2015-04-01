#include "VarTable.h"
#include <unordered_map>

using namespace std;

int VarTable::_hasInstance = 0;
VarTable* VarTable::_instance = NULL;

// constructors
VarTable::VarTable() {

}

// general getters
VarTable* VarTable::getInstance() {
	if (VarTable::_hasInstance != 1) {
		printf("asd\n");
		VarTable::_instance = new VarTable();
		VarTable::_hasInstance = 1;
	}
	printf("instance address: %p\n", VarTable::_instance);
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