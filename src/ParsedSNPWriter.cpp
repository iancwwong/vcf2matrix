/**
 * This is the implementation of the ParsedSNPWriter class
**/
#include <iostream>	/* To print error messages */
#include <thread>	/* multithreaded writing */

#include "ParsedSNPWriter.h"

/* Constructor, destructor */
ParsedSNPWriter::ParsedSNPWriter() {}

ParsedSNPWriter::~ParsedSNPWriter() {}

/* Given parsed SNP's, write to a specific file with
	given file name */
void ParsedSNPWriter::writeParsedSNP(vector<ParsedSNP *> * toWrite, string filename) {
	
	/* Create the files - one for location, one for matrix */
	string outputfn = filename.substr(0, filename.find_last_of(".")); /* remove file extension component */
	ofstream locFile(outputfn + "_locations.loc");
	if (!locFile.is_open()) {
		cout << "Unable to create a locations file." << endl;
		return;
	}
	ofstream matrixFile(outputfn + "_matrix.matrix");
	if (!matrixFile.is_open()) {
		cout << "Unable to create a matrix file." << endl;
		return;
	}
	
	/* Create and run the threads that will write to the files */
	vector<thread *> threads;
	int numThreads = 4;		/* For now, it's a fixed number */
	
	/* For determining toWrite access index values for each thread */
	int baseNumItems = toWrite->size() / numThreads;	
	int overflow = toWrite->size() % numThreads;
	int currIndex = 0;

	for (int i = 0; i < numThreads; i++) {

		/* Determine the index values */
		int threadNumItems = baseNumItems;
		if (overflow) {
			threadNumItems++;
			overflow--;
		}
		int lowerLimit = currIndex;
		int upperLimit = currIndex + threadNumItems - 1;

		/* Create the thread */
		thread * wThread = new thread(&ParsedSNPWriter::writeThread, ParsedSNPWriter(),
									&locFile, &matrixFile, toWrite,
									lowerLimit, upperLimit);
		threads.push_back(wThread);

		/* Update currIndex */
		currIndex += threadNumItems;
	}

	/* Wait for all threads to terminate before cleaning up */
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}

	/* Close file handlers */
	locFile.close();
	matrixFile.close();
}

/* Write thread */
void ParsedSNPWriter::writeThread(ofstream * locFile, ofstream * matrixFile, 
                        vector<ParsedSNP *> * toWrite, int lowLimit, int upLimit) {
	
	/* Loop through index limits, write accordingly */
	for (int i = lowLimit; i <= upLimit; i++) {
		ParsedSNP * pSNP = (*toWrite)[i];
		*locFile << pSNP->chromosomeLoc << "," << pSNP->pos << endl;
		*matrixFile << pSNP->parsed << endl;
	}
}
