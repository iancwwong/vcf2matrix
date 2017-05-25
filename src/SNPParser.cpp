/**
 * Provides implementation for the SNPParser class
**/
#include <thread>
#include <iostream>		/* for printing debug statements */

#include "SNPParser.h"
#include "CStringConverter.h"

/* Constructor, destructor */
SNPParser::SNPParser() {
	this->subfilenames = new vector<string>;
	this->numThreads = 1;		/* default */
};

SNPParser::~SNPParser() {
	delete this->subfilenames;
};

void SNPParser::setNumThreads(int nThreads) {
	this->numThreads = nThreads;
}

/* Return the list of names by which the subfiles are created */
vector<string> * SNPParser::getSubFileNames() {
	return this->subfilenames;
}


/* Parse the given SNPs */
void SNPParser::parseSNPs(vector<string> * toParse, int alleleFreq, int confScore) {

	/* Reduce number of threads in case numthreads > numlines to parse */
	if (this->numThreads > toParse->size()) {
		this->numThreads = 1;
		cout << "Threads changed to 1..." << endl;
	}

	/* Create and run the threads that will parse data */
	thread * threads = new thread[this->numThreads];

	/* For determining toWrite access index values for each thread */
	int baseNumItems = toParse->size() / this->numThreads;	
	int overflow = toParse->size() % this->numThreads;
	int currIndex = 0;

	for (int i = 0; i < this->numThreads; i++) {

		/* Determine the index values */
		int threadNumItems = baseNumItems;
		if (overflow) {
			threadNumItems++;
			overflow--;
		}
		int lowerLimit = currIndex;
		int upperLimit = currIndex + threadNumItems - 1;

		/* Generate the subFileName from which the location and
		 * matrix subfiles (created by parser thread) are to follow
		 * 
		 * Currently: "__[thread number]"
		 */
		string subfilename = "__" + to_string(i);

		/* Create the thread */
		threads[i] = thread(&SNPParser::parseThread, this,
							subfilename, toParse, alleleFreq, confScore, 
							lowerLimit, upperLimit);

		/* Preserve created subfilename */
		this->subfilenames->push_back(subfilename);

		/* Update currIndex */
		currIndex += threadNumItems;
	}

	/* Wait for all threads to terminate */
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}

	delete [] threads;

}

/* Thread that does the parsing */
void SNPParser::parseThread(string subfilename, vector<string> * toParse, 
						int alleleFreq, int confScore,
						int lowLimit, int upLimit) {

	/* Writer intialisation */
	ParsedSNPWriter writer;
	writer.createSubFiles(subfilename);

	/* Converter initialisation */
	Converter c;

	/* Loop through index limits, parse accordingly */
	//#pragma omp parallel for
	for (int i = lowLimit; i <= upLimit; i++) {
		ParsedSNP * pSNP = c.convert((*toParse)[i], alleleFreq, confScore);

		/* Cases when the SNP is successfully parsed: write */
		if (pSNP != NULL) {
			//#pragma omp critical(writeSNP)
			writer.writeParsedSNP(pSNP);
		}
	}

	/* Close the subfiles */
	writer.closeSubFiles();
}

