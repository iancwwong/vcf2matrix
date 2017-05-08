/**
 * Implementation for ParsedSNP
**/
#include <iostream>		/* Print debugging statements */

#include "ParsedSNP.h"

/* Constructor, destructor */
ParsedSNP::ParsedSNP() {
	this->chromosomeLoc = "";
	this->pos = "";
	this->parsed = "";
}

ParsedSNP::~ParsedSNP() {}

/* Debugging */
void ParsedSNP::showDetails() {
	cout << "PSNP: Chrom = " << this->chromosomeLoc
		<< ", Pos = " << this->pos << ", Parsed: " << this->parsed << endl;
}