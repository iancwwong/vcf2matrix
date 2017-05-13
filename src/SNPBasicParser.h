/**
 * ----------------------
 * SNP BASIC PARSER
 * ----------------------
 * This class focuses on parsing SNPs in a basic way
 * ie a simplified version of SNP parser, without threads
**/

#ifndef SNPBASICPARSER_H
#define SNPBASICPARSER_H

#include <string>
#include <vector>

#include "ParsedSNP.h"
#include "ParsedSNPWriter.h"

using namespace std;

class SNPBasicParser {
	public:
		/* Constructor, destructor */
		SNPBasicParser();
		~SNPBasicParser();

		/* Parse the given SNPs */
		void parseSNPs(vector<string> * toParse, int alleleFreq, int confScore);

		/* Return the list of names by which the subfiles are created */
		vector<string> * getSubFileNames();

	private:
		/* Fields needed by other classes */
		vector<string> * subfilenames;		
};

#endif