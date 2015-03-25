#include <string>
#include <vector>
#include <stack>
#include "PDR.h"

using namespace std;

PDR::PDR() {
	stmtCounter = 0;
	stmtStack.push(Type::PROGRAM);
}
 
void PDR::processParsedData(ParsedData data) {
	ParsedData::Type stmtType = data.getType();
	switch(stmtType) {
		case ParsedData::Type::PROCEDURE:
			processProcedureStmt(data);
			break;
		case ParsedData::Type::ASSIGNMENT:
			processAssignStmt(data);
			break;
	}
}
void PDR::processProcedureStmt(ParsedData data) {
	//TODO - if working on a previous procedure, links the prev procedure to the root of the 
	//		 AST and creates a new procedure node
	if(stmtStack.top() == Type::PROCEDURE) {
		stmtStack.pop();
		nodeStack.pop();
	}

	ProcNode procedure = ProcNode();
	nodeStack.push(procedure);
	//TODO - Get the necessary details and create the statement object
}

void PDR::processAssignStmt(ParsedData data) {
	stmtCounter++;
	
	AssgNode assignNode = AssgNode();	
	/* TODO - Get modifies variable
	 *	    - Add to modifies table
	 *      - Get used variables
	 *      - Add to uses table
	 *      - Depending on the operators of the expression, push
	 *        to stack
	 */

	TNode parent = nodeStack.top();
	//parent.link(AssgNode);

	/*
	 * TODO - Create Statement object
	 *		- set the uses
	 *		- set the follows
	 *		- set the parent
	 *		- set the child
	 */
	Statement stmt = Statement();
	//stmt.setType(Statement::NodeType::ASSIGN);

}

void PDR::processIfStmt(ParsedData data) {
	//TODO - processing if stmtLst
	stmtCounter++;
}

void PDR::processWhileStmt(ParsedData data) {
	//TODO - processing while stmtLst
	stmtCounter++;
}

void PDR::processCallStmt(ParsedData data) {
	stmtCounter++;
}

