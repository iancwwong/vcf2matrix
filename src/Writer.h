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
#include <queue>
#include <mutex>
#include <condition_variable>

#include "ParsedSNP.h"

using namespace std;

class Writer {

    public:

        /* Constructor and destructor */
        Writer();
        ~Writer();

        /* Preparation */
        void setOutputFilenames(string filename);

        /* Add data to write */
        void addToWriteData(ParsedSNP * pSNP);

        
        /* Indicates previous procedure is completed. Used to terminate this class'
		executeParse() thread. */
		void signalPrevProcComplete();

        /* Write the parsed SNPs */
        void executeWrite();                /* Consumer thread for toWrite vector */

        /* Public fields */
        queue<ParsedSNP *> * toWrite;           /* Reference to list of parsed SNPs to write */
        mutex * toWrite_lock;                   /* Lock the toWrite queue, and empty_toWrite flag. Used by other classes
                                                That attempt to modify toWrite. */

    private:
        
        ofstream * outputLocFile;             /* Reference to output locations file */
        ofstream * outputMatrixFile;          /* Reference to output matrix file */

        condition_variable * toWrite_empty_cv;  /* Used to wait for an empty toWrite list */
        bool empty_toWrite;
        
        bool prevProcComplete;				/* Tracks whether previous process is completed */
};

#endif