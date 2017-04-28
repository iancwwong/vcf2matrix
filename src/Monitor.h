/**
 * ----------------------
 * MONITOR
 * ----------------------
 * This class is the most complex in the tool.
 * 
 * This class contains the threads of execution that are vital to
 * parsing the VCF file. It contains the two types of threads:
 *	- a list of CONVERTER threads (that aim to convert given data)
 *	- a concurrency management thread that manages the converter threads 
 *    (transfer of data)
 *
 * In particular, it has the following roles:
 * 	- Create a certain number of threads, and prepare them for execution (as well
 *	  as executing them)
 *	- Clean up threads when the overall procedure is finished
 *	- Manage concurrency between conversion threads when attempting to write
 *	  converted data
 *	- Give threads the data to be converted
 *	- Distinguishing between different required conversion procedures (from 
 *	  a previous calling class), and make the threads execute appropriate functions
 * 	- Use the Writer class to write converted data
 *
 * To maximise efficiency for large writes, monitor should call the writer 
 * whenever ~4K Bytes are ready be written.
**/

#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include <mutex>

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
		/* Set the writer */
		void setWriter(Writer writer);

		/* Set the number of samples that exist in the VCF file */
		void setNumSamples(int numSamples);

		/* Set the number of threads to create */
		void setNumThreads(int numThreads);

		/* Set the parameters for parsing */
        void setParseParameters(int alleleFreq, int confScore);

		/* Create the threads and run them */
		void createAndExecuteThreads();

        /* Parse and write a VCF parsed line (ie matrix line) and record SNP location */
        void parseSNPLine(string snpData);

    private:
		Writer writer;									/* Reference to Writer */
        int numSamples;									/* number of samples that exist in a particular VCF file. 
															Used to determine number of lines that should be stored before writing */
		int numThreads;									/* number of conversion threads that this class will manage */
		vector<ConversionThread> conversionThreads;		/* The list of conversion threads that parse/process information */
		vector<ParsedSNP> toWrite;						/* The list of data to write. Flushed every time it's written' */
		mutex toWrite_lock;								/* Lock to the 'toWrite' vector - concurrency control.
															Acquire lock: toWrite_lock.lock()
															Destroy lock: toWrite_lock.unlock() */

		int alleleFreq;									/* Parsing purposes: These are arbitrary parsing parameters */
		int confScore;
		
};

#endif