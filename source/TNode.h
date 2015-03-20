#include <string>

using namespace std;

class TNode {

public:
	TNode(string nodeType); // constructor

	void setStmtNumber(int num); // for nodes that have stmt numbers
	void setValue(string value); // the value for the node, example procnode should have the value = procname
	void setParent(TNode *parent); // direct parent, not the relationship Parent
	void setChild(TNode *child);
	void setLeftSibling(TNode *left);
	void setRightSibling(TNode *right);

	string getNodeType();
	int getStmtNumber();
	TNode * getParent();
	TNode * getChild();
	TNode * getLeftSibling();
	TNode * getRightSibling();

private:
	string _nodeType;
	int _stmtNumber;
	string _value;
	TNode * _parent;
	TNode * _child;
	TNode * _left;
	TNode * _right;

};