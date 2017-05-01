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

#include <vector>
#include <mutex>
#include <thread>

#include "Writer.h"
#include "ConversionThread.h"
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

        /* Thread that runs the parsing */
        void executeParse();

		/* Indicates previous procedure is completed. Used to terminate this class'
		executeParse() thread. */
		void signalPrevProcComplete();
		
		/* Public fields */
		vector<string> toParse;							/* The list of data to parse */
		mutex toParse_lock;	
		int * busyThreadsCount;							/* Number of threads that are busy. */
		mutex busyThreadsCount_lock;	

    private:
		Writer * writer;								/* Reference to Writer. Needs to access (and the corr lock):
																- toWrite vector */
 
		int numThreads;									/* Number of threads to manage */
		vector<thread *> conversionThreads;				/* The list of executing conversion threads that parse/process information */

		bool prevProcComplete;							/* Tracks whether previous process is completed */

		int alleleFreq;									/* Parsing purposes: These are arbitrary parsing parameters */
		int confScore;
		
};

#endif