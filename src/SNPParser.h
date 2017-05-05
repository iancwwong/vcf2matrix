/**
 * ----------------------
 * SNP PARSER
 * ----------------------
 * This class focuses on parsing SNPs
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

		/* Parse the given SNPs */
		void parseSNPs(vector<string> * toParse, int alleleFreq, int confScore);

		/* Return the parsed SNPs */
		vector<ParsedSNP *> * getToWrite();

	private:
		/* Thread that does the parsing */
		void parseThread(vector<string> * toParse, 
						int alleleFreq, int confScore,
						vector<ParsedSNP *> * toWrite,
						int lowLimit, int upLimit);
		vector<ParsedSNP *> * toWrite;

};

#endif
