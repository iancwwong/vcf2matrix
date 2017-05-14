/**
 * Provides implementation for the Converter class
**/
#include <cstring> 		/* for splitting up data string based on delimiters */
#include <iostream> 	/* printing debug statements */

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
	
	/* For calculating allele frequency */
	int numSamples = 0;
	int * alleleFrequencies = new int[2];
	alleleFrequencies[0] = 0;
	alleleFrequencies[1] = 0;

	/* Prepare ParsedSNP */
	ParsedSNP * pSNP = new ParsedSNP();

	/* Track validity of given SNP */
	int invalidSNP = VALID_SNP;

	/* Split data into pieces iteratively - by the VCF standard, data is tab-delimited */
	char * colData = strtok_r(cstrdata, "\t", &cstrdata);
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
				invalidSNP = INDEL_REF;
				break;
			}
		}

		/* Alternate */
		else if (colNum == COL_ALT) {
			/* Early exit if INDEL or mulitiallelic */
			if (strlen(colData) > 1 || strcmp(colData, ".") == 0) {
				invalidSNP = INDEL_ALT;
				break;
			}
		}

		/* Quality, or confidence value */
		else if (colNum == COL_QUAL) {
			/* Early exit if less than than desired */
			int qual = atoi(colData);
			if (atoi(colData) < confScore) {
				invalidSNP = LOW_CONF_SCORE;
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

			char * genotypeData = strtok_r(colData, ":", &colData);

			if (genotypeData != NULL) {
				int * genotypeCount = Converter::countGenotype(genotypeData);

				/* Error with genotypeData occurred */
				if (genotypeCount == NULL) {
					invalidSNP = INVALID_GENOTYPE;
					break;
				}

				/* 
					Parse genotype combinations appropriately.
					Encoding rule:
						- 0|0: becomes '0'
						- 1|0: becomes '1'
						- 0|1 or 1|0: becomes '2'
				*/
				if ((genotypeCount[0] == 2 && genotypeCount[1] == 0)
					|| (genotypeCount[0] == 0 && genotypeCount[1] == 2)) {

					pSNP->parsed = pSNP->parsed + "0" + delimiter;
					alleleFrequencies[0]++;

				} else if (genotypeCount[0] == 1 && genotypeCount[1] == 1) {
					pSNP->parsed = pSNP->parsed + "1" + delimiter;
					alleleFrequencies[1]++;
				} 

				/* Another case not considered */
				else {
					invalidSNP = UNKNOWN_ERROR;
					break;
				}

				/* At this point, sample was parsed: increment numSamples */
				numSamples++;
			}
			
			/* Should not be null - early exit (invalid vcf SNP) */
			else {
				invalidSNP = NO_GENOTYPE_FOUND;
				break;
			}
		}

		colData = strtok_r(cstrdata, "\t", &cstrdata);
		colNum++;
	}

	/* Invalid SNP when no samples were evaluated */
	if (numSamples == 0) {
		invalidSNP = NO_SAMPLES_EVAL;
	}

	/* Case when an invalid SNP is detected - clean up, and return NULL */
	if (invalidSNP) {
		cout << "SNP was not valid. Error code: " << invalidSNP << endl;
		delete [] cstrdata;
		delete pSNP;
		return NULL;
	} 

	/* FURTHER CHECKS FOR SNP INVALIDITY */

	/* Invalid SNP when allele frequency criteria not met */
	if (Converter::calcAlleleFreq(alleleFrequencies) < (double)alleleFreq) {
		invalidSNP = LOW_ALLELE_FREQ;
	}

	/* Clean up */
	delete [] cstrdata;
	delete [] alleleFrequencies;

	/* Case when an invalid SNP is detected - clean up, and return NULL */
	if (invalidSNP) {
		cout << "SNP was not valid. Error code: " << invalidSNP << endl;
		delete [] cstrdata;
		delete pSNP;
		return NULL;
	} 
	
	/* SNP has been successfully parsed */
	else {
		delete [] cstrdata;
		// cout << "Showing details..." << endl;
		// pSNP->showDetails();
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

	char * genotype = strtok_r(genotypeStr, "/|", &genotypeStr);	/* '/' for halpotypes */
	while (genotype != NULL) {
		int gVal = atoi(genotype);
		
		/* Genotypes should ever be 0 (ref) or 1 (alt) */
		if (gVal >= 2 || gVal < 0) {
			validSNP = false;
			break;
		}

		/* Increment the count */
		genotype = strtok_r(genotypeStr, "/|", &genotypeStr);
		genotypeCount[gVal]++;
	}

	if (validSNP) {
		return genotypeCount;
	} else {
		delete [] genotypeCount;
		return NULL;
	}
}

/**
 * Calculate allele frequency
 *
 * In particular, calculate the percentage of samples that possess
 * the alternative allele
**/
double Converter::calcAlleleFreq(int * alleleFrequencies) {

	/* Prevent division by 0 */
	if ((alleleFrequencies[0] == 0) && (alleleFrequencies[1] == 0)) {
		return 0;
	}

	double total = (double)alleleFrequencies[0] + (double)alleleFrequencies[1];
	double alleleFreq = (double) alleleFrequencies[0] * 100 / total;

	return alleleFreq;
}