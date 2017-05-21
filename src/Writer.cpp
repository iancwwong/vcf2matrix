/**
 * Implementation of the Writer class 
*/
#include "Writer.h"
#include "Debug.h"

/* Constructor, destructor */
Writer::Writer() {
	this->toWrite = new queue<ParsedSNP *>();
	this->toWrite_lock = new mutex();
	this->toWrite_empty_cv = new condition_variable();
	this->empty_toWrite = true;				/* queue is empty */
	this->outputLocFile = nullptr;
	this->outputMatrixFile = nullptr;
	this->prevProcComplete = false;				/* default */
}

Writer::~Writer() {
	/* queue data */
	delete this->toWrite;
	delete this->toWrite_lock;

	/* Make sure file handlers are CLOSED before deleting */
	if (this->outputLocFile != nullptr && this->outputLocFile->is_open()) {
		this->outputLocFile->close();
		delete this->outputLocFile;
		this->outputLocFile = nullptr;
	}
	if (this->outputMatrixFile != nullptr && this->outputMatrixFile->is_open()) {
		this->outputMatrixFile->close();
		delete this->outputMatrixFile;
		this->outputMatrixFile = nullptr;
	}
}

/* Preparation */
void Writer::setOutputFilenames(string filename) {

	/* Create the file handlers */
	this->outputLocFile = new ofstream(filename + ".loc");
	if (!this->outputLocFile->is_open()) {
		PrintDebugStatement("Writer", "Unable to create a locations file.");
		return;
	}
	this->outputMatrixFile = new ofstream(filename + ".matrix");
	if (!this->outputMatrixFile->is_open()) {
		PrintDebugStatement("Writer", "Unable to create a matrix file.");
		return;
	}
}

/* Indicates previous procedure is completed. Used to terminate this class'
executeRead() thread. */
void Writer::signalPrevProcComplete() {

	/* Wait until list is empty */
	std::unique_lock<std::mutex> emptyToWrite_lock(*this->toWrite_lock);
	this->toWrite_empty_cv->wait(emptyToWrite_lock, [&]{
		return this->empty_toWrite; 
	});

	this->prevProcComplete = true;
}

/* Add data to write */
void Writer::addToWriteData(ParsedSNP * pSNP) {
	/* Synchronisation */
	this->toWrite_lock->lock();
	this->toWrite->push(pSNP);
	this->empty_toWrite = false;
	this->toWrite_lock->unlock();
}

/* Write the parsed SNPs */
void Writer::executeWrite() {
	/* Keep running as long as the previous process is not complete */
	while (!this->prevProcComplete) {

		/* Remove item to write */
		ParsedSNP * toWritePSNP = nullptr;
		this->toWrite_lock->lock();
		if (this->toWrite->size() > 0) {
			toWritePSNP = this->toWrite->front();
			this->toWrite->pop();
		} 

		/* Case when list is empty - notify the cv (in case prev process has completed) */
		else {
			this->empty_toWrite = true;
			this->toWrite_empty_cv->notify_one();
		}
		this->toWrite_lock->unlock();

		/* Write the obtained ParsedSNP */
		if (toWritePSNP != nullptr) {
			*this->outputLocFile << toWritePSNP->chromosomeLoc << "," << toWritePSNP->pos << "\n";
			*this->outputMatrixFile << toWritePSNP->parsed << "\n";
		}

		// PrintDebugStatement("Writer::executeWrite", "Still writing...");
	}

}

