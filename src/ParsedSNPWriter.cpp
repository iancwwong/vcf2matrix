/**
 * This is the implementation of the ParsedSNPWriter class
**/
#include <iostream>	/* To print error messages */
#include <thread>	/* multithreaded writing */

#include "ParsedSNPWriter.h"

/* Constructor, destructor */
ParsedSNPWriter::ParsedSNPWriter() {
	this->locFile = NULL;
	this->matrixFile = NULL;
}
ParsedSNPWriter::~ParsedSNPWriter() {
	delete this->locFile;
	delete this->matrixFile;
	this->locFile = NULL;
	this->matrixFile = NULL;
}

/* Create the files that contain parsed SNP info.
	* Namely: location, matrix
	*/
void ParsedSNPWriter::createSubFiles(string filename) {
	/* NOTE: Does NOT check whether file exists! If it does, it'l modify it in
	 * unpredicted ways!
	 */

	/* Obtain the file handlers */
	this->locFile = new ofstream(filename + ".loc");
	if (!this->locFile->is_open()) {
		cout << "Unable to create a locations file." << endl;
		return;
	}
	this->matrixFile = new ofstream(filename + ".matrix");
	if (!this->matrixFile->is_open()) {
		cout << "Unable to create a matrix file." << endl;
		return;
	}
}

/* Write a given parsed SNP into the opened files */
void ParsedSNPWriter::writeParsedSNP(ParsedSNP * toWrite) {
		cout << "Writing: " << endl;
		toWrite->showDetails();
		*this->locFile << toWrite->chromosomeLoc << "," << toWrite->pos << endl;
		*this->matrixFile << toWrite->parsed << endl;
}