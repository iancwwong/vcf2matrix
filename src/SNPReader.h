/**
 * ----------------------
 * SNP READER
 * ----------------------
 * Reads the VCF file sequentially (efficiency).
 * This class has the following roles:
 *  - Read and understand semantics of input (VCF) file
 *  - Feed each SNP line to monitor
**/

#ifndef SNPREADER_H
#define SNPREADER_H

#include <string>
#include <vector>   /* contains toParse data */

using namespace std;

class SNPReader {
    public:

        /* Constructor and destructor */
        SNPReader();
        ~SNPReader();

        /* Obtain toParse vector */
        vector<string *> * getToParse();

        /* Read a given file for SNPs */
        void readToParse(string inputFileName);

    private:
       vector<string *> * toParse;
};

#endif