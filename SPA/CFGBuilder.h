#include <stack>
#include <boost/unordered_set.hpp>
#include <string>

#include "AssgCallCNode.h"
#include "CFG.h"
#include "CNode.h"
#include "DummyCNode.h"
#include "EndCNode.h"
#include "IfCNode.h"
#include "ParsedData.h"
#include "ProcCNode.h"
#include "Procedure.h"
#include "ProcTable.h"
#include "Statement.h"
#include "StmtTable.h"
#include "WhileCNode.h"

using namespace std;
using boost::unordered_set;

class CFGBuilder {

public:
	enum PrevState {IF, ELSE, WHILE, PROCEDURE, ASSGCALL, NONE};

	static CFGBuilder* getInstance();
	static void resetInstanceFlag();

	void processParsedData(ParsedData);

	// for testing
	string getCurrProc();
	CNode* getHead();
	int getStmtCounter();
	int getNestingLevel();
	PrevState getPrevState();
	stack<CNode*> getNestingStack();
	stack<CNode*> getIfStack();

private:

	static CFGBuilder* cfgBuilderInstance;
	static bool instanceFlag;
	int stmtCounter;
	int currNestingLevel;
	PrevState prevState;

	CNode* currentProcedure;
	CNode* head;

	stack<CNode*> nestingStack;
	stack<CNode*> ifStack;

	void processProcStmt(ParsedData);
	void processAssgCallStmt(ParsedData);
	void processIfStmt(ParsedData);
	void processElseStmt(ParsedData);
	void processWhileStmt(ParsedData);
	void processEndProgram(ParsedData);

	void addNextAndPrev(CNode*, CNode*);
	void addNextAndPrev(int, int);
	void setRecoverChild(CNode*, CNode*);
	void setRecoverParent(CNode*, CNode*);

	CFGBuilder();
};
