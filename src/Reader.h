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

#include "Monitor.h"

using namespace std;

class Reader {
    public:

        /* Constructor and destructor */
        Reader();
        ~Reader();

        /* Set up the input file */
        void setInputFile(string inputFilename);

        /* Prepares the monitor that will be used for parsing */
        void setMonitor(Monitor * mon);
  
        /* Parses the SNPs in the vcf file */
        void executeParse();

    private:
        Monitor * monitor;        /* Reference to monitor that controls parsing threads */
        fstream inputVCFFile;   /* Reference to input VCF file */
        int numThreads;         /* Number of threads that should be run, 
                                based on the properties of the input VCF file */
};

#endif