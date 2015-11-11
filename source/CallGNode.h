/*
 * CallGNode.h
 *
 *  Created on: 5 Oct, 2015
 *      Author: Leon
 */
#pragma once
#include "GNode.h"
#include "ProcGNode.h"

class CallGNode : public GNode {

public:

	// constructor
	CallGNode(int);

	// getters
	ProcGNode* getProcChild();
	GNode* getChild();
	GNode* getParent();

	// setters
	void setCallChild(ProcGNode*);
	void setChild(GNode*);
	void setParent(GNode*);
};
