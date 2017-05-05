/**
 * ----------------------
 * CONVERSION CLASS
 * ----------------------
 * This class parses a line in a VCF file.
 * Its role is to understand the semantics of the data provided, and how 
 * it should be converted. 
**/

#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

#include "ParsedSNP.h"

using namespace std;

class Converter {

	public:

		/* Constructor, destructor */
		Converter();
		~Converter();

		/* Method to parse a line of data */
		ParsedSNP * convert(string data, int alleleFreq, int confScore);

};

#endif