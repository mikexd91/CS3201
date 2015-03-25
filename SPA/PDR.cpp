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
		//case ParsedData::Type::CALL:
		//	processCallStmt(data);
		//	break;
		//case ParsedData::Type::WHILE:
		//	processWhileStmt(data);
		//	break;
		//case ParsedData::Type::IF:
		//	processIfStmt(data);
		//	break;

	}
}
 
void PDR::processProcedureStmt(ParsedData data) {
	//TODO - if working on a previous procedure, links the prev procedure to the root of the 
	//		 AST and creates a new procedure node
	if(stmtStack.top() == Type::PROCEDURE) {
		stmtStack.pop();
		nodeStack.pop();
	}

	currNestingLevel = data.getNestingLevel();
	ProcNode procedure = ProcNode(data.getProcName());
	StmtLstNode stmtLst = StmtLstNode();
	
	procedure.addChild(&stmtLst);

	nodeStack.push(procedure);
	nodeStack.push(stmtLst);
	currNestingLevel++;
	
	//TODO - Add to proc table
}

void PDR::processAssignStmt(ParsedData data) {
	AssgNode assignNode = AssgNode(++stmtCounter);	
	
	//means that the previous conditional statement has closed
	//pop previous statementLst
	if(currNestingLevel > data.getNestingLevel()) {
		int diffNestingLevel = currNestingLevel - data.getNestingLevel();
		currNestingLevel = data.getNestingLevel();
		
		for(int i = 0; i < diffNestingLevel; i++) {
			nodeStack.pop();
		}
	}

	/* TODO - Get modifies variable
	 *	    - Add to modifies table
	 *      - Get used variables
	 *      - Add to uses table
	 *      - Depending on the operators of the expression, push
	 *        to stack
	 */
	
	TNode parent = nodeStack.top();
	parent.linkChild(&assignNode);

	/*
	 * TODO - Create Statement object
	 *			- set the uses
	 *			- set the follows
	 *			- set the parent
	 *			- set the child
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
	
	WhileNode whileNode = WhileNode(++stmtCounter);
	StmtLstNode stmtLst = StmtLstNode();
	TNode parentNode = nodeStack.top();
	whileNode.linkParent(&parentNode);
	whileNode.linkChild(&stmtLst);

	nodeStack.push(stmtLst);
	currNestingLevel++;
};

void PDR::processCallStmt(ParsedData data) {
	CallNode callNode = CallNode(++stmtCounter, data.getProcName());
};