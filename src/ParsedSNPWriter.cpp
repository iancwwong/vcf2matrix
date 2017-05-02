/**
 * This is the implementation of the ParsedSNPWriter class
**/
#include <fstream>
#include <iostream>	/* To print error messages */

#include "ParsedSNPWriter.h"

/* Constructor, destructor */
ParsedSNPWriter::ParsedSNPWriter() {}

ParsedSNPWriter::~ParsedSNPWriter() {}

/* Given parsed SNP's, write to a specific file with
	given file name */
void writeParsedSNP(vector<ParsedSNP> * toWrite, string filename) {
	
	/* For now, single threaded write */
	

}
