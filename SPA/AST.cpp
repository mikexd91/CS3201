#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "AST.h"

// constructors

AST::AST(const string &progName) {
	this->_rootNode = new ProgNode(progName);
}

//------------------

// getters

AST* AST::getInstance(const string &progName) {
	if (_instance == NULL) {
		_instance = new AST(progName);
	}
	return _instance;
}
	
//------------------

// setters
void AST::addProcNode(ProcNode *procNode) {
	_rootNode->linkProcNode(procNode);
}