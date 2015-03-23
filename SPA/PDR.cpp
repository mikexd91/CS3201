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
};

void PDR::processProcedureStmt(ParsedData data) {
	//TODO - if working on a previous procedure, links the prev procedure to the root of the 
	//		 AST and creates a new procedure node
	if(stmtStack.top() == Type::PROGRAM) {
		stmtStack.pop();
	}
	AST.addProcNode(currentProcNode);

	/*
	 * TODO - Create the procedure node and set the necessary attributes
	 *
	 */
	TNode procedure = TNode(data.getProcName());
	currentProcNode = &procedure;
	currentNodeOnAST = &procedure;
}

void PDR::processAssignStmt(ParsedData data) {
	//TODO - Create an AST node for assignment statements
	TNode assignNode = TNode("assignment");	
}

void processIfStmt(ParsedData data) {
	//TODO - processing if stmtLst
}

void processWhileStmt() {
	//TODO - processing while stmtLst
}

