/*
 * CFGbip.cpp
 *
 *  Created on: 20 Oct, 2015
 *      Author: Leon
 */

#pragma once

#include "CFGbip.h"

using namespace std;

bool CFGbip::hasInstance = false;
CFGbip* CFGbip::instance;

CFGbip::CFGbip() {

}

CFGbip::~CFGbip() {

}

CFGbip* CFGbip::getInstance() {
	if(!hasInstance) {
		hasInstance = true;
		instance = new CFGbip();
	}

	return instance;
}

void CFGbip::addProcedure(ProcGNode* proc) {
	this->procedures.push_back(proc);
}

vector<ProcGNode*> CFGbip::getAllProcedures() {
	return this->procedures;
}

ProcGNode* CFGbip::getProcedure(string procName) {
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

bool CFGbip::hasProcedure(string procName) {
	if(procedures.empty()) {
		return false;
	}

	for(size_t i = 0; i < procedures.size(); i++) {
		ProcGNode* proc = procedures.at(i);
		if(proc->getName() == procName) {
			return true;
		}
	}

	return false;
}

void CFGbip::reset() {
	CFGbip::hasInstance = false;
	delete instance;
}