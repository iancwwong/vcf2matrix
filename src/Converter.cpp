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

	/* Prepare ParsedSNP */
	ParsedSNP * pSNP = new ParsedSNP();

	/* Validity of given SNP. An inalid SNP is one that does not fulfil the 
	given parameters, or some format error  */
	bool validSNP = true;

	/* Split data into pieces iteratively - by the VCF standard, data is tab-delimited */
	char * colData = strtok(cstrdata, "\t");
	while (colData != NULL) {
		
		/* Interpret the column data based on column number */
		
		/* Chromosome */
		if (colNum == COL_CHROM) {
			pSNP->chromosomeLoc = string(colData, strlen(colData));
		}

		/* Position */
		else if (colNum == COL_POS) {
			pSNP->pos = string(colData, strlen(colData));
		}

		/* ID */
		else if (colNum == COL_ID) {}

		/* Reference */
		else if (colNum == COL_REF) {
			/* Early exit if INDEL or mulitiallelic */
			if (strlen(colData) > 1 || strcmp(colData, ".") == 0) {
				validSNP = false;
				break;
			}
		}

		/* Alternate */
		else if (colNum == COL_ALT) {
			/* Early exit if INDEL or mulitiallelic */
			if (strlen(colData) > 1 || strcmp(colData, ".") == 0) {
				validSNP = false;
				break;
			}
		}

		/* Quality, or confidence value */
		else if (colNum == COL_QUAL) {
			/* Early exit if less than than desired */
			int qual = atoi(colData);
			if (atoi(colData) < confScore) {
				validSNP = false;
				break;
			}
		}

		/* Filter */
		else if (colNum == COL_FILTER) {}

		/* Info */
		else if (colNum == COL_INFO) {}

		/* Format */
		else if (colNum == COL_FORMAT) {}

		/* Samples */
		else if (colNum >= COL_SAMPLE) {
			/* First colon separated subfield is the genotype.
			NOTE: A genotype string from strtok is expected */
			char * genotypeData = strtok(colData, ":");
			if (genotypeData != NULL) {
				int * genotypeCount = Converter::countGenotype(genotypeData);

				/* Error with genotypeData occurred */
				if (genotypeCount == NULL) {
					validSNP = false;
					break;
				}

				/* Parse genotype combinations appropriately */
				if (genotypeCount[0] == 2 && genotypeCount[1] == 0) {
					pSNP->parsed = pSNP->parsed + "0" + delimiter;
				} else if (genotypeCount[0] == 1 && genotypeCount[1] == 1) {
					pSNP->parsed = pSNP->parsed + "1" + delimiter;
				} else if (genotypeCount[0] == 0 && genotypeCount[1] == 2) {
					pSNP->parsed = pSNP->parsed + "2" + delimiter;
				}

				/* Another case not considered */
				else {
					validSNP = false;
					break;
				}

				/* At this point, sample was parsed: increment numSamples */
				pSNP->numSamples++;
			}
			
			/* Should not be null - early exit (invalid vcf SNP) */
			else {
				validSNP = false;
				break;
			}
		}

		colNum++;
	}

	/* Invalid SNP when no samples were evaluated */
	if (colNum <= COL_SAMPLE) {
		validSNP = false;
	}

	/* Case when an invalid SNP is detected - clean up, and return NULL */
	if (!validSNP) {
		delete [] cstrdata;
		delete pSNP;
		return NULL;
	} 
	
	/* SNP has been successfully parsed */
	else {
		delete [] cstrdata;
		return pSNP;
	}
}

/** 
 * Count genotype (ie 0's and 1's). Will be a 2-index single dim array.
 * Usually genotypes are expressed: 0/0 or 0|0
*/
int * Converter::countGenotype(char * genotypeStr) {

	/* Initialise return array */
	int * genotypeCount = new int[2];
	genotypeCount[0] = 0;
	genotypeCount[1] = 0;

	bool validSNP = true;

	char * genotype = strtok(genotypeStr, "/|");	/* '/' for halpotypes */
	while (genotype != NULL) {
		int gVal = atoi(genotype);
		
		/* Genotypes should ever be 0 (ref) or 1 (alt) */
		if (gVal >= 2 || gVal < 0) {
			validSNP = false;
			break;
		}

		/* Increment the count */
		genotypeCount[gVal]++;
	}

	if (validSNP) {
		return genotypeCount;
	} else {
		delete [] genotypeCount;
		return NULL;
	}
}