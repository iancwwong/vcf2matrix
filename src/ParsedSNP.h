/**
 * This class encapsulates the translated parsed SNP data. Namely:
 *	- SNP chromosome location
 *	- SNP Position
 * 	- Parsed numbers
*/

#ifndef PARSEDSNP_H
#define PARSEDSNP_H

#include <string>

using namespace std;

class ParsedSNP {
	public:

		/* Constructor, destructor */
		ParsedSNP();
		~ParsedSNP();

		/* Fields that are extracted from SNP data */
		string chromosomeLoc;
		string pos;
		string parsed;
		int numSamples;
};

#endif