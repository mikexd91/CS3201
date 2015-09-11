#pragma once

#include "CFG.h"

using namespace std;

bool CFG::hasInstance = false;
CFG* CFG::instance;

CFG::CFG() {

}

CFG::~CFG() {

}

CFG* CFG::getInstance() {
	if(!hasInstance) {
		hasInstance = true;
		instance = new CFG();
	}

	return instance;
}

