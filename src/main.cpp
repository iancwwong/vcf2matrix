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
    if (argc < 3) {
        cout << "Usage: vcf2matrix <vcf file> <allele freq> <confidence score>" << endl;
        return 0;
    }

    /* Validate and parse arguments */
    string filename = string(argv[0]);
    int alleleFreq = stoi(argv[1], nullptr);
    int confScore = stoi(argv[2], nullptr);

    cout << "Input VCF file: " << filename << endl;
    cout << "Allele freq: " << alleleFreq << endl;
    cout << "Confidence Score: " << confScore << endl;

    /* Read the VCF file */
    SNPReader reader;
    reader.readToParse(filename);
    vector<string> * toParse = reader.getToParse();

    /* Parse the data */
    SNPParser parser;
    parser.parseSNPs(toParse);
    vector<ParsedSNP> * toWrite = parser.getToWrite();

    /* Write the data */
    ParsedSNPWriter writer;
    writer.writeParsedSNP(toWrite, filename);

    return 0;
}