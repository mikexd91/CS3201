#pragma once
#include <string>
#include <set>
#include "VarTable.h"
#include "StmtTable.h"

using namespace std;

class Modifies
{
public:
	Modifies(void);
	~Modifies(void);

	bool isModifies(int stmt, string varName);		// return boolean of Modifies(a,v)

	set<int> getAssignOfModifies(string varName);	// get a of Modifies(a,"v")
	set<string> getVarOfModifies(int stmtNum);		// get v of Modifies(6, v)
	set<int> getAssignOfModifiesUnfixed(set<int> assign, set<string> var0); // get a of Modifies(a,v)
	set<string> getVarOfModifiesUnfixed(set<int> assign, set<string> var0); // get v of Modifies(a,v)

	// set<set<int>> getAllModifies(int stmt1, int stmt2);
};

