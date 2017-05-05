/**
 * Provides implementation for the Converter class
**/
#include <cstring> 		/* for splitting up data string based on delimiters */

#include "Converter.h"

/* Constructor, destructor */
Converter::Converter() {};
Converter::~Converter() {};

/* 
	Convert to ParsedSNP function.

	ASSUMES A VALID SNP IS GIVEN

	Follows the VCF 4.2 column format, with the following 8 (tab-delimitered, order-specific) columns:
		CHROM	POS	ID	REF	ALT	QUAL	FILTER	INFO
	If there are genotypes to follow, a FORMAT field is given specifying the data types 
	and order (colon-separated alphanumeric String). Under this field, the first subfield (if there is any)
	must ALWAYS be the genotype (GT). .

	The full documentation of the VCF 4.2 standard can be found at:
	https://samtools.github.io/hts-specs/VCFv4.2.pdf

 */
ParsedSNP * Converter::convert(string data, int alleleFreq, int confScore) {

	/* Convert data into char * */
	char * cstrdata = new char[data.length() + 1];		/* remember to: delete [] cstrdata! */
	strcpy(cstrdata, data.c_str());

	/* Track the column number */
	unsigned int colNum = 1;

	/* Split data into pieces iteratively - by the VCF standard, data is tab-delimited */
	char * colData = strtok(cstrdata, "\t");
	while (colData != NULL) {
		
		/* Interpret the column data based on column number */
		
		/* Chromosome */
		if (colNum == COL_CHROM) {}

		/* Position */
		else if (colNum == COL_POS) {}

		/* ID */
		else if (colNum == COL_ID) {}

		/* Reference */
		else if (colNum == COL_REF) {}

		/* Alternate */
		else if (colNum == COL_ALT) {}

		/* Quality, or confidence value */
		else if (colNum == COL_QUAL) {}

		/* Filter */
		else if (colNum == COL_FILTER) {}

		/* Info */
		else if (colNum == COL_INFO) {}

		/* Format */
		else if (colNum == COL_FORMAT) {}

		/* Samples */
		else if (colNum >= COL_SAMPLE) {}

		colNum++;
	}

	delete [] cstrdata;
}