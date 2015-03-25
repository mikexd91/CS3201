#include <stack>
#include <string>
#include "ParsedData.h"
#include "ProcNode.h"
#include "AssgNode.h"
#include "Statement.h"
#include "AST.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:
	enum Type {ASSIGNMENT, PROCEDURE, PROGRAM, OPERATOR, WHI};
	int stmtCounter;
	stack<Type> stmtStack;
	stack<TNode> nodeStack;

	TNode previousFollowedByNode;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processCallStmt(ParsedData);

};