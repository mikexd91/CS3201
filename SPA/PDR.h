#include <stack>
#include <string>
#include "ParsedData.h"
#include "TNode.h"
#include "Statement.h"
#include "AST.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:
	enum Type {ASSIGNMENT, PROCEDURE, PROGRAM};
	int stmtCounter;
	stack<Type> stmtStack;

	TNode * currentProcNode;
	TNode * currentNodeOnAST;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processWhileStmt();

};