/**
 * This file implements the Writer class
**/

#include "Writer.h"

/* Constructor, destructor */
Writer::Writer() {
	this->toWrite = new vector<ParsedSNP>;
	this->toWrite_lock = new mutex;
	this->prevProcComplete = false;
}

