/*
 * This file implements the Concatenator class 
*/
#include <fstream>
#include <iostream> 		/* for printing debug statements */

#include "Concatenator.h"

/* Constructor, destructor */
Concatenator::Concatenator() {}
Concatenator::~Concatenator() {}

/* 
* Concatenate the sub-files, into two final sub-files: 
* one for location, one for matrix.
* The names for these files will be the specificied filename,
* appended with a "_output".
* 
* Each given subfile name in the vector contains two files:
* .location, .matrix
*/
void Concatenator::concatenate(vector<string> * subFileNames, string filename) {

	/* Prepare final output filename (remove file extension component) */
	string outputfn = filename.substr(0, filename.find_last_of(".")); 

	cout << "Merging these files: " << endl;
	for (int i = 0; subFileNames->size(); i++) {
		cout << "*" << (*subFileNames)[i] << endl;
	}

	/* Create the handlers to location and matrix files */
	// ofstream locFile(outputfn + "_locations.loc");
	// if (!locFile.is_open()) {
	// 	cout << "Unable to create a locations file." << endl;
	// 	return;
	// }
	// ofstream matrixFile(outputfn + "_matrix.matrix");
	// if (!matrixFile.is_open()) {
	// 	cout << "Unable to create a matrix file." << endl;
	// 	return;
	// }
}
