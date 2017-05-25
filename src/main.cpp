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
    if (argc < 4) {
        cout << "Usage: vcf2matrix <vcf file> <allele freq> <confidence score>" << endl;
        return 0;
    }

    /* Validate and parse arguments */
    string filename = string(argv[1]);
    int alleleFreq = stoi(argv[2], nullptr);
    int confScore = stoi(argv[3], nullptr);

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
    int numThreads = 1;
    monitor.setNumThreads(numThreads);
    monitor.setWriter(&writer);
    monitor.setParseParameters(alleleFreq, confScore);

    /* Prepare writer */
    string outputFilename = filename.substr(0, filename.find_last_of(".")); /* remove file extension component */
    writer.setOutputFilenames(outputFilename);

    /* Parse VCF appropriately */
    cout << "Starting up threads...." << endl;
    thread writerThread(&Writer::executeWrite, &writer);
    thread monitorThread(&Monitor::executeParse, &monitor);
    thread readerThread(&Reader::executeRead, &reader);

    /* Block main thread until reader thread finishes */
    cout << "Waiting for reader to complete..." << endl;
    readerThread.join();

    /* Signal monitor that the reader process has finished 
        Then block until monitor thread finishes */
    cout << "Waiting for parsing to complete..." << endl;
    monitor.signalPrevProcComplete();
    monitorThread.join();

    /* Signal writer that the monitor process has finished.
        Then block until writer process finishes */
    cout << "Waiting for writing to complete..." << endl;
    writer.signalPrevProcComplete();
    writerThread.join();

    cout << "VCF parsing complete!" << endl;
    return 0;
}