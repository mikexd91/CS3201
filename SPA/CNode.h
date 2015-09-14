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
	string getName();
	int getStartStmt();
	int getEndStmt();
	vector<CNode*>& getParents();
	vector<CNode*>& getChildren();

	// setters
	void setNodeType(CType);
	void setName(string);
	void setStartStmt(int);
	void setEndStmt(int);
	void addChild(CNode*);
	void setFirstChild(CNode*);
	void setSecondChild(CNode*);
	void setFirstParent(CNode*);
	void setSecondParent(CNode*);

	// checks
	bool isNodeType(CType);


protected:

	CType nodeType;
	int startStmt;
	int endStmt;
	string nodeName;

	// first element will be used to store the then parent (if) or the direct parent (assign/call/while)
	// second element will be used to store the else parent (if) or the loop parent (while)
	vector<CNode*> parents;

	// first element will be used to store the then child (if) or the direct child (assign/call/while)
	// second element will be used to store the else child (if) or the after-loop child (while)
	vector<CNode*> children;

};
