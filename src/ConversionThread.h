/**
 * ----------------------
 * CONVERSION THREAD
 * ----------------------
 * This class encapsulates a processing thread that parses a given input,
 * and then returns an output.
 * It utilises the 'Converter' class for conversion.
 * It's role:
 *	- Accept an input, put the converted data in a given memory location
**/

#ifndef CONVERSIONTHREAD_H
#define CONVERSIONTHREAD_H

#include <vector>
#include <string>
#include <mutex>

#include "Converter.h"
#include "ParsedSNP.h"

using namespace std;

class ConversionThread {
	public:
		/* Constructor & destructor */
		ConversionThread();
		~ConversionThread();

		/* Parsing */
		void parseSNPData(string data, vector<ParsedSNP> writeVector, mutex writeVector_lock);

	private:
		bool state;				/* Indicator whether thread is busy processing */
		Converter converter;	/* Reference to class that manages conversion */

		int alleleFreq;			/* Parsing parameters */
		int confScore;
};

#endif