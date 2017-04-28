/**
 * ----------------------
 * READER
 * ----------------------
 * This class forms the reading component behind VCF parsing. 
 * Reads the VCF file TWICE - once for preparation, and once for parsing.
 * It is also responsible for managing the threads that parse SNPs.
 * This class has the following roles:
 *  - Read and understand semantics of input (VCF) file
 *  - Determine number of 'Thread' classes to create (based on criteria)
 *  - Feed each SNP line to a 'Thread' class for processing
**/

#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>

#include "Monitor.h"

using namespace std;

class Reader {
    public:

        /* Constructor and destructor */
        Reader();
        ~Reader();

        /* Set up the input file */
        void setInputfile(string inputFilename);

        /* Prepares the monitor that will be used for parsing */
        void setMonitor(Monitor mon);
  
        /* Parses the SNPs in the vcf file */
        void parseSNPs();

    private:
        Monitor monitor;        /* Reference to monitor that controls parsing threads */
        fstream inputVCFFile;   /* Reference to input VCF file */
        int numThreads;         /* Number of threads that should be run, 
                                based on the properties of the input VCF file */
};

#endif