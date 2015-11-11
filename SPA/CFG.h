#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "ProcGNode.h"

class CFG {

public:

	~CFG();

	static CFG* getInstance();

	void addProcedure(ProcGNode*);
	vector<ProcGNode*> getAllProcedures();
	ProcGNode* getProcedure(string);
	bool hasProcedure(string);

	//clear
	static void reset();
	
private:
	CFG();

	static bool hasInstance;
	static CFG* instance;

	vector<ProcGNode*> procedures;

};
