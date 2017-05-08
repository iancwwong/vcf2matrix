/* 
    Contains MAIN 
*/

#include <iostream>
#include <string>
#include <thread>

#include "SNPReader.h"
#include "SNPParser.h"
#include "ParsedSNP.h"
#include "ParsedSNPWriter.h"

using namespace std;

int main(int argc, char * argv[]) {

    /* Check proper usage - VCF file, allele freq, confidence score */
    if (argc < 4) {
        cout << "Usage: vcf2matrix <vcf file> <allele freq> <confidence score>" << endl;
        return 0;
    }

    /* Validate and parse arguments */
    string filename = string(argv[1]);
    int alleleFreq = atoi(argv[2]);
    int confScore = atoi(argv[3]);

    /* Read the VCF file */
    SNPReader reader;
    reader.readToParse(filename);
    vector<string> * toParse = reader.getToParse();

    /* Parse the data */
    SNPParser parser;
    parser.parseSNPs(toParse, alleleFreq, confScore);
    vector<ParsedSNP*> * toWrite = parser.getToWrite();
    cout << "Data parsed!" << endl;

    /* Write the data */
    cout << "Writing datat..." << endl;
    ParsedSNPWriter writer;
    writer.writeParsedSNP(toWrite, filename);
    cout << "Written data!" << endl;

    return 0;
}