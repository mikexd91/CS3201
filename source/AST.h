#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "TNode.h"

using namespace std;
typedef short PROC;

class AST {

public:
	AST(); // constructor: should automatically set the root

	TNode * getRoot(); // return root of tree
	TNode * createTNode(string nodetype); // declare a nodetype class soon

	TNode * setFirstChild(TNode parent, TNode child); // set child as first child of parent
	TNode * setRightSibling(TNode left, TNode right); // set right as right sibling of left

private:
	TNode *_root;

};