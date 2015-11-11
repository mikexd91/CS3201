#pragma once

#include "GNode.h"
#include <boost/unordered_set.hpp>

using boost::unordered_set;

class EndGNode : public GNode {

public:

	EndGNode();

	// getters
	GNode* getParent();
	GNode* getProcNode();
	unordered_set<int> getPrevStmts();

	// setters
	void setParent(GNode*);
	void setProcNode(GNode*);
	void setPrevStmts(unordered_set<int>);

private:

	GNode* procNode;
	unordered_set<int> prevStmts;
};
