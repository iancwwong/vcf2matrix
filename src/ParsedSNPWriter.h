/**
 * ----------------------
 * PARSED SNP WRITER
 * ----------------------
 * This class forms the writing component behind VCF parsing.
 * Able to write parsed SNPs to subfiles with a given name.
**/

#ifndef PARSEDSNPWRITER_H
#define PARSEDSNPWRITER_H

#include <string>
#include <fstream>

#include "ParsedSNP.h"

using namespace std;

class ParsedSNPWriter {

    public:

        /* Constructor and destructor */
        ParsedSNPWriter();
        ~ParsedSNPWriter();

        /* Open a file to write parsed SNPs */
        void createSubFile(string filename);

        /* Write a given parsed SNP into the opened file */
        void writeParsedSNP(ParsedSNP * toWrite);

    private: 

        /* File handler for the files that'll contain the parsed SNP info */
        ofstream * locFile;
        ofstream * matrixFile;
};

#endif