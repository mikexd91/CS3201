
#include <stack>
#include <queue>

#ifndef PDR_HEADER
#define PDR_HEADER

#include <string>
#include "ConstNode.h"
#include "ParsedData.h"
#include "ProcNode.h"
#include "ProcTable.h"
#include "AssgNode.h"
#include "StmtTable.h"
#include "Statement.h"
#include "WhileNode.h"
#include "StmtLstNode.h"
#include "IfNode.h"
#include "CallNode.h"
#include "OpNode.h"
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

	stack<int> stmtParentNumStack;
	stack<TNode*> nodeStack;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processCallStmt(ParsedData);
    TNode* breakDownAssignExpression(ParsedData);
    
    bool isInteger(string);

};
#endif
