/**
 * ----------------------
 * MONITOR
 * ----------------------
 * This term may be confused with the 'monitor' used in concurrency control,
 * however this class is simply a container for all the conversion threads.
 * 
 * This class contains the threads of execution that are vital to
 * parsing the VCF file. It contains the two types of threads:
 *	- a list of CONVERTER threads (that aim to convert given data)
 *
 * In particular, it has the following roles:
 * 	- Create a certain number of threads, and prepare them for execution (as well
 *	  as executing them)
 *	- Clean up threads when the overall procedure is finished
 *	- Keep track of a list of toParse SNPs (which are accessed by each thread)
 * 	
 * This class' completion criteria is:
 *	- Previous procedure is completed (eg reader)
 *	- toParse is empty
 *	- numBusyThreads = 0
 * 
**/

#ifndef MONITOR_H
#define MONITOR_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "Writer.h"
#include "ParsedSNP.h"

using namespace std;

class Monitor {

    public:

        /* Constructor and destructor */
        Monitor();
        ~Monitor();

		/* Preparation functions */
		void setWriter(Writer * writer);
        void setParseParameters(int alleleFreq, int confScore);
		void setNumThreads(int numThreads);

		/* Add data to the toParse */
		void addToParseData(string data);

		/* Indicates previous procedure is completed. Used to terminate this class'
		executeParse() thread. */
		void signalPrevProcComplete();

        /* Thread that starts and manages parsing */
        void executeParse();
		
		/* Public fields */
		queue<string> * toParse;							/* The list of data to parse */
		mutex * toParse_lock;

    private:
		Writer * writer;								/* Reference to Writer. Needs to access (and the corr lock):
																- toWrite vector */
 
		int numThreads;									/* Number of threads to manage */

		/* 
		 * Thread that does the parsing.
		 * Each thread is given a specific name for its corresponding subfiles,
		 * generated from its ID
		 */
		void parseThread(queue<string> * toParse, mutex * toParseLock,			// Thread is consumer for toParse vector
						condition_variable * toParse_empty_cv, bool * empty_toParse,
						Writer * writer,										// Thread is producer for toWrite vector (located inside writer)
						bool * prevProcComplete,								// Thread knows when to terminate
						int alleleFreq, int confScore);

		condition_variable * toParse_empty_cv;				/* Used to wait for an empty toParse list */
		bool empty_toParse;	

		bool prevProcComplete;							/* Tracks whether previous process is completed */

		int af;		/* Allele Frequency (parsing purposes) */
		int cs;		/* Confidence Score (parsing purposes) */
		
};

#endif