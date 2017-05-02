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
		void parseSNPs(vector<string> * toParse);

		/* Return the parsed SNPs */
		vector<ParsedSNP *> * getToWrite();

	private:
		vector<ParsedSNP *> * toWrite;

};

#endif
