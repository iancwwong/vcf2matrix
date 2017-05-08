/**
 * ----------------------
 * CONCATENATOR
 * ----------------------
 * Concatenates sub files that contains parsed SNP details:
 *	- locations
 *	- matrix
**/

#ifndef CONCATENATOR_H
#define CONCATENATOR_H

#include <string>
#include <vector>

using namespace std;

class Concatenator {
    public:

        /* Constructor and destructor */
        Concatenator();
        ~Concatenator();

        /* 
         * Concatenate the sub-files, into two final sub-files: 
         * one for location, one for matrix.
         * The names for these files will be the specificied filename,
         * appended with a "_output".
         * 
         * Each given subfile name in the vector contains two files:
         * .location, .matrix
        */
        void concatenate(vector<string> * subFileNames, string filename);

};

#endif