/**
 * Implementation of the SNPReader class 
*/
#include <fstream>
#include <iostream> /* Print error messages */

#include "SNPReader.h"

/* Constructor , destructor */
SNPReader::SNPReader() {
	this->toParse = new vector<string>;
}

SNPReader::~SNPReader() {
	delete this->toParse;
}

/* Obtain toParse vector */
vector<string> * SNPReader::getToParse() {
	return this->toParse;
}

/* Read a given file for SNPs */
void SNPReader::readToParse(string inputFileName) {
	/* For now, single threaded read */
	string line;
	ifstream inputVCFFile(inputFileName);

	/* Check file opened properly */
	if (!inputVCFFile.is_open()) {
		cout << "Unable to open file." << endl;
		return;
	}

	/* For now, read file in LINES, and sequentially
	TODO: Research how this can be optimised */
	while ( getline(inputVCFFile,line) ) {

		/* Ignore all lines that start with '##' - most likely header info */
		if (line.compare(0,2,"##") == 0) {
			continue;
		}

		/* Pass the line as data to Monitor for processing */
		this->toParse->push_back(line);
	}

	inputVCFFile.close();
}