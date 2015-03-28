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

    for(int i = 0; i < currNestingLevel - data.getNestingLevel(); i++) {
        nodeStack.pop();
    }
    
    if(!nodeStack.empty()) {
        TNode* previousProc = nodeStack.top();
        // TODO - link previous proc node to the program node
        nodeStack.pop();
    }

    ProcNode* procedure = new ProcNode(data.getProcName());
    StmtLstNode* stmtLst = new StmtLstNode();
    procedure->addChild(stmtLst);
    
    nodeStack.push(procedure);
    nodeStack.push(stmtLst);
    
    currNestingLevel = data.getNestingLevel();
    currNestingLevel++;
    
    // TODO - Add to proc table
    
}

void PDR::processAssignStmt(ParsedData data) {
    AssgNode* assignNode = new AssgNode(++stmtCounter);
    
    if(currNestingLevel > data.getNestingLevel()) {
        int diffNestingLevel = currNestingLevel - data.getNestingLevel();
        for(int i = 0; i < diffNestingLevel; i++) {
            nodeStack.pop();
        }
        currNestingLevel = data.getNestingLevel();
    }
    
    /* TODO - Get modifies variable
     *	    - Add to modifies table
     *      - Get used variables
     *      - Add to uses table
     *      - Depending on the operators of the expression, push
     *        to stack
     */
    
    TNode* parent = nodeStack.top();
    parent->linkChild(assignNode);
    
    /*
     * TODO - Create Statement object
     *			- set the uses
     *			- set the follows
     *			- set the parent
     *			- set the child
     */
    Statement* stmt = new Statement();
    stmt->setType(assignNode->getNodeType());
    stmt->setStmtNum(stmtCounter);
    stmt->setTNodeRef(assignNode);
    
    StmtTable* stmtTable = StmtTable.getInstance();
    stmtTable->addStmt(stmt);
    
}

void PDR::processIfStmt(ParsedData data) {
    
}

void PDR::processWhileStmt(ParsedData data) {
    //TODO - processing while stmtLst
    if(currNestingLevel > data.getNestingLevel()) {
        int diffNestingLevel = currNestingLevel - data.getNestingLevel();
        
        for(int i = 0; i < diffNestingLevel; i++) {
            nodeStack.pop();
        }
    } else if(data.getNestingLevel() - currNestingLevel > 1) {
        // Throw error because nesting level cannot increase by more than 1
    }
    
    WhileNode* whileNode = new WhileNode(++stmtCounter);
    StmtLstNode* stmtLst = new StmtLstNode();
    TNode* parentNode = nodeStack.top();
    
    whileNode->linkParent(parentNode);
    
    nodeStack.push(stmtLst);
    stmtParentNumStack.push(stmtCounter);
    currNestingLevel = data.getNestingLevel() + 1;
    
    /*
     * Creating statement object that will be used to populate the 
     * necessary tables
     */
    Statement whileStmt = Statement();
    whileStmt.setType(whileNode->getNodeType());
    whileStmt.setStmtNum(whileNode->getStmtNum());
    whileStmt.setTNodeRef(whileNode);
    
    StmtTable* stmtTable = StmtTable.getInstance();
    stmtTable->addStmt(&whileStmt);
    
}

void PDR::breakDownAssignExpression(ParsedData data) {
    // Assume expression to be the RPN of the variables and operators
    queue<string> expression;
    stack<string> rpnStack;
    
    for(int i = 0; i < expression.size(); i++) {
        string exp = expression.front();
        expression.pop();
        
    }
}

void PDR::processCallStmt(ParsedData data) {
}