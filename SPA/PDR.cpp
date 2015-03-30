#include "PDR.h"

using namespace std;

bool PDR::instanceFlag = false;
PDR* PDR::pdrInstance = NULL;

PDR* PDR::getInstance() {
    if(!instanceFlag) {
        pdrInstance = new PDR();
        instanceFlag = true;
    }
    
    return pdrInstance;
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
    
    TNode* assignExpChild = breakDownAssignExpression(data);
    TNode* parent = nodeStack.top();
    parent->linkChild(assignNode);
    assignNode->linkExprNode(assignExpChild);
    assignNode->linkVarNode(new VarNode(data.getAssignVar()));
    
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
    
    StmtTable* stmtTable = StmtTable::getInstance();
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
    
    StmtTable* stmtTable = StmtTable::getInstance();
    stmtTable->addStmt(&whileStmt);
    
}

TNode* PDR::breakDownAssignExpression(ParsedData data) {
    // Assume expression to be the RPN of the variables and operators
    queue<string> expression;
    stack<string> rpnStack;
    stack<TNode*> rpnNodeStack;
    
    for(int i = 0; i < expression.size(); i++) {
        string exp = expression.front();
        expression.pop();
        
        if(exp == "+" || exp == "-" || exp == "*" || exp == "/") {
            OpNode* operat = new OpNode();
            TNode* right = rpnNodeStack.top();
            rpnNodeStack.pop();
            TNode* left = rpnNodeStack.top();
            rpnNodeStack.pop();
            operat->linkLeftNode(left);
            operat->linkRightNode(right);
            rpnNodeStack.push(operat);
        } else {
            if(isInteger(exp)) {
                ConstNode* constNode = new ConstNode(exp);
                rpnNodeStack.push(constNode);
            } else {
                VarNode* var = new VarNode(exp);
                rpnNodeStack.push(var);
            }
        }
    }
    
    TNode* result = NULL;
    while(!rpnNodeStack.empty()) {
        result = rpnNodeStack.top();
        rpnNodeStack.pop();
    }
    
    return result;
}

bool PDR::isInteger(string exp) {
    for(int i = 0; i < exp.length(); i++) {
        if(!isdigit(exp[i])) {
            return false;
        }
    }
    return true;
}

void PDR::processCallStmt(ParsedData data) {
}