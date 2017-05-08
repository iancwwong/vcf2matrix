/* 
    Contains MAIN 
*/

#include <iostream>
#include <string>
#include <thread>

#include "SNPReader.h"
#include "SNPParser.h"
#include "Concatenator.h"

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
    cout << "Reading vcf...";
    SNPReader reader;
    reader.readToParse(filename);
    cout << "Vcf file read!" << endl;

    /* Parse the data, and write parsed data to subfiles */
    cout << "Parsing and writing data...";
    vector<string> * toParse = reader.getToParse();
    SNPParser parser;
    parser.setNumThreads(4);
    parser.parseSNPs(toParse, alleleFreq, confScore);
    cout << "Data parsed, sub files generated!" << endl;

    /* Concatenate the parsed subfiles */
    cout << "Concatenating files...";
    vector<string> * subFileNames = parser.getSubFileNames();
    Concatenator c;
    c.concatenate(subFileNames, filename);
    cout << "Files concatenated!" << endl;

    return 0;
}