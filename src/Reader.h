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

class Reader {
    public:
        // Constructor & Destructor
        Reader();
        ~Reader();

        // Prepare reader, given an input file
        void prepare(char * filename, Monitor monitor);

        // Parses the input file
        void parse();

    private:
        Monitor monitor;    // Reference to monitor that controls parsing threads
}