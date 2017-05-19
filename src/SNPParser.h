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
#include "ParsedSNPWriter.h"

using namespace std;

class SNPParser {
	
	public:

		/* Constructor, destructor */
		SNPParser();
		~SNPParser();

		/* Set number of threads */
		void setNumThreads(int nThreads);

		/* Parse the given SNPs */
		void parseSNPs(vector<string *> * toParse, int alleleFreq, int confScore);

		/* Return the list of names by which the subfiles are created */
		vector<string> * getSubFileNames();

	private:
		/* 
		 * Thread that does the parsing.
		 * Each thread is given a specific name for its corresponding subfiles,
		 * generated from its ID
		 */
		void parseThread(string subfilename, vector<string *> * toParse, 
						int alleleFreq, int confScore,
						int lowLimit, int upLimit);
		
		/* Number of parsing threads to create */
		int numThreads;

		/* Fields needed by other classes */
		vector<string> * subfilenames;

};

#endif
