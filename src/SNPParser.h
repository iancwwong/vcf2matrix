/**
 * ----------------------
 * SNP PARSER
 * ----------------------
 * This class focuses on parsing SNPs.
 * Every time a thread parses a SNP, it will be written to a subfile.
 * Each thread is in charge of a subset of SNPs, and one corresponding sub-files name, 
 * which are to contain their parsed details.
**/

#ifndef SNPPARSER_H
#define SNPPARSER_H

#include <string>
#include <vector>

#include "ParsedSNP.h"

using namespace std;

class SNPParser {
	
	public:

		/* Constructor, destructor */
		SNPParser();
		~SNPParser();

		/* Set number of threads */
		void setNumThreads(int nThreads);

		/* Parse the given SNPs */
		void parseSNPs(vector<string> * toParse, int alleleFreq, int confScore);

		/* Return the parsed SNPs */
		vector<ParsedSNP *> * getToWrite();

		/* Return the list of names by which the subfiles are created */
		vector<string> * getSubFileNames();

	private:
		/* 
		 * Thread that does the parsing.
		 * Each thread is given a specific name for its corresponding subfiles,
		 * generated from its ID
		 */
		void parseThread(string subFileName, vector<string> * toParse, 
						int alleleFreq, int confScore,
						vector<ParsedSNP *> * toWrite,
						int lowLimit, int upLimit);
		
		/* Number of parsing threads to create */
		int numThreads;

		/* Fields needed by other classes */
		vector<ParsedSNP *> * toWrite;
		vector<string> * subFileNames;

};

#endif
