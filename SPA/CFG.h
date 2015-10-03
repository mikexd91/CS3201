#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "CNode.h"

class CFG {

public:

	~CFG();

	static CFG* getInstance();

	void addProcedure(CNode*);
	vector<CNode*> getAllProcedures();
	CNode* getProcedure(string);
	bool hasProcedure(string);

	//clear
	static void reset();
	
private:
	CFG();

	static bool hasInstance;
	static CFG* instance;

	vector<CNode*> procedures;

};
