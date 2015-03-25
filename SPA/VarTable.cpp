#include "VarTable.h"

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

const set<string>& VarTable::getAllVars() {
	return this->_allVarsTable;
}