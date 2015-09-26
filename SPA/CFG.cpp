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

void CFG::addProcedure(CNode* proc) {
	this->procedures.push_back(proc);
}

vector<CNode*> CFG::getAllProcedures() {
	return this->procedures;
}

CNode* CFG::getProcedure(string procName) {
	if(procedures.empty()) {
		return NULL;
	}

	for(int i = 0; i < procedures.size(); i++) {
		CNode* proc = procedures.at(i);
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

	for(int i = 0; i < procedures.size(); i++) {
		CNode* proc = procedures.at(i);
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