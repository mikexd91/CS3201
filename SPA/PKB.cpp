#include "PKB.h"

bool PKB::instanceFlag = false;				// instance flag

// ACCESSORS
// gets instance of PKB
PKB* PKB::getInstance() {
	// if instance does not exist, create and return instance
	if(!instanceFlag) {
		pkbInstance = new PKB();
		instanceFlag = true;
	}

	return pkbInstance;
}

// pass parsed data to PDR
void PKB::processParsedData(ParsedData data) {
	PDR* pdr = PDR::getInstance();
	pdr->processParsedData(data);
}