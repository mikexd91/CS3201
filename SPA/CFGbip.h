/*
 * CFGbip.h
 *
 *  Created on: 20 Oct, 2015
 *      Author: Leon
 */

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ProcGNode.h"

class CFGbip {

public:

	~CFGbip();

	static CFGbip* getInstance();

	void addProcedure(ProcGNode*);
	vector<ProcGNode*> getAllProcedures();
	ProcGNode* getProcedure(string);
	bool hasProcedure(string);

	//clear
	static void reset();

private:
	
	CFGbip();

	static bool hasInstance;
	static CFGbip* instance;

	vector<ProcGNode*> procedures;

};