/*
 * CFGbipBuilder.h
 *
 *  Created on: 20 Oct, 2015
 *      Author: Leon
 */

#include <stack>
#include <boost/unordered_set.hpp>
#include <string>

#include "AssgGNode.h"
#include "CallGNode.h"
#include "CFGbip.h"
#include "DesignExtractor.h"
#include "GNode.h"
#include "DummyGNode.h"
#include "EndGNode.h"
#include "IfGNode.h"
#include "ParsedData.h"
#include "ProcGNode.h"
#include "Procedure.h"
#include "ProcTable.h"
#include "Statement.h"
#include "StmtTable.h"
#include "WhileGNode.h"

using namespace std;
using boost::unordered_set;

class CFGbipBuilder {

public:
	enum PrevState {IF, ELSE, WHILE, PROCEDURE, ASSG, CALL, NONE};

	static CFGbipBuilder* getInstance();
	static void resetInstanceFlag();

	void processParsedData(ParsedData);

	// for testing
	string getCurrProc();
	GNode* getHead();
	int getStmtCounter();
	int getNestingLevel();
	PrevState getPrevState();
	stack<GNode*> getNestingStack();
	stack<GNode*> getIfStack();

private:

	static CFGbipBuilder* cfgBipBuilderInstance;
	static bool instanceFlag;
	int stmtCounter;
	int currNestingLevel;
	PrevState prevState;

	ProcGNode* currentProcedure;
	GNode* head;

	stack<GNode*> nestingStack;
	stack<GNode*> ifStack;

	void processProcStmt(ParsedData);
	void processAssgStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processElseStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processCallStmt(ParsedData);
	void processEndProgram(ParsedData);

	void addNextAndPrev(GNode*, GNode*);
	void addNextAndPrev(int, int);
	void setRecoverChild(GNode*, GNode*);
	void setRecoverParent(GNode*, GNode*);

	void setStatementReference(GNode*);
	void setProcReference(ProcGNode*);
	void setDummyIfNestingRecovery();
	void setWhileNestingRecovery();
	void setNodeSameRecoveryIfNest(GNode*);
	void setEndProcOrEndNodeIfNest(EndGNode*);
	void checkPrevStateAndSetProperties(GNode*);

	CFGbipBuilder();
};