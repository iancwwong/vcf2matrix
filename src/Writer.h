/**
 * ----------------------
 * WRITER
 * ----------------------
 * This class forms the writing component behind VCF parsing.
 * Runs a thread that writes parsed SNPs, as read from a vector.
 * 
 * This class' completion criteria is:
 *  - Prev procedure is completed
 *  - toWrite vector is empty
**/

#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include <mutex>

#include "ParsedSNP.h"

using namespace std;

class Writer {

    public:

        /* Constructor and destructor */
        Writer();
        ~Writer();

        /* Preparation */
        void setOutputFilenames(string filename);

        /* Write the parsed SNPs */
        void executeParse();

        /* Public fields */
        vector<ParsedSNP> toWrite;          /* Reference to list of parsed SNPs to write */
        mutex toWrite_lock;                 /* Lock to toWrite vector. Used by other classes
                                                That attempt to modify toWrite. */

        /* Indicates previous procedure is completed. Used to terminate this class'
		executeParse() thread. */
		void signalPrevProcComplete();

    private:

        ofstream outputMatrixFile;          /* Reference to output matrix file */
        ofstream outputLocFile;             /* Reference to output locations file */
        ofstream outputSamplesNamesFile;    /* Reference to sample names */

        bool prevProcComplete;				/* Tracks whether previous process is completed */
};

#endif