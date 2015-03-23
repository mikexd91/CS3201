#include <stack>
#include <string>
#include "ParsedData.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:
	enum Type {ASSIGNMENT, PROCEDURE, PROGRAM};
	int stmtCounter;
	stack<Type> stmtStack;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	//void processWhileStmt();

};