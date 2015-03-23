#ifndef PDR_HEADER
#define PDR_HEADER

#include <string>
#include <vector>
#include "ParsedData.h"

using namespace std;

class PDR {
	
public:	
	PDR(); //Constructor

	void processParsedData(ParsedData);

private:

	int stmtCounter;

	void processProcedureStmt(ParsedData);
	void processAssignStmt(ParsedData);
	//void processWhileStmt();

};
#endif