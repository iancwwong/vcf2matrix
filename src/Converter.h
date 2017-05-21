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

/* Error codes for invalid SNPs */
#define VALID_SNP 0
#define INDEL_REF 1
#define INDEL_ALT 2
#define LOW_CONF_SCORE 3
#define INVALID_GENOTYPE 4
#define HIGH_ALT_ALLELE_FREQ 5
#define NO_SAMPLES_EVAL 6
#define NO_GENOTYPE_FOUND 7

#define UNKNOWN_ERROR 50

/* Delimiter used to separate the encoded matrix values */
#define DELIM_CHAR " "

/* Class definition */
class Converter {

	public:

		/* Constructor, destructor */
		Converter();
		~Converter();

		/* Method to parse a line of data. Returns NULL if SNP was invalid. */
		ParsedSNP * convert(string data, int alleleFreq, int confScore);
	
	private:

		/* String version of the delimiter */
		string delimiter = string(DELIM_CHAR);

};

#endif