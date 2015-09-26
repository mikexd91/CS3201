#include "PKB.h"

bool PKB::instanceFlag = false;				// instance flag

// ACCESSORS
// gets instance of PKB
const PKB& PKB::getInstance() {
	// if instance does not exist, create and return instance
	if(!instanceFlag) {
		instanceFlag = true;
	}

	return *this;
}

