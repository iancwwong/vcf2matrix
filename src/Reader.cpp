/**
 * Implementation of the Reader class 
*/

#include "Reader.h"

/* Constructor , destructor */
Reader::Reader() {
	this->numLines = 0;
}

Reader::~Reader() {}

/* Set up the input file */
void Reader::setInputFile(string inputFilenameStr) {
	this->inputFilename = inputFilenameStr;
}

/* Prepares monitor */
void Reader::setMonitor(Monitor * mon) {
	this->monitor = mon;
}

/* Returns number of SNP lines */
int Reader::getNumLines() {

	/* For now, ignore updating the number of SNP lines */
	return this->numLines;
}

/* Thread that reads the VCF file for parsing SNPs */
void Reader::executeParse() {
	string line;
	ifstream inputVCFFile(this->inputFilename);

	/* Check file opened properly */
	if (!inputVCFFile.is_open()) {
		cout << "Unable to open file." << endl;
		return;
	}

	/* For now, read file in LINES - an optimisation would be
	   to read in a certain number of bytes */
	while ( getline(inputVCFFile,line) ) {

		// /* Ignore all lines that start with '##' - most likely header info */
		// if (line.compare(0,2,"##") == 0) {
		// 	continue;
		// }

		// /* Ignore all lines that start with '#' - most likely vcf content info */
		// else if (line.compare(0,1,"#") == 0) {
		// 	/* Note: Information from this string is important, and should be considered in the future */
		// 	continue;
		// }

		// /* SNP info encountered - pass to monitor for parsing */
		// else {
		// 	this->monitor->addToParseData(line);
		// }

		/* Optimised reading by only checking lines that do NOT start with '#' */
		if (line.compare(0,1,"#") != 0) {
			this->monitor->addToParseData(line);
		}

	}

	inputVCFFile.close();

}