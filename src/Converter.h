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

/* Constants */
/* VCF SNP Column representations */
#define COL_CHROM 1
#define COL_POS 2
#define COL_ID 3
#define COL_REF 4
#define COL_ALT 5
#define COL_QUAL 6
#define COL_FILTER 7
#define COL_INFO 8
#define COL_FORMAT 9
#define COL_SAMPLE 10

#define delimiter " "

/* Class definition */
class Converter {

	public:

		/* Constructor, destructor */
		Converter();
		~Converter();

		/* Method to parse a line of data */
		ParsedSNP * convert(string data, int alleleFreq, int confScore);
	
	private:
		/* Count genotype (ie 0's and 1's) */
		int * countGenotype(char * genotypeStr);

};

#endif