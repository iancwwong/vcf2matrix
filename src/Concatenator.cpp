/*
 * This file implements the Concatenator class 
*/
#include <fstream>
#include <cstdio>			/* removing files */
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

	/* Create the handlers to location and matrix files */
	ofstream locFile(outputfn + "_locations.loc");
	if (!locFile.is_open()) {
		cout << "Unable to create a locations file." << endl;
		return;
	}
	ofstream matrixFile(outputfn + "_matrix.matrix");
	if (!matrixFile.is_open()) {
		cout << "Unable to create a matrix file." << endl;
		return;
	}

	/* Iterate over each subfilename, merging them into the final files */
	for (int i = 0; i < subFileNames->size(); i++) {

		string subfilename = (*subFileNames)[i];

		/* Merge location */
		string locationSubfilename = subfilename + ".loc";
		ifstream locToMerge(locationSubfilename);
		locFile << locToMerge.rdbuf();
		locToMerge.close();
		if (remove(locationSubfilename.c_str()) != 0) {
			cout << "Error removing location subfile: " << locationSubfilename << endl;
		}

		/* Merge matrix */
		string matrixSubfilename = subfilename + ".matrix";
		ifstream matrixToMerge(matrixSubfilename);
		matrixFile << matrixToMerge.rdbuf();
		matrixToMerge.close();
		if (remove(matrixSubfilename.c_str()) != 0) {
			cout << "Error removing matrix subfile: " << matrixSubfilename << endl;
		}
	}

	/* Close the final output files */
	locFile.close();
	matrixFile.close();
}
