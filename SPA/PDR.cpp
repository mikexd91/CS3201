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
        case ParsedData::PROCEDURE:
            processProcedureStmt(data);
            break;
        case ParsedData::ASSIGNMENT:
            processAssignStmt(data);
            break;
        case ParsedData::WHILE:
            processWhileStmt(data);
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
    if(currNestingLevel > data.getNestingLevel()) {
        int diffNestingLevel = currNestingLevel - data.getNestingLevel();
        for(int i = 0; i < diffNestingLevel; i++) {
            stmtParentNumStack.pop();
            nodeStack.pop();
        }
        currNestingLevel = data.getNestingLevel();
    }
    
    AssgNode* assignNode = new AssgNode(++stmtCounter);
    TNode* assignExpChild = breakDownAssignExpression(data);
    TNode* parentStmtLst = nodeStack.top();
    
    if(parentStmtLst->hasChildren()) {
        vector<TNode*> listOfChildren = parentStmtLst->getChildren();
        long int lastChild = listOfChildren.size() - 1;
        TNode* leftSibling = listOfChildren[lastChild];
        assignNode->linkLeftSibling(leftSibling);
    }
    
    assignNode->linkParent(parentStmtLst);
    assignNode->linkExprNode(assignExpChild);
    assignNode->linkVarNode(new VarNode(data.getAssignVar()));
    
    Statement* stmt = new Statement();
    stmt->setType(assignNode->getNodeType());
    stmt->setStmtNum(stmtCounter);
    stmt->setTNodeRef(assignNode);
    
    if(!stmtParentNumStack.empty()) {
        int parentStmtNum = stmtParentNumStack.top();
        stmt->setChildOf(parentStmtNum);
    }
    
    StmtTable* stmtTable = StmtTable::getInstance();
    stmtTable->addStmt(stmt);
}

void PDR::processWhileStmt(ParsedData data) {
    if(currNestingLevel > data.getNestingLevel()) {
        int diffNestingLevel = currNestingLevel - data.getNestingLevel();
        for(int i = 0; i < diffNestingLevel; i++) {
            stmtParentNumStack.pop();
            nodeStack.pop();
        }
    } else if(data.getNestingLevel() - currNestingLevel > 1) {
        // Throw error because nesting level cannot increase by more than 1
    }
    
    WhileNode* whileNode = new WhileNode(++stmtCounter);
    StmtLstNode* stmtLst = new StmtLstNode();
    TNode* parentStmtLst = nodeStack.top();
    
    if(parentStmtLst->hasChildren()) {
        vector<TNode*> listOfChildren = parentStmtLst->getChildren();
        long int lastChild = listOfChildren.size() - 1;
        TNode* leftSibling = listOfChildren[lastChild];
        whileNode->linkLeftSibling(leftSibling);
    }
    
    whileNode->linkParent(parentStmtLst);
    whileNode->linkVarNode(new VarNode(data.getWhileVar()));
    whileNode->linkStmtLstNode(stmtLst);
    
    stmtParentNumStack.push(stmtCounter);
    nodeStack.push(stmtLst);
    stmtParentNumStack.push(stmtCounter);
    currNestingLevel = data.getNestingLevel() + 1;
    
    Statement* whileStmt = new Statement();
    whileStmt->setType(whileNode->getNodeType());
    whileStmt->setStmtNum(whileNode->getStmtNum());
    whileStmt->setTNodeRef(whileNode);
    
    if(!stmtParentNumStack.empty()) {
        int parentStmtNum = stmtParentNumStack.top();
        whileStmt->setChildOf(parentStmtNum);
    }
    
    StmtTable* stmtTable = StmtTable::getInstance();
    stmtTable->addStmt(whileStmt);
}

TNode* PDR::breakDownAssignExpression(ParsedData data) {
    // Assume expression to be the RPN of the variables and operators
    queue<string> expression;
    stack<string> rpnStack;
    stack<TNode*> rpnNodeStack;
    
    for(size_t i = 0; i < expression.size(); i++) {
        string exp = expression.front();
        expression.pop();
        
        if(exp == "+" || exp == "-" || exp == "*" || exp == "/") {
            OpNode* operat = new OpNode(exp);
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
    for(size_t i = 0; i < exp.length(); i++) {
        if(!isdigit(exp[i])) {
            return false;
        }
    }
    return true;
}