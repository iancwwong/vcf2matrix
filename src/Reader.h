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
#include <mutex>    /* Acquiring lock to 'toParse' in monitor */ 

#include "Monitor.h"

using namespace std;

class Reader {
    public:

        /* Constructor and destructor */
        Reader();
        ~Reader();

        /* Set up the input file */
        void setInputFile(string inputFilenameStr);

        /* Prepares the monitor that will be used for parsing */
        void setMonitor(Monitor * mon);

        /* Returns number of SNP lines */
        int getNumLines();
  
        /* Parses the SNPs in the vcf file */
        void executeParse();            /* Producer thread for the 'toParse' vector (located within monitor) */

    private:
        Monitor * monitor;              /* Reference to monitor that controls parsing threads */
        string inputFilename;           /* Reference to input VCF file name */
        int numLines;                   /* FUTURE: Number of threads that should be run, 
                                           based on the properties of the input VCF file */
};

#endif