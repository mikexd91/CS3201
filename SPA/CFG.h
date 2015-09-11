#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

class CFG {

public:

	~CFG();

	static CFG* getInstance();
	
private:
	CFG();

	static bool hasInstance;
	static CFG* instance;

}