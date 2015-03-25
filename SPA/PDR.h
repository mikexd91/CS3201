
#include <stack>

#ifndef PDR_HEADER
#define PDR_HEADER

#include <string>
#include "ParsedData.h"
#include "ProcNode.h"
#include "ProcTable.h"
#include "AssgNode.h"
#include "Statement.h"
#include "WhileNode.h"
#include "StmtLstNode.h"
#include "StmtTable.h"
#include "IfNode.h"
#include "CallNode.h"
#include "AST.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:
	enum Type {ASSIGNMENT, PROCEDURE, PROGRAM, OPERATOR, WHILE};
	int stmtCounter;
	int currNestingLevel;

	stack<TNode> nodeStack;
    stack<int> stmtNumStack;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processCallStmt(ParsedData);

};
#endif
