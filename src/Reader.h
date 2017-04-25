/**
 * ----------------------
 * READER
 * ----------------------
 * This class forms the reading component behind VCF parsing. 
 * It is also responsible for managing the threads that parse SNPs.
 * This class has the following roles:
 *  - Read and understand semantics of input (VCF) file
 *  - Determine number of 'Thread' classes to create (based on criteria)
 *  - Feed each SNP line to a 'Thread' class for processing
**/
#include "Monitor.h"

class Reader {
    public:

        /* Constructor and destructor */
        Reader(char * filename);
        ~Reader();

        /* Prepares the monitor that will be used for parsing */
        void setMonitor(Monitor mon);

        /* Set the parameters for parsing */
        void setParameters(int alleleFreq, int confScore);
  
        /* Parses the input vcf file */
        void parse();

    private:
        Monitor monitor;    /* Reference to monitor that controls parsing threads */

}