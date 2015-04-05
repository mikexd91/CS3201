#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "AST.h"

bool AST::_hasInstance = false;
AST AST::_instance;
ProgNode AST::_root;

// constructors

AST::AST() {
}

//------------------

// getters

AST& AST::getInstance() {
	if (!AST::_hasInstance) {
		AST::_instance = *(new AST());
		AST::_hasInstance = true;
	}
	return _instance;
}

bool AST::contains(const string& procName) {
	return this->_root.contains(procName);
}

ProcNode* AST::getProcNode(const string& procName) {
	return this->_root.getProcNode(procName);
}

//------------------

// setters
void AST::addProcNode(ProcNode *procNode) {
	_root.linkProcNode(procNode);
}