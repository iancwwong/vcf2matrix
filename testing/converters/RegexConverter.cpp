/**
 * Provides implementation for the Converter class
**/
//#include <cstring> 		/* for splitting up data string based on delimiters */
#include <iostream> 	/* printing debug statements */
#include <regex>		/* For extracting desired fields from string */

#include "RegexConverter.h"

/* Regexes used to extract info */
/* Extracts: Ref, Alt, and Qual */
#define FIRST_REGEX "^(?:[^\t]*\t){3}([^\t]*)\t([^\t]*)\t([^\t]*)\t.*$"

/* Extracts: Chr, pos, and genotypes */
#define SECOND_REGEX "^([^\t]*)\t([^\t]*)\t(?:[^\t]*\t){7}(.*)$"

/* Regex match offsets */
#define INDEX_REF 1
#define INDEX_ALT 2
#define INDEX_QUAL 3

#define INDEX_CHROMOSOME 1
#define INDEX_POS 2
#define INDEX_GENOTYPES 3

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

	/* Regex extract the Reference, Alternative, and Qual columns */
	regex firstRegex ("^(?:[^\t]*\t){3}([^\t]*)\t([^\t]*)\t([^\t]*)\t.*$");
	smatch firstMatches;
	regex_match(data, firstMatches, firstRegex);

	string ref = *(firstMatches.begin() + INDEX_REF);
	string alt = *(firstMatches.begin() + INDEX_ALT);
	string qual = *(firstMatches.begin() + INDEX_QUAL);

	/* Indel cases */
	if (ref.length() >= 2 || ref[0] == '.' || alt.length() >= 2 || alt[0] == '.') {
		cout << "Indel detected!" << endl;
		return NULL;
	}

	/* Case when conf score is too low */
	int conf = stoi(qual, nullptr);
	if (conf < confScore) {
		cout << "Too low conf score!" << endl;
		return NULL;
	}

	/* Regex match the data to extract: chrom, pos, and genotypes */
	regex secondRegex (SECOND_REGEX);
	smatch secondMatches;
	regex_match(data, secondMatches, secondRegex);

	/* Prepare the ParsedSNP to return */
	ParsedSNP * pSNP = new ParsedSNP();

	pSNP->chromosomeLoc = *(secondMatches.begin() + INDEX_CHROMOSOME);
	pSNP->pos = *(secondMatches.begin() + INDEX_POS);

	string genotypeStr = *(secondMatches.begin() + INDEX_GENOTYPES);

	/* Invalid SNP due to too high alternative allele freq */
	int result = translateGenotypes(&genotypeStr, pSNP, alleleFreq);
	if (result == HIGH_ALT_ALLELE_FREQ) {
		delete pSNP;
		return NULL;
	}

	return pSNP;
}

/* Translate the genotype string.
	Assume string has length > 0, and SNP is valid. */
int Converter::translateGenotypes(string * genotypeStr, ParsedSNP * pSNP, int alleleFreq) {

	/* Maintain a count of number of samples with alternative alleles */
	int genotypeCount[3] = {0,0,0};

	/* Split the genotypes (tab-delimitered) */
	string splitDelim = "\t";
	int splitDelimLength = splitDelim.length();
	int delimIndex = genotypeStr->find(splitDelim);
	string genotype;
	while (delimIndex != string::npos) {
		/* Edge case: end of the line */
		genotype = genotypeStr->substr(0, delimIndex);

		if (genotype[0] == genotype[2]) {
			if (genotype[0] == '0') {
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

		/* Erase the extracted / converted genotype from the original string */
		genotypeStr->erase(0, delimIndex + splitDelimLength);
		delimIndex = genotypeStr->find(splitDelim);

	}

	/* Last genotype */
	if ((*genotypeStr)[0] == (*genotypeStr)[2]) {
		if ((*genotypeStr)[0] == '0') {
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

	/* Calculate allele frequency */
	double numAltAlleleSamples = genotypeCount[1] + genotypeCount[2];
	double totalSamples = genotypeCount[0] + numAltAlleleSamples;
	double freq = (numAltAlleleSamples / totalSamples) * 100;

	if (freq > alleleFreq) {
		cout << "High alt allele freq!" << endl;
		return HIGH_ALT_ALLELE_FREQ;
	}

	return 0;
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