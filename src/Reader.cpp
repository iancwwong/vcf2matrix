/**
 * Implementation of the Reader class 
*/

#include "Reader.h"

/* Constructor , destructor */
Reader::Reader() {
	this->toParse = new vector<string>;
}

Reader::~Reader() {
	delete this->toParse;
}

/* Obtain toParse vector */
vector<string> * Reader::getToParseVector() {
	return this->toParse;
}

/* Read a given file for SNPs */
void Reader::readToParse(string inputFileName) {
	/* For now, single threaded read */
	string line;
	ifstream inputVCFFile(this->inputFilename);

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