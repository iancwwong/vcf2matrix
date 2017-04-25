/**
 * ----------------------
 * WRITER
 * ----------------------
 * This class forms the writing component behind VCF parsing.
 * Able to accept parsed SNPs as encoded numbers, and write to
 * a specified external file.
**/

class Writer {

    public:

        /* Constructor and destructor */
        Writer(char * outputFileName);
        ~Writer();

        /* Write a VCF parsed line (ie matrix line) and record SNP location */
        writeParsedSNPLine(char * snpLoc, char * parsedLine);

        /* Write the samples' names */
        writeSamplesNames(int numSamples, char * namesArray[]);

    private:
        ofstream outputMatrixFile;          /* Reference to output matrix file */
        ofstream outputLocFile;             /* Reference to output locations file */
        ofstream outputSamplesNamesFile;    /* Reference to sample names */
}