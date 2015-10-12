#include "VarTable.h"
#include <boost/foreach.hpp>

bool VarTable::_hasInstance = false;
VarTable* VarTable::_instance;

// constructors
VarTable::VarTable() {}

VarTable::~VarTable() {}

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

unordered_map<string, Variable*>::iterator VarTable::getIterator() {
	boost::unordered_map<string, Variable*>::iterator tableIter = _table.begin();

	return tableIter;
}

unordered_map<string, Variable*>::iterator VarTable::getEnd() {
	boost::unordered_map<string, Variable*>::iterator tableIter = _table.end();

	return tableIter;
}

const unordered_set<string>& VarTable::getAllVarNames() {
	unordered_set<string>* allVarNames = new unordered_set<string>;
	BOOST_FOREACH(auto p, _table) {
		allVarNames->emplace(p.first);
	}
	return *allVarNames;
}

const unordered_set<Variable*>& VarTable::getAllVariables() {
	unordered_set<Variable*>* allVars = new unordered_set<Variable*>;
	BOOST_FOREACH(auto p, _table) {
		allVars->emplace(p.second);
	}
	return *allVars;
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
	//VarTable::_hasInstance = false;
	//_instance->reset();
	_instance->_table.clear();
}
