/**
 * ----------------------
 * READER
 * ----------------------
 * This class forms the reading component behind VCF parsing, using a thread. 
 * It's completion criteria is:
 *  - when VCF file is completely read. 
 * 
 * Reads the VCF file sequentially (efficiency).
 * This class has the following roles:
 *  - Read and understand semantics of input (VCF) file
 *  - Feed each SNP line to monitor
**/

#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <iostream> /* Print error messages */
#include <vector>   /* contains toParse data */

using namespace std;

class Reader {
    public:

        /* Constructor and destructor */
        Reader();
        ~Reader();

        /* Obtain toParse vector */
        vector<string> * getToParseVector();

        /* Read a given file for SNPs */
        void readToParse(string inputFileName);

    private:
       vector<string> * toParse;
};

#endif