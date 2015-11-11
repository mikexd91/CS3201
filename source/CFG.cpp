#pragma once

#include "CFG.h"

using namespace std;

bool CFG::hasInstance = false;
CFG* CFG::instance;

CFG::CFG() {

}

CFG::~CFG() {

}

CFG* CFG::getInstance() {
	if(!hasInstance) {
		hasInstance = true;
		instance = new CFG();
	}

	return instance;
}

void CFG::addProcedure(ProcGNode* proc) {
	this->procedures.push_back(proc);
}

vector<ProcGNode*> CFG::getAllProcedures() {
	return this->procedures;
}

ProcGNode* CFG::getProcedure(string procName) {
	if(procedures.empty()) {
		return NULL;
	}

	for(size_t i = 0; i < procedures.size(); i++) {
		ProcGNode* proc = procedures.at(i);
		if(proc->getName() == procName) {
			return proc;
		}
	}

	return NULL;

}

bool CFG::hasProcedure(string procName) {
	if(procedures.empty()) {
		return false;
	}

	for(size_t i = 0; i < procedures.size(); i++) {
		GNode* proc = procedures.at(i);
		if(proc->getName() == procName) {
			return true;
		}
	}

	return false;
}

void CFG::reset() {
	CFG::hasInstance = false;
	delete instance;
}
