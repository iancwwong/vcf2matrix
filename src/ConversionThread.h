/**
 * ----------------------
 * CONVERSION THREAD
 * ----------------------
 * Essentially a consumer for the data to parse (as produced by the reader, and fed into Monitor),
 * and the producer for the writer.
 * 
 * Runs a thread that converts a single SNP located in the toParse vector within monitor.
 * 
 * During parsing, it increments numBusyThreads in Monitor, and decrements it
 * once the parsing is completed.
**/

#ifndef CONVERSIONTHREAD_H
#define CONVERSIONTHREAD_H

#include <vector>
#include <string>
//#include <mutex>

#include "Converter.h"
#include "ParsedSNP.h"

using namespace std;

class ConversionThread {
	public:
		/* Constructor & destructor */
		ConversionThread();
		~ConversionThread();

		/* Preparation */
		void setParseParameters(int alleleFreq, int confScore);

		/* 
			Parsing execution. 
			This thread needs access to the following (and their locks): 
				- toParse: Contains the SNPs to be parsed
				- toWrite: Contains the parsed SNPs
				- busyThreadCount: Increments it when parsing, decrements when completed
		*/

		void executeParse(vector<string> * toParse, mutex * toParse_lock,
						vector<ParsedSNP> * toWrite, mutex * toWrite_lock,
						int * busyThreadsCount, mutex * busyThreadsCount_lock);

	private:

		Converter * converter;			/* Reference to class that manages conversion */

		int alleleFreq;					/* Parsing parameters */
		int confScore;
};

#endif