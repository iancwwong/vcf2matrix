/* Implementation of SNPBasicParser.h */

#include "SNPBasicParser.h"
#include "CStringConverter.h"
#include <iostream>

/* Constructor, destructor */
SNPBasicParser::SNPBasicParser() {
	this->subfilenames = new vector<string>;
};

SNPBasicParser::~SNPBasicParser() {
	delete this->subfilenames;
};

/* Return the list of names by which the subfiles are created */
vector<string> * SNPBasicParser::getSubFileNames() {
	return this->subfilenames;
}

/* Parse the given SNPs */
void SNPBasicParser::parseSNPs(vector<string> * toParse, int alleleFreq, int confScore) {
		
	/* Writer intialisation */
	ParsedSNPWriter writer;
	string subfilename = "_basic_parsed";
	this->subfilenames->push_back(subfilename);
	writer.createSubFiles(subfilename);

	cout << "Problem not yet reached..." << endl;

	/* Converter initialisation */
	Converter c;

	/* Loop through index limits, parse accordingly */
	//#pragma omp parallel for
	for (int i = 0; i < toParse->size(); i++) {
		ParsedSNP * pSNP = c.convert((*toParse)[i], alleleFreq, confScore);

		/* Cases when the SNP is successfully parsed: write */
		if (pSNP != NULL) {
			//#pragma omp critical(writeSNP)
			writer.writeParsedSNP(pSNP);
		}
	}
	cout << "All written!" << endl;

	/* Close the subfiles */
	writer.closeSubFiles();
	cout << "Closed file " << endl;
}