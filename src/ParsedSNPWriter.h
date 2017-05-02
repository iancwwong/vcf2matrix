/**
 * ----------------------
 * PARSED SNP WRITER
 * ----------------------
 * This class forms the writing component behind VCF parsing.
 * Runs a thread that writes parsed SNPs, as read from a vector.
 * 
 * This class' completion criteria is:
 *  - Prev procedure is completed
 *  - toWrite vector is empty
**/

#ifndef PARSEDSNPWRITER_H
#define PARSEDSNPWRITER_H

#include <string>
#include <vector>
#include <fstream>

#include "ParsedSNP.h"

using namespace std;

class ParsedSNPWriter {

    public:

        /* Constructor and destructor */
        ParsedSNPWriter();
        ~ParsedSNPWriter();

        /* Given parsed SNP's, write to a specific file with
           given file name */
        void writeParsedSNP(vector<ParsedSNP *> * toWrite, string filename);

    private: 
        /* Thread function to write */
        void writeThread(ofstream * locFile, ofstream * matrixFile, 
                        vector<ParsedSNP *> * toWrite, int lowLimit, int upLimit);
};

#endif