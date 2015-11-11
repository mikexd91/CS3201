#include "PKB.h"
#include "PDR.h"
#include "CFGBuilder.h"
#include "CFGbipBuilder.h"

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
	CFGBuilder* cfgBuilder = CFGBuilder::getInstance();
	CFGbipBuilder* cfgBipBuilder = CFGbipBuilder::getInstance();

	pdr->processParsedData(data);
	cfgBuilder->processParsedData(data);
	cfgBipBuilder->processParsedData(data);
}