
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
#include "IfNode.h"
#include "CallNode.h"
#include "AST.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:
	enum Type {ASSIGNMENT, PROCEDURE, PROGRAM, OPERATOR, WHI};
	int stmtCounter;
	int currNestingLevel;

	stack<Type> stmtStack;
	stack<TNode> nodeStack;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processCallStmt(ParsedData);

};
#endif
