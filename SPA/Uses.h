#pragma once
#include <string>
#include <set>
#include "VarTable.h"
#include "StmtTable.h"

using namespace std;

class Uses
{
public:
	Uses(void);
	~Uses(void);

	bool isUses(int stmt, string varName);			// return boolean of Uses(a,v)

	set<int> getAssignOfUses(string varName);		// get a of Uses(a,"v")
	set<string> getVarOfUses(int stmtNum);			// get v of Uses(a, v)
	set<int> getAssignOfUsesUnfixed(set<int> assign, set<string> var0);
	set<string> getVarOfUsesUnfixed(set<int> assign, set<string> var0);

	// set<set<int>> getAllUses(int stmt1, int stmt2);
};

