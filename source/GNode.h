#pragma once

#include <string>
#include <vector>

using namespace std;

enum GType {
	PROC_, 
	WHILE_, 
	IF_,
	ASSIGN_, 
	CALL_, 
	DUMMY_, 
	END_
};

class GNode {

public:

	// constructor
	GNode(GType);

	// destructor
	~GNode();

	// getters
	GType getNodeType();
	string getName();
	int getStartStmt();
	int getEndStmt();
	vector<GNode*>& getParents();
	vector<GNode*>& getChildren();

	// setters
	void setNodeType(GType);
	void setName(string);
	void setStartStmt(int);
	void setEndStmt(int);
	void addChild(GNode*);
	void setFirstChild(GNode*);
	void setSecondChild(GNode*);
	void setFirstParent(GNode*);
	void setSecondParent(GNode*);
	void setChildren(vector<GNode*>);
	void setParents(vector<GNode*>);

	// checks
	bool isNodeType(GType);


protected:

	GType nodeType;
	int startStmt;
	int endStmt;
	string nodeName;

	// first element will be used to store the then parent (if) or the direct parent (assign/call/while)
	// second element will be used to store the else parent (if) or the loop parent (while)
	vector<GNode*> parents;

	// first element will be used to store the then child (if) or the direct child (assign/call/while)
	// second element will be used to store the else child (if) or the after-loop child (while)
	vector<GNode*> children;

};
