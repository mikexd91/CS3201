#pragma once

//#include "boost/algorithm/string.hpp"
#include "StmtTable.h"
#include "AssgNode.h"

using namespace std;
//using namespace boost;

class PatternAssg {

	/*

	ONLY FOR ASSIGNMENT STMTS

	1. Pattern ASS ( VAR, EXPR )

	Possible Pattern VAR
	1. _
	2. var

	Possible Pattern EXPR
	1. _
	2. _ int _
	3. _ int + int _
	4. _ var _
	5. _ var + var _

	*/


public:
	PatternAssg(int stmtNum);
	~PatternAssg();

	// pattern a(_, _)
	bool matchNothing();

	// pattern a("x", _)
	bool matchVar(string var);

	// pattern a(_, _"x y +"_)
	

	// pattern a("x", _"x y +"_);
	bool match(string var, string expr);

	
private:
	bool matchExpr(AssgNode* node, string expr);

	int _stmtNum;





};

