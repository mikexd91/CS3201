#include "PKB.h"
#include "PDR.h"
#include "CFGBuilder.h"

bool PKB::instanceFlag = false;				// instance flag
PKB* PKB::pkbInstance = NULL;				// table instance

// ACCESSORS
// empty constructor
PKB::PKB() {}

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
	CFGBuilder* cfg = CFGBuilder::getInstance();

	pdr->processParsedData(data);
	cfg->processParsedData(data);
}