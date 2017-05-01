/* 
    Contains MAIN 
*/

#include <iostream>
#include <string>
#include <thread>

#include "Reader.h"
#include "Monitor.h"
#include "Writer.h"

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

    /* Create the classes */
    Reader reader;
    Monitor monitor;
    Writer writer;

    /* Set up reader */
    reader.setInputFile(filename);
    reader.setMonitor(&monitor);

    /* Prepare monitor for parsing */
    monitor.setWriter(&writer);
    monitor.setParseParameters(alleleFreq, confScore);

    /* Prepare writer */
    string outputFilename = filename.substr(0, filename.find_last_of(".")); /* remove file extension component */
    writer.setOutputFilenames(outputFilename);

    /* Parse VCF appropriately */
    thread readerThread(&Reader::executeParse, &reader);
    thread monitorThread(&Monitor::executeParse, &monitor);
    thread writerThread(&Writer::executeParse, &writer);

    /* Block main thread until reader thread finishes */
    readerThread.join();

    /* Signal monitor that the reader process has finished 
        Then block until monitor thread finishes */
    monitor.signalPrevProcComplete();
    monitorThread.join();

    /* Signal writer that the monitor process has finished.
        Then block until writer process finishes */
    writer.signalPrevProcComplete();
    writerThread.join();

    return 0;
}