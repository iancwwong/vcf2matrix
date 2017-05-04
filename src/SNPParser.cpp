/**
 * Provides implementation for the SNPParser class
**/
#include <thread>

#include "SNPParser.h"

/* Constructor, destructor */
SNPParser::SNPParser() {
	this->toWrite = new vector<ParsedSNP *>;
};

SNPParser::~SNPParser() {
	delete this->toWrite;
};

/* Return the parse SNPs */
vector<ParsedSNP *> * SNPParser::getToWrite() {
	return this->toWrite;
}

/* Parse the given SNPs */
void SNPParser::parseSNPs(vector<string> * toParse) {

}

/* Thread that does the parsing */
void SNPParser::parseThread(vector<string> * toParse, int lowLimit, int upLimit) {

}

