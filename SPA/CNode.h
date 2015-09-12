#pragma once

#include <string>
#include <vector>

using namespace std;

enum CType {
	PROGRAM_, PROCEDURE_, WHILE_, IF_,
	ASSIGN_CALL_, DUMMY_
};

class CNode {

public:

	// constructor
	CNode(CType);

	// destructor
	~CNode();

	// getters
	CType getNodeType();
	int getStartStmt();
	int getEndStmt();
	CNode* getFirstChild();
	CNode* getSecondChild();
	CNode* getFirstParent();
	CNode* getSecondParent();

	// setters
	void setNodeType(CType);
	void setStartStmt(int);
	void setEndStmt(int);
	void linkFirstChild(CNode*);
	void linkSecondChild(CNode*);
	void linkFirstParent(CNode*);
	void linkSecondParent(CNode*);

	// checks
	bool isNodeType(CType);



protected:

	CType nodeType;
	int startStmt;
	int endStmt;

	// first child will be used to store the then child (if) or the direct child (assign/call/while)
	// second child will be used to store the else child (if) or the after-loop child (while)
	CNode* firstChild;
	CNode* secondChild;

	// first parent will be used to store the then parent (if) or the direct parent (assign/call/while)
	// second parent will be used to store the else parent (if) or the loop parent (while)
	CNode* firstParent;
	CNode* secondParent;

};
