#include "NoSynInsert.h"

NoSynInsert::NoSynInsert() {
}

void NoSynInsert::setPass(bool success) {
	pass = success;
}


bool NoSynInsert::execute(ResultTable& resultTable) {
	return pass;
}



