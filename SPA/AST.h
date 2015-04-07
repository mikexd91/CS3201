#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ProgNode.h"

using namespace std;

class AST {

public:

	~AST();

	// getters
	static AST* getInstance();
	bool contains(const string& procName);
	ProcNode* getProcNode(const string& procName);

	// setters
	void addProcNode(ProcNode *procNode); // adds the procedure to the program
	
	// clear
	static void reset();

private:
	// constructor: creates tree rooted at programNode (named programName)
	AST();

	
	ProgNode _root;
	static bool _hasInstance;
	static AST* _instance;

};