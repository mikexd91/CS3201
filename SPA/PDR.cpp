#include "PDR.h"

using namespace std;

PDR::PDR() {
    stmtCounter = 0;
    currNestingLevel = 0;
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
        case ParsedData::Type::CALL:
            processCallStmt(data);
            break;
        case ParsedData::Type::WHILE:
            processWhileStmt(data);
            break;
        case ParsedData::Type::IF:
            processIfStmt(data);
            break;
        default:
            break;
    }
}

void PDR::processProcedureStmt(ParsedData data) {
    //TODO - if working on a previous procedure, links the prev procedure to the root of the
    //		 AST and creates a new procedure node
    
    // New Procedure
    if(data.getNestingLevel() == 0) {
        for(int i = 0; i < currNestingLevel - data.getNestingLevel(); i++) {
            nodeStack.pop();
        }
        
        TNode procToBeAttached = nodeStack.top();
        // Link to program node of AST
        nodeStack.pop();
    }
    
    currNestingLevel = data.getNestingLevel();
    ProcNode procedure = ProcNode(data.getProcName());
    StmtLstNode stmtLst = StmtLstNode();
    
    procedure.addChild(&stmtLst);
    
    nodeStack.push(procedure);
    nodeStack.push(stmtLst);
    currNestingLevel++;
    
    // TODO - Add to proc table
    
}

void PDR::processAssignStmt(ParsedData data) {
    AssgNode assignNode = AssgNode(++stmtCounter);
    
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
    
}

void PDR::processWhileStmt(ParsedData data) {
    //TODO - processing while stmtLst
    if(currNestingLevel > data.getNestingLevel()) {
        int diffNestingLevel = currNestingLevel - data.getNestingLevel();
        currNestingLevel = data.getNestingLevel();
        
        for(int i = 0; i < diffNestingLevel; i++) {
            nodeStack.pop();
        }
    } else if(data.getNestingLevel() - currNestingLevel > 1) {
        // Throw error because nesting level cannot increase by more than 1
    }
    
    WhileNode whileNode = WhileNode(++stmtCounter);
    StmtLstNode stmtLst = StmtLstNode();
    TNode parentNode = nodeStack.top();
    
    whileNode.linkParent(&parentNode);
    whileNode.linkChild(&stmtLst);
    
    nodeStack.push(stmtLst);
    stmtNumStack.push(stmtCounter);
    currNestingLevel++;
    
    Statement whileStmt = Statement();
    whileStmt.setType(whileNode.getNodeType());
    whileStmt.setStmtNum(whileNode.getStmtNum());
    whileStmt.setTNodeRef(&whileNode);
    
    StmtTable* stmtTable = StmtTable.getInstance();
    stmtTable->addStmt(&whileStmt);
    
}

void PDR::processCallStmt(ParsedData data) {
}