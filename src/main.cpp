/* 
    Contains MAIN
*/

#include <iostream>
#include <string>
#include "Reader.h"
#include "Monitor.h"
#include "Writer.h"

using namespace std;

int main(int argc, char * argv[]) {

    // Check proper usage - VCF file, allele freq, confidence score,
    if (argc < 3) {
        cout << "Usage: vcf2matrix <vcf file> <allele freq> <confidence score>" << endl;
        return 0;
    }

    // Validate and parse arguments
    string filename = str(argv[0]);
    int alleleFreq = stoi(argv[1], nullptr);
    int confScore = stoi(argv[2], nullptr);

    cout << "Input VCF file: " << filename << endl;
    cout << "Allele freq: " << alleleFreq << endl;
    cout << "Confidence Score: " << confScore << endl;

    // Prepare writer
    string outputFilename = filename.substr(0, filename.find_last_of(".")); // remove file extension component
    Writer writer(outputFilename);

    // Prepare monitor for concurrency parsing
    Monitor monitor;
    monitor.setWriter(writer);

    // Set up reader
    Reader reader(filename);
    reader.setMonitor(monitor);
    reader.setParseParameters(alleleFreq, confScore);

    // Parse VCF into a matrix text file
    reader.parse();

    return 0;
}