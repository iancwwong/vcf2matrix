/**
 * Provides implementation for the SNPParser class
**/
#include <thread>
#include <iostream>		/* for printing debug statements */

#include "SNPParser.h"
#include "Converter.h"

/* Constructor, destructor */
SNPParser::SNPParser() {
	this->toWrite = new vector<ParsedSNP *>;
};

SNPParser::~SNPParser() {
	delete this->toWrite;
};

/* Return the parse SNPs */
vector<ParsedSNP *> * SNPParser::getToWrite() {
	return this->toWrite;
}

/* Parse the given SNPs */
void SNPParser::parseSNPs(vector<string> * toParse, int alleleFreq, int confScore) {

	/* Create and run the threads that will parse data */
	
	int numThreads = 4;	/* For now, it's a fixed number */
	thread * threads = new thread[numThreads];

	/* For determining toWrite access index values for each thread */
	int baseNumItems = toParse->size() / numThreads;	
	int overflow = toParse->size() % numThreads;
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
		cout << "Creating thread " << i << endl;		

		threads[i] = thread(&SNPParser::parseThread, SNPParser(),
									toParse, alleleFreq, confScore, 
									this->toWrite,
									lowerLimit, upperLimit);

		cout << "Thread is created and running! " << endl;

		/* Update currIndex */
		currIndex += threadNumItems;
	}

	/* Wait for all threads to terminate */
	cout << "Joining threads..." << endl;
	for (int i = 0; i < numThreads; i++) {
		cout << "Waiting for thread " << i << " to join..." << endl;
		threads[i].join();
	}

	delete [] threads;

}

/* Thread that does the parsing */
void SNPParser::parseThread(vector<string> * toParse, 
						int alleleFreq, int confScore,
						vector<ParsedSNP *> * toWrite,
						int lowLimit, int upLimit) {

	/* Converter initialisation */
	Converter c;

	/* Loop through index limits, parse accordingly */
	// for (int i = lowLimit; i <= upLimit; i++) {
	// 	ParsedSNP * pSNP = c.convert((*toParse)[i], alleleFreq, confScore);

	// 	/* Cases when the toParse SNP is parsed, eg due to parameters */
	// 	if (pSNP != NULL) {
	// 		toWrite->push_back(pSNP);
	// 	}
	// }

	cout << "Thread is done processing. " << endl;
}

