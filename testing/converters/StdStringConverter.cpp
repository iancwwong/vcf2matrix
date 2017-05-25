/**
 * Provides implementation for the Converter class
**/
#include <cstring> 		/* for splitting up data string based on delimiters */
#include <iostream> 	/* printing debug statements */

#include "StdStringConverter.h"

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

	/* Track the column number */
	unsigned int colNum = 1;
	
	/* For calculating allele frequency */
	int genotypeCount[3] = {0,0,0};

	/* Prepare ParsedSNP */
	ParsedSNP * pSNP = new ParsedSNP();

	/* Track validity of given SNP */
	int invalidSNP = VALID_SNP;

	/* Split data into pieces iteratively - by the VCF standard, data is tab-delimited */
	string splitDelim = "\t";
	int splitDelimLength = splitDelim.length();
	size_t delimIndex = data.find(splitDelim, 0);	/* Start from beginning of string */
	size_t currIndex = 0;
	string colData;
	while (delimIndex != string::npos) {

		/* Extract the column data */
		colData = data.substr(currIndex,delimIndex - currIndex);
		
		/* Interpret the column data based on column number */
		/* Chromosome */
		if (colNum == COL_CHROM) {
			pSNP->chromosomeLoc = colData;
		}

		/* Position */
		else if (colNum == COL_POS) {
			pSNP->pos = colData;
		}

		/* Reference */
		else if (colNum == COL_REF) {
			/* Early exit if INDEL or mulitiallelic */
			if (colData.length() > 1 || colData[0] == '.') {
				invalidSNP = INDEL_REF;
				break;
			}
		}

		/* Alternate */
		else if (colNum == COL_ALT) {
			/* Early exit if INDEL or mulitiallelic */
			if (colData.length() > 1 || colData[0] == '.') {
				invalidSNP = INDEL_REF;
				break;
			}
		}

		/* Quality, or confidence value */
		else if (colNum == COL_QUAL) {
			/* Early exit if less than than desired */
			int qual = stoi(colData, nullptr);
			if (qual < confScore) {
				invalidSNP = LOW_CONF_SCORE;
				break;
			}
		}

		/* Samples */
		else if (colNum >= COL_SAMPLE) {

			/* Convert the genotype */
			if (colData[0] == colData[2]) {
				if (colData[0] == '0') {
					pSNP->parsed += "0" + this->delimiter;
					genotypeCount[0]++;
				} else {
					pSNP->parsed += "1" + this->delimiter;
					genotypeCount[1]++;
				}
			} else {
				pSNP->parsed += "2" + this->delimiter;
				genotypeCount[2]++;
			}
			
		}

		/* Move to next column */
		currIndex = delimIndex+1;
		delimIndex = data.find(splitDelim, currIndex);
		colNum++;
	}

	/* Last tab delimitered column */
	colNum++;
	colData = data.substr(currIndex);
	if (colNum >= COL_SAMPLE) {

			/* Convert the genotype */
			if (colData[0] == colData[2]) {
				if (colData[0] == '0') {
					pSNP->parsed += "0" + this->delimiter;
					genotypeCount[0]++;
				} else {
					pSNP->parsed += "1" + this->delimiter;
					genotypeCount[1]++;
				}
			} else {
				pSNP->parsed += "2" + this->delimiter;
				genotypeCount[2]++;
			}
	}
	
	/* Invalid SNP when no samples were evaluated */
	if (colNum < COL_SAMPLE) {
		invalidSNP = NO_SAMPLES_EVAL;
	} 

	/* Case when an invalid SNP is detected - clean up, and return NULL */
	if (invalidSNP) {
		cout << "SNP was not valid. Error code: " << invalidSNP << endl;
		delete pSNP;
		return NULL;
	} 

	/* FURTHER CHECKS FOR SNP INVALIDITY */

	/* Allele frequency criteria */
	double numAltAlleleSamples = genotypeCount[1] + genotypeCount[2];
	double totalSamples = genotypeCount[0] + numAltAlleleSamples;
	double freq = (numAltAlleleSamples / totalSamples) * 100;
	if (freq > alleleFreq) {
		cout << "High alt allele freq!" << endl;
		invalidSNP = HIGH_ALT_ALLELE_FREQ;
	}

	/* Case when an invalid SNP is detected - clean up, and return NULL */
	if (invalidSNP) {
		cout << "SNP was not valid. Error code: " << invalidSNP << endl;
		delete pSNP;
		return NULL;
	} 
	
	/* SNP has been successfully parsed */
	else {
		return pSNP;
	}
}