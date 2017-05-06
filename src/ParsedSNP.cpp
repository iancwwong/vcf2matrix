/**
 * Implementation for ParsedSNP
**/

#include "ParsedSNP.h"

/* Constructor, destructor */
ParsedSNP::ParsedSNP() {
	this->chromosomeLoc = "";
	this->pos = "";
	this->parsed = "";
	this->numSamples = 0;
}

ParsedSNP::~ParsedSNP() {}