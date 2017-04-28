/**
 * ----------------------
 * WRITER
 * ----------------------
 * This class forms the writing component behind VCF parsing.
 * Able to accept parsed SNPs as encoded numbers, and write to
 * a specified external file.
**/

#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <string>

#include "ParsedSNP.h"

using namespace std;

class Writer {

    public:

        /* Constructor and destructor */
        Writer();
        ~Writer();

        /* Set the appropriate names for output */
        void setOutputFilenames(string filename);

        /* Write a VCF parsed line (ie matrix line) and record SNP location */
        void writeParsedSNPLine(ParsedSNP pSNP);

    private:
        ofstream outputMatrixFile;          /* Reference to output matrix file */
        ofstream outputLocFile;             /* Reference to output locations file */
        ofstream outputSamplesNamesFile;    /* Reference to sample names */
};

#endif